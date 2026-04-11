# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(greentea-fake-console-io ./fake_console_io.cpp)
target_compile_features(greentea-fake-console-io PUBLIC cxx_std_14)
target_include_directories(greentea-fake-console-io PUBLIC .)
target_link_libraries(greentea-fake-console-io PRIVATE greentea::client_userio)
