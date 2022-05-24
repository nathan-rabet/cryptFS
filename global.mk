######################################################
###            GLOBAL MAKEFILES SETTINGS           ###
###                MUST BE INCLUDED                ###
###     AFTER THE DEFINITION OF $(PROJECT_DIR)     ###
######################################################

greentext = "\e[32m$(1)\e[0m"
yellowtext = "\e[33m$(1)\e[0m"
redtext = "\e[31m$(1)\e[0m"
bluetext = "\e[34m$(1)\e[0m"

PROJECT_DIR ?= $(shell false)

BUILD = $(PROJECT_DIR)/build

SRC_VM = $(PROJECT_DIR)/vm
BUILD_VM = $(BUILD)/vm
    IMG_URL = https://cloud-images.ubuntu.com/focal/current/focal-server-cloudimg-amd64.img
    IMG_NAME = ubuntu_dev.img
    IMG_NAME_TMP = $(IMG_NAME).tmp
    IMG_NAME_BACKUP = $(IMG_NAME).bak
    IMG_SSH = ssh.img

SRC_KERNEL = $(PROJECT_DIR)/kernel
BUILD_KERNEL = $(BUILD)/kernel
    KERNEL_VERSION = linux-5.13.1
    KERNEL_BUILD_DIRNAME = $(KERNEL_VERSION)
    KERNEL_ARCHIVE_NAME = $(KERNEL_VERSION).tar.xz
    KERNEL_COMPILED = $(KERNEL_ARCHIVE_NAME)/arch/x86/boot/bzImage
    KERNEL_URL = https://cdn.kernel.org/pub/linux/kernel/v5.x/$(KERNEL_ARCHIVE_NAME)
    KERNEL_BUILD_MODULE = $(BUILD_KERNEL)/module
    NEW_KERNELDIR = $(BUILD_KERNEL)/$(KERNEL_BUILD_DIRNAME)/build 

SRC_MODULE = $(PROJECT_DIR)/module
BUILD_MODULE = $(BUILD)/module
    MODULE_NAME = cryptolib
    MODULE_FILENAME = $(MODULE_NAME).ko
