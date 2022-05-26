########################
### WRAPPER MAKEFILE ###
########################

PROJECT_DIR ?= $(shell pwd)

include $(PROJECT_DIR)/global.mk

module:
	$(MAKE) -C $(SRC_MODULE) modules
	cp $(BUILD_KERNEL)/$(KERNEL_COMPILED) ./$(MODULE_FILENAME)

dependencies:
	bash dependencies.sh

vm:
	$(MAKE) -C $(SRC_VM) vm

kernel:
	$(MAKE) -C $(SRC_KERNEL) kernel

kernel-module:
	$(MAKE) -C $(SRC_MODULE) kernel-module

image:
	$(MAKE) -C $(SRC_SYS) image

clean-all:
	rm -rf $(BUILD) $(MODULE_FILENAME)

clean-vm:
	$(MAKE) -C $(SRC_VM) clean-vm

clean-vm-all:
	$(MAKE) -C $(SRC_VM) clean-all

clean-module:
	rm -rf $(MODULE_FILENAME)
	$(MAKE) -C $(SRC_MODULE) clean

clean-sytem-kernel-module:
	$(MAKE) -C $(SRC_SYS) clean-module

clean-system-kernel:
	$(MAKE) -C $(SRC_SYS) clean-kernel

clean-system-image:
	$(MAKE) -C $(SRC_SYS) clean-image

clean-system-all:
	$(MAKE) -C $(SRC_KERNEL) clean-all

.PHONY: module vm kernel kernel-module image dependencies clean-all clean-vm clean-vm-all clean-module clean-sytem-kernel-module clean-system-kernel clean-system-image clean-system-all
