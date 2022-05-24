################################
### VIRTUAL MACHINE MAKEFILE ###
################################

PROJECT_DIR ?= ..

include $(PROJECT_DIR)/global.mk

# Launch virtual machine (qemu)
vm: $(BUILD_VM)/$(IMG_NAME) $(BUILD_VM)/$(IMG_SSH) $(BUILD_KERNEL)/$(KERNEL_COMPILED)
	@echo $(call greentext,"Running VM")
	@echo $(call greentext,"This will take a some minutes to boot...")
	@echo $(call bluetext,"You will be able to connect to the VM using SSH with the following command:")
	@echo $(call bluetext,"$$	ssh $(shell whoami)@localhost -p 2222")
	@sleep 5
	mkdir -p $(BUILD_VM)/logs

	qemu-system-x86_64 \
	-enable-kvm \
	-smp 2 \
	-m 1024 \
	-nographic \
	-hda $(BUILD_VM)/$(IMG_NAME) \
	-hdb $(BUILD_VM)/$(IMG_SSH) \
	-nic user,hostfwd=tcp::2222-:22 \
	-kernel $(BUILD_KERNEL)/$(KERNEL_COMPILED) \
	-pidfile $(BUILD_VM)/logs/vm.pid  | tee $(BUILD_VM)/logs/$(shell date +%Y-%m-%d_%H-%M-%S)_vm.log

# Compile kernel
$(BUILD_KERNEL)/$(KERNEL_COMPILED):
	$(MAKE) -C $(SRC_KERNEL) kernel

# Ubuntu image downloading/restoring
$(BUILD_VM)/$(IMG_NAME_TMP):
ifeq ($(shell test -e $(BUILD_VM)/$(IMG_NAME_BACKUP);echo $$?),0)
	cp $(BUILD_VM)/$(IMG_NAME_BACKUP) $(BUILD_VM)/$(IMG_NAME_TMP)
else
	mkdir -p $(BUILD_VM)
	@echo $(call bluetext,"Downloading Ubuntu image...")
	wget $(IMG_URL) -O $(BUILD_VM)/$(IMG_NAME_TMP)

	@echo $(call bluetext,"Backuping $(BUILD_VM)/$(IMG_NAME_TMP)...")
	cp $(BUILD_VM)/$(IMG_NAME_TMP) $(BUILD_VM)/$(IMG_NAME_BACKUP)
endif

# SSH image creation
$(BUILD_VM)/$(IMG_SSH):
	@echo $(call bluetext,"Creating SSH image...")
	cat ~/.ssh/id_rsa 2>&1 > /dev/null || ssh-keygen -f ~/.ssh/id_rsa -t rsa -N ''
	cp templates/ssh.config.template $(BUILD_VM)/ssh.config
	sed -i 's/USERNAME/$(shell whoami)/g' $(BUILD_VM)/ssh.config
	sed -i 's#PUBKEY#$(shell cat ~/.ssh/id_rsa.pub)#g' $(BUILD_VM)/ssh.config

	cloud-localds --disk-format qcow2 $(BUILD_VM)/ssh.img $(BUILD_VM)/ssh.config
	@echo $(call greentext,"SSH image created")

# Crafting the VM
$(BUILD_VM)/$(IMG_NAME): $(BUILD_VM)/$(IMG_NAME_TMP) $(BUILD_KERNEL)
	@echo $(call bluetext,"Crafting VM \(this may take some minutes\)...")

	@echo $(call bluetext,"Compiling project for the VM \($(KERNEL_VERSION)\)")
	$(MAKE) -C $(SRC_MODULE) modules BUILD_MODULE=$(KERNEL_BUILD_MODULE) KERNELDIR=$(BUILD_KERNEL)/$(KERNEL_BUILD_DIRNAME)
	
	@echo $(call bluetext,"Copying kernel modules to the VM")
	sudo virt-customize -a $(BUILD_VM)/$(IMG_NAME_TMP) --copy-in $(KERNEL_BUILD_MODULE)/$(MODULE_FILENAME):/

	@echo $(call bluetext,"Installing useful C packages on the VM")
	sudo virt-customize -a $(BUILD_VM)/$(IMG_NAME_TMP) --install make,gcc

	@echo $(call bluetext,"Renaming $(BUILD_VM)/$(IMG_NAME_TMP) to $(BUILD_VM)/$(IMG_NAME)")
	mv $(BUILD_VM)/$(IMG_NAME_TMP) $(BUILD_VM)/$(IMG_NAME)


clean-vm:
	@echo $(call bluetext,"Cleaning VM...")
	rm -rf $(shell find $(BUILD_VM) ! -name $(IMG_NAME_BACKUP) ! -wholename $(BUILD_VM))
	@echo $(call greentext,"VM cleaned")

clean-all:
	@echo $(call bluetext,"Cleaning all...")
	rm -rf $(BUILD_VM)
	@echo $(call greentext,"All cleaned")

.PHONY: vm clean-vm clean-all
