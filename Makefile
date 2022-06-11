########################
### WRAPPER MAKEFILE ###
########################

PROJECT_DIR ?= $(shell pwd)

include $(PROJECT_DIR)/global.mk

module:
	$(MAKE) -C $(SRC_MODULE) modules
	cp $(BUILD_KERNEL)/$(KERNEL_BUILD_COMPILED) ./$(MODULE_FILENAME)

dependencies:
	bash dependencies.sh

vm:
	$(MAKE) -C $(SRC_VM) vm

kernel:
	$(MAKE) -C $(SRC_SYS) kernel

kernel-module:
	$(MAKE) -C $(SRC_SYS) module

image:
	$(MAKE) -C $(SRC_SYS) image

clean-module:
	rm -rf $(MODULE_FILENAME)
	$(MAKE) -C $(SRC_MODULE) clean

clean-vm:
	$(MAKE) -C $(SRC_VM) clean

clean-sytem-kernel-module:
	$(MAKE) -C $(SRC_SYS) clean-module

clean-system-image:
	$(MAKE) -C $(SRC_SYS) clean-image

clean-system-all:
	$(MAKE) -C $(SRC_SYS) clean-all

clean-all-yes-i-am-sure:
	@echo $(call yellowtext,"Do you REALLY want to delete the $(BUILD) directory?")
	@echo $(call yellowtext,"Press [Enter] to continue or Ctrl+C to abort.")
	@read ack
	rm -rf $(BUILD) $(MODULE_FILENAME)


.PHONY: module vm kernel kernel-module image dependencies clean-all clean-vm clean-vm-all clean-module clean-sytem-kernel-module clean-system-kernel clean-system-image clean-system-all
