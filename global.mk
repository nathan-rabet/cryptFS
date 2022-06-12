######################################################
###            GLOBAL MAKEFILES SETTINGS           ###
###                MUST BE INCLUDED                ###
###     AFTER THE DEFINITION OF $(PROJECT_DIR)     ###
######################################################

greentext = "\e[32m$(1)\e[0m"
yellowtext = "\e[33m$(1)\e[0m"
redtext = "\e[31m$(1)\e[0m"
bluetext = "\e[34m$(1)\e[0m"

PROJECT_DIR ?= $(shell false) # MUST BE SET BEFORE INCLUDING THIS FILE
DROPZONE = dropzone

BUILD = $(PROJECT_DIR)/build

SRC_CODE = $(PROJECT_DIR)/code

BUILD_VM = $(BUILD)/vm
SRC_VM = $(PROJECT_DIR)/vm

SRC_SYS = $(SRC_VM)/system
BUILD_SYS = $(BUILD)/system
    SYSTEM_IMAGE_DIR = image
    SYSTEM_VERSION = stretch
    SYSTEM_IMAGE = $(SYSTEM_VERSION).img

    BUILD_KERNEL = $(BUILD_SYS)/kernel
        KERNEL_VERSION = linux-5.13.1
        KERNEL_BUILD_DIRNAME = $(KERNEL_VERSION)
        KERNEL_ARCHIVE_NAME = $(KERNEL_VERSION).tar.xz
        KERNEL_BUILD_COMPILED = $(KERNEL_BUILD_DIRNAME)/arch/x86/boot/bzImage
        KERNEL_URL = https://cdn.kernel.org/pub/linux/kernel/v5.x/$(KERNEL_ARCHIVE_NAME)
        KERNEL_BUILD_MODULE = $(BUILD_KERNEL)/module
        

SRC_MODULE = $(SRC_CODE)/module
BUILD_MODULE = $(BUILD)/module
    MODULE_NAME = cryptolib
    MODULE_FILENAME = $(MODULE_NAME).ko

SRC_FS = $(SRC_CODE)/fs
BUILD_FS = $(BUILD)/fs
