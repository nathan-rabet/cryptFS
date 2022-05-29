#######################
### MODULE MAKEFILE ###
#######################

greentext = "\e[32m$(1)\e[0m"
yellowtext = "\e[33m$(1)\e[0m"
redtext = "\e[31m$(1)\e[0m"
bluetext = "\e[34m$(1)\e[0m"

BUILD = ../build
BUILD_MODULE = $(BUILD)/module
MODULE_NAME = cryptolib
MODULE_FILENAME = $(MODULE_NAME).ko
PWD := $(shell pwd)

ifeq ($(KERNELRELEASE),)

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
COMPILATION_FILES = *.o *~ core .depend .*.cmd *.ko *.mod.c *.tmp_versions *.mod *.order *.symvers


modules: $(BUILD_MODULE)
	sudo $(MAKE) -C $(KERNELDIR) M=$(PWD) $@
	mv $(PWD)/$(COMPILATION_FILES) $(BUILD_MODULE) 2>/dev/null; true

modules_install: $(BUILD_MODULE)
	sudo $(MAKE) -C $(KERNELDIR) M=$(PWD) $@
	mv $(PWD)/$(COMPILATION_FILES) $(BUILD_MODULE) 2>/dev/null; true

$(BUILD_MODULE):
	mkdir -p $(BUILD_MODULE)

$(BUILD_MODULE)/$(MODULE_FILENAME): $(BUILD_MODULE)
	$(MAKE) modules

load: $(BUILD_MODULE)/$(MODULE_FILENAME)
	@echo $(call yellowtext,"Do you REALLY want to load the module [$(MODULE_NAME)] on your system?")
	@echo $(call yellowtext,"It may cause some trouble.")
	@echo $(call yellowtext,"You have 5 seconds to presse Ctrl+C if you DO NOT want to do this")
	@sleep 5

	@echo $(call bluetext,"Loading kernel module")
	sudo insmod $(BUILD_MODULE)/$(MODULE_FILENAME)
	@echo $(call greentext,"Module loaded successfully")

unload:
	@echo $(call yellowtext,"Do you REALLY want to unload the module [$(MODULE_NAME)] from your system?")
	@echo $(call yellowtext,"If you DO NOT want to do this: press Ctrl+C now.")
	@echo $(call yellowtext,"You have 3 seconds to presse Ctrl+C if you DO NOT want to do this")
	@sleep 3

	@echo $(call bluetext,"Unloading kernel module")
	sudo rmmod $(BUILD_MODULE)/$(MODULE_FILENAME)
	@echo $(call greentext,"Module unloaded successfully")

clean:
	$(RM) -rf $(COMPILATION_FILES) $(BUILD_MODULE)

.PHONY: modules modules_install clean

else
SRC = $(shell find $(PWD)/src -name '*.c')
OBJS = $(SRC:.c=.o)

obj-m := $(MODULE_NAME).o
$(MODULE_NAME)-objs = $(OBJS)

endif
