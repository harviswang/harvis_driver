#Makefile 2.6

obj-m := jiffies_64.o
KERNEL := /Android/linux-2.6.39-android_altus
PWD := $(shell pwd)
modules:
	$(MAKE) -C $(KERNEL) M=$(PWD) modules

.PHONEY:clean install
install:
	cp -f jiffies_64.ko /tftpboot/
clean:
	rm -f *.o *.ko

