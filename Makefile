KERNEL_DIR = /workplace/linux/linux-4.19.94

CURRENT_DIR = $(shell pwd)

obj-m = chardevice.o

build:module_buile

module_buile:
	cd $(KERNEL_DIR) && $(MAKE) M=$(CURRENT_DIR) modules
	#$(MAKE) -C $(KERNEL_DIR) M=$(CURRENT_DIR) modules

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(CURRENT_DIR) clean
	
