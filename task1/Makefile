VER = $(shell uname -r)
KDIR = /lib/modules/$(CURVER)/build

TARGET = task1

obj-m := $(TARGET).o

all:
		$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
		$(MAKE) -C $(KDIR) M=$(PWD) clean

