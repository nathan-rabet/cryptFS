###################################
### KERNEL COMPILATION MAKEFILE ###
###################################

PROJECT_DIR ?= ..

include $(PROJECT_DIR)/global.mk

kernel: $(BUILD_KERNEL)/$(KERNEL_COMPILED)
	
$(BUILD_KERNEL)/$(KERNEL_COMPILED):
	mkdir -p $(BUILD_KERNEL)
ifeq ($(shell test -e $(BUILD_KERNEL)/$(KERNEL_ARCHIVE_NAME);echo $$?),0)
	tar -xf $(BUILD_KERNEL)/$(KERNEL_VERSION).tar.xz -C $(BUILD_KERNEL)
else
	wget $(KERNEL_URL) -O $(BUILD_KERNEL)/$(KERNEL_VERSION).tar.xz
endif
	@echo $(call bluetext,"Building kernel configuration...")
	cd $(BUILD_KERNEL)/$(KERNEL_BUILD_DIRNAME) && make defconfig
	cd $(BUILD_KERNEL)/$(KERNEL_BUILD_DIRNAME) && make kvm_guest.config

	echo "CONFIG_KCOV=y" >> $(BUILD_KERNEL)/$(KERNEL_BUILD_DIRNAME)/.config
	echo "CONFIG_DEBUG_INFO=y" >> $(BUILD_KERNEL)/$(KERNEL_BUILD_DIRNAME)/.config
	echo "CONFIG_KASAN=y" >> $(BUILD_KERNEL)/$(KERNEL_BUILD_DIRNAME)/.config
	echo "CONFIG_KASAN_INLINE=y" >> $(BUILD_KERNEL)/$(KERNEL_BUILD_DIRNAME)/.config
	
	cd $(BUILD_KERNEL)/$(KERNEL_BUILD_DIRNAME) && make olddefconfig

	@echo $(call bluetext,"Building kernel...")
	cd $(BUILD_KERNEL)/$(KERNEL_BUILD_DIRNAME) && make -j$(shell nproc)

	@echo $(call greentext,"Kernel built successfully")

module: $(BUILD_KERNEL)/$(KERNEL_COMPILED)
	@echo $(call bluetext,"Compiling module for $(KERNEL_VERSION)...")
	cd $(MODULE) && make modules BUILD=$(BUILD_KERNEL)/module KERNELDIR=$(BUILD_KERNEL)/$(KERNEL_BUILD_DIRNAME)
	@echo $(call greentext,"Module compiled successfully")

