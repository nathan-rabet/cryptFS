################################
### VIRTUAL MACHINE MAKEFILE ###
################################

PROJECT_DIR ?= ..

include $(PROJECT_DIR)/global.mk

# Launch virtual machine (qemu)
vm: $(BUILD_SYS)/$(SYSTEM_IMAGE_DIR)/$(SYSTEM_IMAGE) $(KERNEL_BUILD_MODULE)/$(MODULE_FILENAME) $(BUILD_KERNEL)/$(KERNEL_BUILD_COMPILED)
	@echo $(call greentext,"Running VM")
	@echo $(call greentext,"This will take a some minutes to boot...")
	@echo $(call bluetext,"You will be able to connect to the VM using SSH with the following command:")
	@echo $(call bluetext,"$$	ssh -i $(BUILD_SYS)/$(SYSTEM_IMAGE_DIR)/$(SYSTEM_VERSION).id_rsa -p 2222 -o "StrictHostKeyChecking no" root@localhost")
	@sleep 5
	mkdir -p $(BUILD_VM)/logs
	sudo virt-customize -a $(BUILD_SYS)/$(SYSTEM_IMAGE_DIR)/$(SYSTEM_IMAGE) --copy-in $(KERNEL_BUILD_MODULE)/$(MODULE_FILENAME):/root
	sudo virt-customize -a $(BUILD_SYS)/$(SYSTEM_IMAGE_DIR)/$(SYSTEM_IMAGE) --copy-in $(PROJECT_DIR)/$(DROPZONE):/root

	qemu-system-x86_64 \
	-enable-kvm \
	-smp 2 \
	-m 1024 \
	-nographic \
	-drive file=$(BUILD_SYS)/$(SYSTEM_IMAGE_DIR)/$(SYSTEM_IMAGE),format=raw \
	-append "console=ttyS0 root=/dev/sda earlyprintk=serial net.ifnames=0 nokaslr" \
	-nic user,hostfwd=tcp::2222-:22 \
	-kernel $(BUILD_KERNEL)/$(KERNEL_BUILD_COMPILED) \
	-pidfile $(BUILD_VM)/logs/vm.pid  | tee $(BUILD_VM)/logs/$(shell date +%Y-%m-%d_%H-%M-%S)_vm.log

$(BUILD_SYS)/$(SYSTEM_IMAGE_DIR)/$(SYSTEM_IMAGE):
	$(MAKE) -C $(SRC_SYS) image

$(KERNEL_BUILD_MODULE)/$(MODULE_FILENAME):
	$(MAKE) -C $(SRC_SYS) module

$(BUILD_KERNEL)/$(KERNEL_BUILD_COMPILED):
	$(MAKE) -C $(SRC_SYS) kernel

clean:
	@echo $(call bluetext,"Cleaning VM...")
	rm -rf $(BUILD_VM)
	@echo $(call greentext,"VM cleaned")

.PHONY: vm clean-vm clean-all
