MODULE_NAME :=net_link
obj-m += $(MODULE_NAME).o
KERNELDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
CFLAGS_hwf.o += -DDEBUG

all:
	make -C $(KERNELDIR) M=$(PWD) modules
	gcc -o sender sender.c
clean:
	make -C $(KERNELDIR) M=$(PWD) clean
