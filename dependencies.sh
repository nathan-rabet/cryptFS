#!/bin/bash

if [ -x "$(command -v apt)" ]; then
    sudo apt update
    sudo apt install -y \
        wget \
        qemu \
        qemu-system-x86 \
        libguestfs-tools \
        cloud-image-utils \
        git \
        gcc \
        build-essential \
        ncurses-dev \
        xz-utils \
        libssl-dev \
        libncurses-dev \
        flex \
        libelf-dev \
        bison
elif [ -x "$(command -v yum)" ]; then
    sudo pacman -Syu
    sudo pacman -S --noconfirm \
        wget \
        qemu \
        qemu-system-x86 \
        libguestfs \
        cloud-image-utils \
        git \
        gcc \
        base-devel \
        openssl \
        ncurses \
        flex \
        libelf \
        bison
else
    echo "Unsupported OS"
    exit 1
fi
