# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(GoogleTest)

add_executable(greentea-tests test_kv_protocol.cpp)
target_compile_features(greentea-tests PUBLIC cxx_std_14)
target_link_libraries(greentea-tests PUBLIC greentea::client_userio greentea-fake-console-io gtest_main)
gtest_discover_tests(greentea-tests PROPERTIES LABELS "greentea-client")