# ------------------------------------------------------------------------------
# Pull base image
FROM ubuntu:24.04

# ------------------------------------------------------------------------------
# set locale
ENV LANG=C.UTF-8
ENV LC_ALL=C.UTF-8

# ------------------------------------------------------------------------------
# Arguments
ARG WORKDIR=/root

# ------------------------------------------------------------------------------
# Install tools via apt
ENV DEBIAN_FRONTEND=noninteractive
RUN set -x \
    && apt -y update \
    && apt -y install \
    git \
    wget \
    xz-utils \
    python3 \
    python3-dev \
    python3-setuptools \
    python3-wheel \
    python3-pip \
    python3-venv \
    python-is-python3 \
    build-essential \
    astyle \
    mercurial \
    cmake \
    ninja-build \
    libssl-dev \
    cargo \
    flex \
    bison \
    doxygen \
    aspell \
    ccache \
    gcovr \
    && apt clean && rm -rf /var/lib/apt/lists \
    && : # last line

# Disable Git safe directory checking, as it tends to cause a lot of issues on Github CI runners
RUN git config --global --add safe.directory '*'

# ------------------------------------------------------------------------------
# Install arm-none-eabi-gcc
WORKDIR /opt/mbed-os-toolchain
RUN set -x \
    && if [ "$(uname -m)" = "aarch64" ]; then \
        HOST_ARCH=aarch64; \
        TARBALL_SHA256="2d465847eb1d05f876270494f51034de9ace9abe87a4222d079f3360240184d3"; \
    else \
        HOST_ARCH=x86_64; \
        TARBALL_SHA256="8f6903f8ceb084d9227b9ef991490413014d991874a1e34074443c2a72b14dbd"; \
    fi \
    && TARBALL="arm-gnu-toolchain-14.3.rel1-${HOST_ARCH}-arm-none-eabi.tar.xz" \
    && wget -q "https://gitlab.arm.com/api/v4/projects/tooling%2Fgnu-toolchains-for-arm/packages/generic/gnu-toolchain/14.3.rel1/${TARBALL}" \
    && echo "${TARBALL_SHA256}  ${TARBALL}" | sha256sum -c - \
    && mkdir gcc-arm-none-eabi \
    && tar -xJf "${TARBALL}" --strip-components=1 -C gcc-arm-none-eabi \
    && rm "${TARBALL}" \
    && : # last line

# ------------------------------------------------------------------------------
# Configure environment variables to put GCC and pipx-installed stuff on PATH
ENV MBED_GCC_ARM_PATH=/opt/mbed-os-toolchain/gcc-arm-none-eabi/bin/
ENV PATH="${PATH}:${MBED_GCC_ARM_PATH}:/root/.local/bin"

# ------------------------------------------------------------------------------
# Display, check and save environment settings
# NOTE: using bash instead of Ubuntu default bash due to unsupport for pipefail
# Pipefail is crucial here, if the tools didn't install properly, docker build should not pass because of piping 
RUN /bin/bash -c \
   "set -x -o pipefail \
    && arm-none-eabi-gcc --version | grep arm-none-eabi-gcc | tee env_settings \
    && cmake --version | grep version | tee -a env_settings \
    && ninja --version | tee -a env_settings \
    && python --version 2>&1 | tee -a env_settings \
    && python3 -c 'import venv' \
    && : # LAST LINE"

WORKDIR /root
