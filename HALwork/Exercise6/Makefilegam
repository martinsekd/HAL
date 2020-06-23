# To build modules outside of the kernel tree, we run "make"
# in the kernel source tree; the Makefile these then includes this
# Makefile once again.
# This conditional selects whether we are being included from the
# kernel Makefile or not.
ifeq ($(KERNELRELEASE),)
# If KERNELDIR is not set by an environment variable
ifeq ($(KERNELDIR),)
	# Assume the source tree is where the running kernel was built
	KERNELDIR = ~/sources/rpi-4.14
endif
# If CCPREFIX is not set by an environment variable
ifeq ($(CCPREFIX),)
	CCPREFIX = arm-poky-linux-gnueabi-
endif
  # The current directory is passed to sub-makes as argument
  PWD := $(shell pwd)
modules:
	$(MAKE) ARCH=arm CROSS_COMPILE=$(CCPREFIX) -C $(KERNELDIR) M=$(PWD) modules
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions modules.order Module.symvers
.PHONY: modules clean
else
  # called from kernel build system: just declare what our modules are
  # Avoid c90 warning on mixed declaration
	ccflags-y := -DDEBUG -g -std=gnu99 -Wno-declaration-after-statement -Werror
	obj-m := driver.o
endif
