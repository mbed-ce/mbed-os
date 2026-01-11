#!/bin/bash -e

# Script to upload a PlatformIO release of Mbed.
# Note that you need to `pio account login` before running this script.
# Use `pio pkg show mbed-ce-project/framework-mbed-ce` to see published versions.
# (old versions may need to be unpublished to avoid hitting the 500MB limit)

# Go to mbed-os root dir
cd "$(dirname "$0")/.."

# Clone all submodules, as we want them all to be included in the PIO archive
git submodule update --init --recursive

# Remove all build dirs as we don't want them included in the archive
echo "Removing all build dirs..."
rm -rf cmake-build-*
rm -rf build

# Update the version number
version_number=$(cat VERSION)
sed "s/VERSION_NUMBER_GOES_HERE/$version_number/" tools/pio_package.json.in > package.json

# Publish the package
pio pkg publish --owner mbed-ce-project .