ifneq ($(KERNELRELEASE),)
	obj-m := hello.o
else
	KERNEL_DIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
default:
	$(MAKE) -C ${KERNEL_DIR} M=$(PWD) modules
clean:
	$(MAKE) -C ${KERNEL_DIR} M=$(PWD) clean
endif
