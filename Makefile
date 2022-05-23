# Makefile color functions
greentext = "\e[32m$(1)\e[0m"
yellowtext = "\e[33m$(1)\e[0m"
redtext = "\e[31m$(1)\e[0m"
bluetext = "\e[34m$(1)\e[0m"

# Compiler configuration
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic -Iinclude
BUILD = build
BUILD_VM = $(BUILD)/vm

# Source files to compile
SRC = $(shell find src -name '*.c')
OBJS = $(SRC:.c=.o)

# Kernel module configuration
PWD = $(shell pwd)

IMG_NAME = ubuntu_dev.img
IMG_NAME_BACKUP = $(IMG_NAME).bak
IMG_SSH = ssh.img
DIR_VERSION =  linux-5.13.1
all: virtual-machine
	
# Ubuntu image downloading/restoring
$(BUILD_VM)/$(IMG_NAME):

# Check if the backup exists
ifeq ($(shell test -e $(BUILD_VM)/$(IMG_NAME_BACKUP);echo $$?),0)
	$(MAKE) restore
else
	mkdir -p $(BUILD_VM)
	@echo $(call bluetext,"Downloading Ubuntu image...")
	wget https://cloud-images.ubuntu.com/focal/current/focal-server-cloudimg-amd64.img -O $(BUILD_VM)/$(IMG_NAME)
	@echo $(call greentext "Image $(BUILD_VM)/$(IMG_NAME) downloaded")

	@echo $(call bluetext,"Backuping $(BUILD_VM)/$(IMG_NAME)...")
	cp $(BUILD_VM)/$(IMG_NAME) $(BUILD_VM)/$(IMG_NAME_BACKUP)
	@echo $(call greentext,"Image $(BUILD_VM)/$(IMG_NAME) backed up")
	@echo $(call bluetext,"\nUse 'make restore' to restore the image")
	@sleep 3
endif

# SSH image creation
$(BUILD_VM)/$(IMG_SSH): $(BUILD_VM)/$(IMG_NAME)
	@echo $(call bluetext,"Creating SSH image...")
	cat ~/.ssh/id_rsa > /dev/null 2>&1 || ssh-keygen -f ~/.ssh/id_rsa -t rsa -N ''
	cp vm/ssh.config.template $(BUILD_VM)/ssh.config
	sed -i 's/USERNAME/$(shell whoami)/g' $(BUILD_VM)/ssh.config
	sed -i 's#PUBKEY#$(shell cat ~/.ssh/id_rsa.pub)#g' $(BUILD_VM)/ssh.config

	cloud-localds --disk-format qcow2 $(BUILD_VM)/ssh.img $(BUILD_VM)/ssh.config
	@echo $(call greentext,"SSH image created")

# Install dependencies
dependencies:
	./package.sh
	wget https://cdn.kernel.org/pub/linux/kernel/v5.x/$(DIR_VERSION).tar.xz
	tar xvf  $(DIR_VERSION).tar.xz
	cd $(DIR_VERSION) && make defconfig
	cd $(DIR_VERSION) && make kvm_guest.config
	
	@echo "CONFIG_KCOV=y" >> $(DIR_VERSION)/.config
	@echo "CONFIG_DEBUG_INFO=y" >> $(DIR_VERSION)/.config
	@echo "CONFIG_KASAN=y" >> $(DIR_VERSION)/.config
	@echo "CONFIG_KASAN_INLINE=y" >> $(DIR_VERSION)/.config
	cd $(DIR_VERSION) && make olddefconfig
	cd $(DIR_VERSION) && make -j`nproc`

	@echo $(call greentext, "Dependencies installed")

# Launch virtual machine (qemu)
virtual-machine: $(BUILD_VM)/$(IMG_SSH) $(BUILD_VM)/$(IMG_NAME)
	@echo $(call greentext,"Running VM")
	@echo $(call greentext,"This will take a some minutes to boot...")
	@sleep 2
	@echo $(call bluetext,"You will be able to connect to the VM using SSH with the following command:")
	@echo $(call bluetext,"	ssh $(shell whoami)@localhost -p 2222")
	@sleep 5
	qemu-system-x86_64 \
	-enable-kvm \
	-smp 2 \
	-m 1024 \
	-nographic \
	-hda $(BUILD_VM)/$(IMG_NAME) \
	-hdb $(BUILD_VM)/$(IMG_SSH) \
	-nic user,hostfwd=tcp::2222-:22 \
	-kernel $KERNEL/arch/x86/boot/bzImage \
	-pidfile vm.pid
	2>&1 | tee vm.log

# Restore non-modified image (in case of existing backup)
restore: $(BUILD_VM)/$(IMG_NAME_BACKUP)
	@echo $(call bluetext,"Restoring image...")
	cp $(BUILD_VM)/$(IMG_NAME_BACKUP) $(BUILD_VM)/$(IMG_NAME)
	@echo $(call greentext,"Image restored")
