###################################
### SYSTEM COMPILATION MAKEFILE ###
###################################

PROJECT_DIR ?= ../..

include $(PROJECT_DIR)/global.mk

CREATE_IMG_SCRIPT = create-image.sh

image: $(BUILD_SYS)/$(SYSTEM_IMAGE_DIR)/$(SYSTEM_IMAGE)

kernel: $(BUILD_KERNEL)/$(KERNEL_BUILD_COMPILED)
	
$(BUILD_SYS)/$(SYSTEM_IMAGE_DIR)/$(SYSTEM_IMAGE):
	mkdir -p $(BUILD_SYS)/$(SYSTEM_IMAGE_DIR)
	cp $(CREATE_IMG_SCRIPT) $(BUILD_SYS)/$(SYSTEM_IMAGE_DIR)
	cd $(BUILD_SYS)/$(SYSTEM_IMAGE_DIR) && bash $(CREATE_IMG_SCRIPT)

$(BUILD_KERNEL)/$(KERNEL_BUILD_COMPILED):
	mkdir -p $(BUILD_KERNEL)
ifeq ($(shell test -e $(BUILD_KERNEL)/$(KERNEL_ARCHIVE_NAME);echo $$?),1)
	wget $(KERNEL_URL) -O $(BUILD_KERNEL)/$(KERNEL_VERSION).tar.xz
endif
	tar -xf $(BUILD_KERNEL)/$(KERNEL_VERSION).tar.xz -C $(BUILD_KERNEL)
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

module:
	@echo $(call bluetext,"Compiling module for $(KERNEL_VERSION)...")
	make -C $(SRC_MODULE) modules BUILD=$(shell pwd)/$(BUILD_KERNEL) KERNELDIR=$(shell pwd)/$(BUILD_KERNEL)/$(KERNEL_BUILD_DIRNAME)
	@echo $(call greentext,"Module compiled successfully")

clean-module:
	@echo $(call bluetext,"Cleaning module for $(KERNEL_VERSION)...")
	sudo rm -r $(KERNEL_BUILD_MODULE)
	@echo $(call greentext,"Module cleaned successfully")

clean-kernel:
	@echo $(call bluetext,"Cleaning kernel for $(KERNEL_VERSION)...")
	rm -r $(BUILD_KERNEL)/$(KERNEL_BUILD_DIRNAME)
	@echo $(call greentext,"Kernel cleaned successfully")

# ! FIXME : SUDO
clean-image:
	@echo $(call bluetext,"Cleaning system...")
	sudo rm -r $(BUILD_SYS)/$(SYSTEM_IMAGE_DIR)
	@echo $(call greentext,"System cleaned successfully")

clean-all:
	@echo $(call bluetext,"Cleaning all for $(KERNEL_VERSION)...")
	rm -r $(BUILD_KERNEL) $(BUILD_SYS)
	@echo $(call greentext,"All cleaned successfully")
