#!/bin/bash

. /etc/os-release
OS=$ID

if [ $OS == "ubuntu" ]; then
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
elif [ $OS == "debian" ]; then
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
        xz-utils \
		libssl-dev \
		libncurses-dev \
		flex \
		libelf-dev \
		bison

elif [ $OS == "arch" ]; then
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
		ncurses-dev xz-utils \
		libssl-dev \
		libncurses-dev \
		flex \
		libelf-dev \
		bison

fi