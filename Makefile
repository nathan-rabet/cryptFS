########################
### WRAPPER MAKEFILE ###
########################

PROJECT_DIR ?= $(shell pwd)

include $(PROJECT_DIR)/global.mk

format:
	@$(MAKE) -C $(PROJECT_DIR)/src format


.PHONY: module vm kernel kernel-module image dependencies clean-all clean-vm clean-vm-all clean-module clean-sytem-kernel-module clean-system-kernel clean-system-image clean-system-all
