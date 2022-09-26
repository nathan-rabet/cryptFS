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

BUILD_DIR = $(PROJECT_DIR)/build
TESTS_DIR = $(PROJECT_DIR)/tests/criterion

SRC_DIR = $(PROJECT_DIR)/src
FS_CORE_DIR = $(SRC_DIR)/fs
FUSE_CORE_DIR = $(SRC_DIR)/fuse

