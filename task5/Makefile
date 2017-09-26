obj-m := task5.o

.PHONY: clean

all:
		make -C /lib/modules/4.4.0-93-generic/build M=$(PWD) modules

clean:
		make -C /lib/modules/4.4.0-93-generic/build M=$(PWD) clean
