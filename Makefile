########################
### WRAPPER MAKEFILE ###
########################

PROJECT_DIR ?= $(shell pwd)

include $(PROJECT_DIR)/global.mk

module:
	$(MAKE) -C $(SRC_MODULE) modules
	cp $(BUILD_KERNEL)/$(KERNEL_COMPILED) ./$(MODULE_FILENAME)

vm:
	$(MAKE) -C $(SRC_VM) vm

kernel:
	$(MAKE) -C $(SRC_KERNEL) kernel

kernel-module:
	$(MAKE) -C $(SRC_MODULE) kernel-module

dependencies:
	bash dependencies.sh

clean-all:
	rm -rf $(BUILD) $(MODULE_FILENAME)

clean-vm:
	$(MAKE) -C $(SRC_VM) clean-vm

clean-vm-all:
	$(MAKE) -C $(SRC_VM) clean-all

clean-module:
	rm -rf $(MODULE_FILENAME)
	$(MAKE) -C $(SRC_MODULE) clean

clean-kernel-module:
	$(MAKE) -C $(SRC_KERNEL) clean-module

clean-kernel-kernel:
	$(MAKE) -C $(SRC_KERNEL) clean-kernel

clean-kernel-all:
	$(MAKE) -C $(SRC_KERNEL) clean-all

.PHONY: module vm kernel kernel-module clean-all clean-vm clean-vm-all clean-module clean-kernel-module clean-kernel-kernel clean-kernel-all
