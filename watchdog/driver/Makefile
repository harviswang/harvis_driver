#Makefile 2.6

obj-m := at91sam9_wdt.o
KERNEL := /Android/linux-2.6.39-android_altus
PWD := $(shell pwd)
modules:
	$(MAKE) -C $(KERNEL) M=$(PWD) modules

.PHONEY:clean
clean:
	rm -f *.o *.ko

