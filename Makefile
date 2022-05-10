
# Compiler configuration
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic -Iinclude
BUILD = build

# Source files to compile
SRC = $(shell find src -name '*.c')
OBJS = $(SRC:.c=.o)

# Kernel module configuration
PWD = $(shell pwd)

QCOW_IMG_NAME = ubuntu_dev.qcow2

VM_CONF_FILE = vm.conf
VM_CONF_IMG_NAME = $(VM_CONF_FILE).img

VM_PASS = ubuntu

all: virtual-machine
	
# Build directories

# Linux image creation
$(BUILD)/$(QCOW_IMG_NAME):
	mkdir -p $(BUILD) && \
	cd $(BUILD) && \
	wget -nc https://cloud-images.ubuntu.com/focal/current/focal-server-cloudimg-amd64.img && \
	sudo virt-customize -a focal-server-cloudimg-amd64.img --root-password password:$(VM_PASS) && \
	qemu-img create -f qcow2 -b focal-server-cloudimg-amd64.img $(QCOW_IMG_NAME) 5G
	@echo "\e[32mUbuntu image successfully created\e[0m"

dependencies:
	sudo apt update && sudo apt install -y \
		gcc \
		wget \
		bison \
		build-essential \
		flex \
		git \
		libelf-dev \
		libssl-dev \
		ncurses-dev \
		qemu \
		qemu-system-x86 \
		cloud-image-utils \
		libguestfs-tools

	@echo "\e[32mDependencies installed\e[0m"

modules: kernel
	@echo "\e[32mModules compiled\e[0m"

kernel: dependencies $(BUILD)/$(QCOW_IMG_NAME)
	@echo "\e[32mKernel compiled\e[0m"


virtual-machine: kernel
	@echo "\e[32mRuinning QEMU (x86_64)\e[0m"
	qemu-system-x86_64 \
    -enable-kvm \
    -m 2048 \
    -drive file=$(BUILD)/$(QCOW_IMG_NAME),media=disk,if=virtio
	-drive file=$(VM_CONF_IMG_NAME),format=raw