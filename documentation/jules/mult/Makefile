KERNELDIR = /lib/modules/$(shell uname -r)/build
PWD = $(shell pwd)
target = test 
obj-m := test.o
test-objs := kernel_test.o second.o

modules: $(OBJS)
	$(MAKE) -C $(KERNELDIR) M=$(PWD) $@ $^

modules_install: $(OBJS)
	$(MAKE) -C $(KERNELDIR) M=$(PWD) $@ $^

clean:
	$(RM) *.o *~ core .depend .*.cmd *.ko *.mod.c *.tmp_versions *.mod *.order *.symvers

load:
	@sudo insmod ./$(target).ko

unload:
	@sudo rmmod $(target)

.PHONY: modules modules_install clean
