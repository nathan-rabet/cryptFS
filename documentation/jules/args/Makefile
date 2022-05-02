KERNELDIR = /lib/modules/$(shell uname -r)/build
PWD = $(shell pwd)
target = args
obj-m = args.o

modules: $(OBJS)
	$(MAKE) -C $(KERNELDIR) M=$(PWD) $@ $^

modules_install: $(OBJS)
	$(MAKE) -C $(KERNELDIR) M=$(PWD) $@ $^

clean:
	$(RM) *.o *~ core .depend .*.cmd *.ko *.mod.c *.tmp_versions *.mod *.order *.symvers

load:
	@sudo insmod ./$(target).ko login="aubert_o" age=42 grades=7,11,19

unload:
	@sudo rmmod $(target)

.PHONY: modules modules_install clean
