INC=-I$(realpath ../machinekit/src/hal/lib) -I$(realpath ../machinekit/src/rtapi)

ifneq ($(KERNELRELEASE),)
# kbuild part of makefile
obj-m  := hal_xadc.o

else
# normal makefile
    KDIR ?= $(realpath ../Snickerdoodle/kernel)

default:
	$(MAKE) $(INC) -C $(KDIR) M=$$PWD

endif
