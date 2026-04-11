# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.10)

project(imported-targets
    VERSION 0.1
    DESCRIPTION "Tests greentea-client's exported targets can be imported"
    LANGUAGES C CXX
)

find_package(greentea REQUIRED)

add_executable(imported-targets main.cpp)
target_link_libraries(imported-targets greentea::client)
