#Makefile 2.6

MODULE := jiffies_64
obj-m := $(MODULE).o
KERNEL ?= /Android/linux-2.6.39-android_altus
PWD := $(shell pwd)
modules:
	$(MAKE) -C $(KERNEL) M=$(PWD) modules

.PHONEY:clean install
install:
	cp -f $(MODULE).ko /tftpboot/
clean:
	rm -f *.o *.ko* *.mod.c *.order *.symvers
	rm -fr *.ko.cmd  *.mod.o.cmd  *.o.cmd   .tmp_versions/

