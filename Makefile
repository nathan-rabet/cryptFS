################################
### VIRTUAL MACHINE MAKEFILE ###
################################

# Makefile color functions
greentext = "\e[32m$(1)\e[0m"
yellowtext = "\e[33m$(1)\e[0m"
redtext = "\e[31m$(1)\e[0m"
bluetext = "\e[34m$(1)\e[0m"

# Build configuration
PROJECT_DIR = .
PROJECT_SRC = $(PROJECT_DIR)/module

BUILD = $(PROJECT_DIR)/build
BUILD_VM = $(BUILD)/vm
MODULE_NAME = cryptolib
MODULE_FILENAME = $(MODULE_NAME).ko
BUILD_MODULE = $(BUILD)/module

IMG_URL = https://cloud-images.ubuntu.com/focal/current/focal-server-cloudimg-amd64.img
IMG_NAME = ubuntu_dev.img
IMG_NAME_TMP = $(IMG_NAME).tmp
IMG_NAME_BACKUP = $(IMG_NAME).bak
IMG_SSH = ssh.img

######################
### CALLABLE TASKS ###
######################

# Launch virtual machine (qemu)
vm: $(BUILD_VM)/$(IMG_SSH) $(BUILD_VM)/$(IMG_NAME)
	@echo $(call greentext,"Running VM")
	@echo $(call greentext,"This will take a some minutes to boot...")
	@sleep 2
	@echo $(call bluetext,"You will be able to connect to the VM using SSH with the following command:")
	@echo $(call bluetext,"$$	ssh $(shell whoami)@localhost -p 2222")
	@sleep 5
	qemu-system-x86_64 \
	-enable-kvm \
	-smp 2 \
	-m 1024 \
	-nographic \
	-hda $(BUILD_VM)/$(IMG_NAME) \
	-hdb $(BUILD_VM)/$(IMG_SSH) \
	-nic user,hostfwd=tcp::2222-:22

restore-vm-backup: $(BUILD_VM)/$(IMG_NAME_BACKUP)
	@echo $(call bluetext,"Restoring image...")
	cp $(BUILD_VM)/$(IMG_NAME_BACKUP) $(BUILD_VM)/$(IMG_NAME)
	@echo $(call greentext,"Image restored")

# Install dependencies
dependencies:
	sudo apt update
	sudo apt install -y \
		wget \
		qemu \
		qemu-system-x86 \
		libguestfs-tools \
		cloud-image-utils

	@echo $(call greentext,"Dependencies installed")

$(BUILD_MODULE)/$(MODULE_FILENAME):
	$(MAKE) -C module modules

# Ubuntu image downloading/restoring
$(BUILD_VM)/$(IMG_NAME): $(BUILD_MODULE)/$(MODULE_FILENAME)
ifeq ($(shell test -e $(BUILD_VM)/$(IMG_NAME_BACKUP);echo $$?),0)
	$(MAKE) restore-vm-backup
	mv $(BUILD_VM)/$(IMG_NAME) $(BUILD_VM)/$(IMG_NAME_TMP)
else
	mkdir -p $(BUILD_VM)
	@echo $(call bluetext,"Downloading Ubuntu image...")
	wget $(IMG_URL) -O $(BUILD_VM)/$(IMG_NAME_TMP)
	@echo $(call greentext "Image $(BUILD_VM)/$(IMG_NAME_TMP) downloaded")

	@echo $(call bluetext,"Backuping $(BUILD_VM)/$(IMG_NAME_TMP)...")
	cp $(BUILD_VM)/$(IMG_NAME_TMP) $(BUILD_VM)/$(IMG_NAME_BACKUP)
	@echo $(call greentext,"Image $(BUILD_VM)/$(IMG_NAME_TMP) backed up")

	@echo $(call bluetext,"Use 'make restore' to restore the image")
	@sleep 3
endif
	@echo $(call bluetext,"Crafting VM \(this may take some minutes\)...")

	@echo $(call bluetext,"Copying project in the VM")
	sudo virt-customize -a $(BUILD_VM)/$(IMG_NAME_TMP) --copy-in $(PROJECT_SRC):/home
	@echo $(call greentext,"Project copied in the VM")

	@echo $(call bluetext,"Installing dependencies in the VM")
	sudo virt-customize -a $(BUILD_VM)/$(IMG_NAME_TMP) --install make,gcc
	@echo $(call greentext,"Dependencies installed in the VM")

	@echo $(call bluetext,"Compiling project in the VM")
	sudo virt-customize -a $(BUILD_VM)/$(IMG_NAME_TMP) --run-command 'cd /home/$(PROJECT_SRC) \
		&& make modules KERNELDIR=`find /lib/modules/*/build`'
	@echo $(call greentext,"Project compiled in the VM")

	@echo $(call bluetext,"Loading module in the VM")
	sudo virt-customize -a $(BUILD_VM)/$(IMG_NAME_TMP) --run-command 'cd /home/$(PROJECT_SRC) && sudo make load KERNELDIR=`find /lib/modules/*/build`' 
	@echo $(call greentext,"Module loaded in the VM")
	
	@echo $(call bluetext,"Rename $(BUILD_VM)/$(IMG_NAME_TMP) to $(BUILD_VM)/$(IMG_NAME)")
	mv $(BUILD_VM)/$(IMG_NAME_TMP) $(BUILD_VM)/$(IMG_NAME)
	@echo $(call greentext,"Image renamed to $(IMG_NAME)")

# SSH image creation
$(BUILD_VM)/$(IMG_SSH): $(BUILD_VM)/$(IMG_NAME)
	@echo $(call bluetext,"Creating SSH image...")
	cat ~/.ssh/id_rsa 2>&1 > /dev/null || ssh-keygen -f ~/.ssh/id_rsa -t rsa -N ''
	cp vm/ssh.config.template $(BUILD_VM)/ssh.config
	sed -i 's/USERNAME/$(shell whoami)/g' $(BUILD_VM)/ssh.config
	sed -i 's#PUBKEY#$(shell cat ~/.ssh/id_rsa.pub)#g' $(BUILD_VM)/ssh.config

	cloud-localds --disk-format qcow2 $(BUILD_VM)/ssh.img $(BUILD_VM)/ssh.config
	@echo $(call greentext,"SSH image created")

clean-module:
	@echo $(call bluetext,"Cleaning module...")
	rm -rf $(BUILD_MODULE)
	@echo $(call greentext,"Module cleaned")

clean-vm:
	@echo $(call bluetext,"Cleaning VM...")
	rm -rf $(BUILD_VM)
	@echo $(call greentext,"VM cleaned")

clean-all:
	@echo $(call bluetext,"Cleaning all...")
	rm -rf $(BUILD)
	@echo $(call greentext,"All cleaned")

.PHONY: dependencies vm restore-vm-backup clean-module clean-vm clean-all
