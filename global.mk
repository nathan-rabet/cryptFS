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

BUILD = $(PROJECT_DIR)/build

SRC_CODE = $(PROJECT_DIR)/code
FUSE_CODE = $(SRC_CODE)/fuse
