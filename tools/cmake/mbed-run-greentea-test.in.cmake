# This file is configured by CMake to create the script used to execute each Greentea test.

# First flash the target using its configured Mbed OS upload method
# -----------------------------------------------------------------------
execute_process(
	# Note: we don't use cmake --build because that performs a reconfigure of the build system each time
	COMMAND @CMAKE_MAKE_PROGRAM@ flash-@MBED_GREENTEA_TEST_NAME@
	WORKING_DIRECTORY @CMAKE_BINARY_DIR@
	COMMAND_ERROR_IS_FATAL ANY)

# Then run and communicate with the test
# -----------------------------------------------------------------------

# Before starting, do we need to acquire a lock? If so, acquire it
if(NOT "@EXCLUSIVE_GROUP_LOCK_FILE@" STREQUAL "")
	file(LOCK @EXCLUSIVE_GROUP_LOCK_FILE@ TIMEOUT 0 RESULT_VARIABLE TRY_LOCK_SUCCESSFUL)
	if(NOT TRY_LOCK_SUCCESSFUL STREQUAL "0")
		message("Another test in group @MBED_GREENTEA_EXCLUSIVE_GROUP@ is currently running, waiting...")
		file(LOCK @EXCLUSIVE_GROUP_LOCK_FILE@)
	endif()
endif()

set(MBEDHTRUN_ARGS @MBED_HTRUN_ARGUMENTS@) # filled in by configure script

# Print out command
string(REPLACE ";" " " MBEDHTRUN_ARGS_FOR_DISPLAY "${MBEDHTRUN_ARGS}")
message("Executing: @mbedhtrun@ ${MBEDHTRUN_ARGS_FOR_DISPLAY}")

execute_process(
	COMMAND @mbedhtrun@ ${MBEDHTRUN_ARGS}
	COMMAND_ERROR_IS_FATAL ANY)