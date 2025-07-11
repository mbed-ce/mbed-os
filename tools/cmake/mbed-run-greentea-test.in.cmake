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
set(MBEDHTRUN_ARGS --skip-flashing @MBED_HTRUN_ARGUMENTS@) # filled in by configure script

# Print out command
string(REPLACE ";" " " MBEDHTRUN_ARGS_FOR_DISPLAY "${MBEDHTRUN_ARGS}")
message("Executing: @mbedhtrun@ ${MBEDHTRUN_ARGS_FOR_DISPLAY}")

execute_process(
	COMMAND @mbedhtrun@ ${MBEDHTRUN_ARGS}
	COMMAND_ERROR_IS_FATAL ANY)