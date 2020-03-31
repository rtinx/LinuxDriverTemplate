################################################################################
# RTinX Linux Driver Template Makefile.
################################################################################

################################################################################
# Compile options
################################################################################
EXTRA_CFLAGS += 

################################################################################
# Module files
################################################################################
obj-m += rtinx.o

EXTRA_CFLAGS += -I./

rtinx-objs := rtinx_pinctl.o
              
ifneq ($(KERNELRELEASE),)

################################################################################
# If KERNELRELEASE is defined, we've been invoked from the
# kernel build system and can use its language.
################################################################################

else

################################################################################
# Otherwise we were called directly from the command
# line; invoke the kernel build system.
################################################################################

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

endif
