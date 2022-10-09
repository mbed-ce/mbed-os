# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

mbed_add_library(mbed-greentea-io EXCLUDE_FROM_ALL)
target_sources(mbed-greentea-io PRIVATE mbed_io.cpp)
target_link_libraries(mbed-greentea-io PUBLIC mbed-core-flags)
