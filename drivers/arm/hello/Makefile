ifeq ($(KERNELRELEASE),)
	KERNELDIR ?= ~/linux-2.6.32.2
	PWD := $(shell pwd)
modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

else
	obj-m:=hello.o

endif
