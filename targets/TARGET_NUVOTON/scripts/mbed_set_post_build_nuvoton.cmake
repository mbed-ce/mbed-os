# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_target_functions)

#
# Sign TF-M secure and non-secure images and combine them with the bootloader
#
macro(mbed_post_build_nuvoton_tfm_sign_image
    nuvoton_target
    tfm_import_path
    signing_key
    signing_key_1
)
    if("${nuvoton_target}" STREQUAL "${MBED_TARGET}")
        function(mbed_post_build_function target)
            find_package(Python3)

            if("${MBED_OUTPUT_EXT}" STREQUAL "")
                # If both bin and hex are being generated, just pick one.
                set(EXT hex)
            else()
                set(EXT ${MBED_OUTPUT_EXT})
            endif()

            # NOTE: Macro arguments are not variables and cannot pass to if(<condition>).
            set(signing_key_1_ ${signing_key_1})
            if(signing_key_1_)
                add_custom_command(
                    TARGET
                        ${target}
                    POST_BUILD
                    COMMAND
                        ${Python3_EXECUTABLE}
                        ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/NUVOTON.py
                        tfm_sign_image
                        --tfm-import-path ${tfm_import_path}
                        --signing_key ${signing_key}
                        --signing_key_1 ${signing_key_1}
                        --non-secure-binhex $<TARGET_FILE_DIR:${target}>/$<TARGET_FILE_BASE_NAME:${target}>.${EXT}
                )
            else()
                add_custom_command(
                    TARGET
                        ${target}
                    POST_BUILD
                    COMMAND
                        ${Python3_EXECUTABLE}
                        ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/NUVOTON.py
                        tfm_sign_image
                        --tfm-import-path ${tfm_import_path}
                        --signing_key ${signing_key}
                        --non-secure-binhex $<TARGET_FILE_DIR:${target}>/$<TARGET_FILE_BASE_NAME:${target}>.${EXT}
                )
            endif()
        endfunction()
    endif()
endmacro()
