# Mbed CE development environment Docker image

This Docker image is the official Mbed CE development environment.

- It is based on Ubuntu 24.04
- Python, CMake, and Ninja are installed
- Arm-none-eabi-gcc toolchain is installed
- All other Mbed CE dependency tools are installed.

# How to use the Docker image:

## Pull the Docker image

```bash
docker pull ghcr.io/mbed-ce/mbed-ce-env:<label>
```

The following image tags are published:

- `main`: Latest successful build of `main`
- `main-YYYYMMDD-<sha7>`: Snapshot of a particular `main` build
- `mbed-ce-X.Y.Z`: Release image for a specific mbed-ce version

Note: release Git tags may still use the `mbed-os-X.Y.Z` naming scheme, even though the published container images use `mbed-ce-X.Y.Z`.

## Run Mbed CE environment without HW support (build Mbed images only)

Launch the Docker image by

```bash
docker run -it ghcr.io/mbed-ce/mbed-ce-env:<label>
```

Then you will have a container with an Mbed CE development environment.
For instructions on creating a project and building Mbed CE applications, see the [Mbed CE documentation](https://mbed-ce.dev/).

## Run Mbed CE environment with HW support (USB pass-through)

If you want to use this Docker image to connect and flash your targets, you will need some extra command line options to pass-through your USB devices (NOTE: USB pass-through with these `/dev` mounts only works for Linux hosts):

```bash
sudo docker run -it --privileged -v /dev/disk/by-id:/dev/disk/by-id -v /dev/serial/by-id:/dev/serial/by-id ghcr.io/mbed-ce/mbed-ce-env:<label>
```

Then you will have a container with an Mbed CE development environment.
