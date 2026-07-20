#!/bin/bash -e
#
# Copyright (c) 2026 Jamie Smith
# SPDX-License-Identifier: Apache-2.0
#

# Script to upload a PlatformIO release of Mbed.
# Note that you need to `pio account login` before running this script.
# Use `pio pkg show mbed-ce-project/framework-mbed-ce` to see published versions.
# (old versions may need to be unpublished to avoid hitting the 500MB limit)
# Also note that if you have CLion open on the mbed-os dir, you will need to close it before running this script,
# as otherwise it may keep trying to recreate the build dirs that we need to delete.

# Go to mbed-os root dir
cd "$(dirname "$0")/.."

# Clone all submodules, as we want them all to be included in the PIO archive
git submodule update --init --recursive

# Remove all build dirs as we don't want them included in the archive
echo "Removing all build dirs..."
rm -rf cmake-build-*
rm -rf build

# Remove venv dir as we don't want it included in the archive
echo "Removing venv dir..."
rm -rf venv

# Update the version number
version_number=$(cat VERSION)
sed "s/VERSION_NUMBER_GOES_HERE/$version_number/" tools/pio_package.json.in > package.json

# Publish the package
pio pkg publish --owner mbed-ce-project .