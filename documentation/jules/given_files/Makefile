obj-m += epidriver.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

test:
	@-sudo rmmod driver
	@sudo dmesg -C
	@sudo insmod driver.ko
	@sudo dmesg

nod:
	@echo sudo mknod /dev/epidriver c major 0
