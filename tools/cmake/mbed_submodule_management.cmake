# Copyright (c) 2025 Jamie Smith. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

option(MBED_MANAGE_SUBMODULES "If true, Mbed will clone submodules as-needed based on the build target(s) the user has selected." TRUE)
option(MBED_USE_SHALLOW_SUBMODULES "If true, clone submodules as shallow. This reduces disk space usage at the cost of making it difficult to make & push changes to the code in the submodule. Disabling this will cause all submodules to be recloned as non-shallow." TRUE)

#
# Function to set up a git submodule, cloning it if necessary.
#
# Usage: mbed_setup_submodule(<submodule path> CHECK_FILE <check file>
#            [IF_LABEL <label>])
#
#  - <submodule path> is the path of the submodule, relative to the current list dir
#  - <check file> is the path to some file that exists in the submodule. This is used to determine if the
#     submodule was cloned or not. This path should be relative to the submodule's root dir.
#  - If IF_LABEL <label> is passed, then the given label is checked in MBED_TARGET_LABELS and the submodule
#     is only cloned if the label is found.
#
# When this function runs, it will check the status of the submodule, and make sure it matches the current configuration.
# Specifically, it will perform the following checks:
# - Is the submodule cloned? If not, it will be cloned automatically (as long as MBED_MANAGE_SUBMODULES is true -- otherwise
#    an error will be printed)
# - Is the submodule on the same commit as that pointed to by the outer repo? If not, it will be automatically updated,
#    as long as it does not have any local changes.
# - Is the submodule
#
function(mbed_setup_submodule SUBMODULE_PATH)
    # Parse args
    cmake_parse_arguments(PARSE_ARGV 1 ARGS "" "CHECK_FILE;IF_LABEL" "")
    if("${ARGS_CHECK_FILE}" STREQUAL "")
        message(FATAL_ERROR "CHECK_FILE arg is required!")
    endif()

    # Do we need this submodule? If not, bail
    if(NOT "${ARGS_IF_LABEL}" STREQUAL "")
        if(NOT "${ARGS_IF_LABEL}" IN_LIST MBED_TARGET_LABELS)
            return()
        endif()
    endif()

    # Is the submodule cloned?
    set(FULL_SUBMODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/${SUBMODULE_PATH})
    file(RELATIVE_PATH SOURCE_DIR_REL_SUBMODULE_PATH ${CMAKE_SOURCE_DIR} ${FULL_SUBMODULE_PATH})
    if(EXISTS "${FULL_SUBMODULE_PATH}/${ARGS_CHECK_FILE}")
        set(SUBMODULE_CLONED TRUE)
    else()
        set(SUBMODULE_CLONED FALSE)
    endif()

    if(NOT MBED_MANAGE_SUBMODULES)
        if(NOT SUBMODULE_CLONED)
            message(FATAL_ERROR "Missing submodule at ${FULL_SUBMODULE_PATH}, and MBED_MANAGE_SUBMODULES is disabled so we cannot fix this automatically.\
To fix this, run the following commands:
cd ${CMAKE_CURRENT_LIST_DIR}
git submodule update --init ${SUBMODULE_PATH}")
        endif()

        # Not managing submodules so as long as it exists, ¯\_(ツ)_/¯
    endif()

    # Now we need git
    find_package(Git REQUIRED)

    # Clone the submodule if not done already
    if(NOT SUBMODULE_CLONED)

        # Github Actions has an issue where the clone of the repo is done as a different user
        # https://github.com/actions/checkout/issues/47
        # This causes an error like 'fatal: unsafe repository ('/__w/mbed-os/mbed-os' is owned by someone else)'
        # Other than chown-ing the source directory after the checkout step, it seems like the only fix
        # is to run the following command if we detect Github Actions
        if(NOT "$ENV{GITHUB_RUN_ID}" STREQUAL "")
            execute_process(
                COMMAND ${GIT_EXECUTABLE} config --global --add safe.directory ${CMAKE_SOURCE_DIR}
                COMMAND_ERROR_IS_FATAL ANY
                WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
                COMMAND_ECHO STDOUT
            )
        endif()


        if(MBED_USE_SHALLOW_SUBMODULES)
            set(SHALLOW_ARGS --depth 1)
        else()
            set(SHALLOW_ARGS "")
        endif()

        message(STATUS "Cloning git submodule ${SOURCE_DIR_REL_SUBMODULE_PATH}...")
        execute_process(
                COMMAND ${GIT_EXECUTABLE} submodule update --init ${SHALLOW_ARGS} ${SUBMODULE_PATH}
                COMMAND_ERROR_IS_FATAL ANY
                WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
                COMMAND_ECHO STDOUT
        )
    endif()

    # To get the current commit that the submodule is actually on, we need this command.
    # See https://stackoverflow.com/a/54238999
    execute_process(
            COMMAND ${GIT_EXECUTABLE} submodule status ${SUBMODULE_PATH}
            COMMAND_ERROR_IS_FATAL ANY
            OUTPUT_VARIABLE SUBMODULE_GIT_SUBMODULE_STATUS
            WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    )
    if(NOT "${SUBMODULE_GIT_SUBMODULE_STATUS}" MATCHES "^[ +]([0-9a-z]+)")
        message(WARNING "Unable to parse output of 'git submodule status ${SOURCE_DIR_REL_SUBMODULE_PATH}': \"${SUBMODULE_GIT_SUBMODULE_STATUS}\". Not touching this submodule.")
        return()
    endif()
    string(SUBSTRING ${CMAKE_MATCH_1} 0 8 SUBMODULE_CURRENT_HASH)

    # Check whether the submodule is on the right commit
    execute_process(
        COMMAND ${GIT_EXECUTABLE} status --porcelain=2 ${SUBMODULE_PATH}
        COMMAND_ERROR_IS_FATAL ANY
        OUTPUT_VARIABLE SUBMODULE_GIT_STATUS
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    )
    set(SUBMODULE_COMMIT_CHANGED FALSE)
    set(SUBMODULE_TRACKED_CHANGES FALSE)
    set(SUBMODULE_UNTRACKED_CHANGES FALSE)
    if("${SUBMODULE_GIT_STATUS}" STREQUAL "")
        # Submodule is up to date, OK
        set(SUBMODULE_SUPERPROJECT_POINTER_HASH ${SUBMODULE_CURRENT_HASH})
    elseif("${SUBMODULE_GIT_STATUS}" MATCHES "^1 A.")
        # Submodule newly added locally and not commited yet, OK.
        # Don't touch this submodule any more as it's not committed yet.
        return()
    else()
        # Some sort of change detected. Check what kind of change it is.
        # Format documented here: https://git-scm.com/docs/git-status#_changed_tracked_entries
        if(NOT "${SUBMODULE_GIT_STATUS}" MATCHES "^1 .M S(.)(.)(.) [0-9]+ [0-9]+ [0-9]+ ([0-9a-z]+)")
            message(WARNING "Unable to parse output of 'git status --porcelain=2 ${SOURCE_DIR_REL_SUBMODULE_PATH}': \"${SUBMODULE_GIT_STATUS}\". Not touching this submodule.")
            return()
        endif()
        if(${CMAKE_MATCH_1} STREQUAL "C")
            set(SUBMODULE_COMMIT_CHANGED TRUE)
        endif()
        if(${CMAKE_MATCH_2} STREQUAL "M")
            set(SUBMODULE_TRACKED_CHANGES TRUE)
        endif()
        if(${CMAKE_MATCH_3} STREQUAL "U")
            set(SUBMODULE_UNTRACKED_CHANGES TRUE)
        endif()
        string(SUBSTRING ${CMAKE_MATCH_4} 0 8 SUBMODULE_SUPERPROJECT_POINTER_HASH)

        if(SUBMODULE_COMMIT_CHANGED)
            if(SUBMODULE_TRACKED_CHANGES OR SUBMODULE_UNTRACKED_CHANGES)
                message(WARNING "Submodule ${SOURCE_DIR_REL_SUBMODULE_PATH} is on the wrong commit (should be ${SUBMODULE_SUPERPROJECT_POINTER_HASH}, is ${SUBMODULE_CURRENT_HASH}, but also has local modifications. Not touching this submodule. You may wish to commit your changes and/or update the commit of this submodule in git.")
                return()
            endif()

            message(STATUS "Submodule ${SOURCE_DIR_REL_SUBMODULE_PATH} is on the wrong commit (should be ${SUBMODULE_SUPERPROJECT_POINTER_HASH}, is ${SUBMODULE_CURRENT_HASH}).")
            message(STATUS "This is expected if the super-project commit changed. Updating the commit to the correct one.")
            execute_process(
                COMMAND ${GIT_EXECUTABLE} submodule update ${SUBMODULE_PATH}
                COMMAND_ERROR_IS_FATAL ANY
                WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
            )
        endif()
    endif()

    # Now process shallow vs deepness.
    # Is the submodule currently shallow?
    # This command can tell us -- it conveniently returns "true" or "false"
    # https://stackoverflow.com/a/37533086
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --is-shallow-repository
        COMMAND_ERROR_IS_FATAL ANY
        OUTPUT_VARIABLE SUBMODULE_IS_SHALLOW
        WORKING_DIRECTORY ${FULL_SUBMODULE_PATH}
    )

    # Remove ending newline
    string(STRIP "${SUBMODULE_IS_SHALLOW}" SUBMODULE_IS_SHALLOW)

    # If the submodule is shallow and we don't want it to be, unshallow it.
    # Note that if it is NOT shallow and should be, we do nothing. This is likely because the user or another
    # CMake build configuration already unshallowed it.
    if(SUBMODULE_IS_SHALLOW AND NOT MBED_USE_SHALLOW_SUBMODULES)
        # Now unshallow the repo
        message(STATUS "Submodule ${SOURCE_DIR_REL_SUBMODULE_PATH} will now be unshallowed.")

        # Commands from here: https://stackoverflow.com/a/17937889/7083698
        execute_process(
            COMMAND ${GIT_EXECUTABLE} fetch --unshallow
            COMMAND ${GIT_EXECUTABLE} config remote.origin.fetch "+refs/heads/*:refs/remotes/origin/*"
            COMMAND ${GIT_EXECUTABLE} fetch origin
            COMMAND_ERROR_IS_FATAL ANY
            COMMAND_ECHO STDOUT
            OUTPUT_VARIABLE SUBMODULE_IS_SHALLOW
            WORKING_DIRECTORY ${FULL_SUBMODULE_PATH}
        )
    endif()

endfunction(mbed_setup_submodule)