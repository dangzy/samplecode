obj-m := helloworld.o
KERNELBUILD := /lib/modules/`uname -r`/build
all:
	make -C $(KERNELBUILD) M=$(shell pwd) modules
clean:
	rm -rf *.o  *.ko  *.mod.c .tmp_versions modules* Module*
