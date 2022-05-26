#!/bin/bash
if [ -x "$(command -v apt)" ]; then
    sudo apt update
    sudo apt install -y \
        wget \
        qemu \
        qemu-system-x86 \
        libguestfs-tools \
        cloud-image-utils \
        debootstrap \
        gcc \
        flex bison libncurses-dev libelf-dev libssl-dev
elif [ -x "$(command -v yum)" ]; then
    sudo pacman -Syu
    sudo pacman -S --noconfirm \
        wget \
        qemu \
        qemu-system-x86 \
        libguestfs \
        cloud-image-utils \
        debootstrap \
        gcc \
        flex bison libncurses-dev libelf-dev libssl-dev
else
    echo "Unsupported OS"
    exit 1
fi
