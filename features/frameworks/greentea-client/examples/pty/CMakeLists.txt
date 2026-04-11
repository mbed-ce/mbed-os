# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# PTY (pseudo-terminals) are available on most UNIX-like
# systems but for simplicity we only check macOS and Linux.
# Windows has an entirely different API for terminals which this
# demo does not support yet.
set(SUPPORTED_SYSTEMS Darwin Linux)
if(NOT CMAKE_SYSTEM_NAME IN_LIST SUPPORTED_SYSTEMS)
    message(WARNING "Skipping the PTY example which supports macOS and Linux only.")
else()
    add_executable(greentea-client-example-pty main.cpp)
    target_link_libraries(greentea-client-example-pty
            greentea::client
            util
    )
endif()
