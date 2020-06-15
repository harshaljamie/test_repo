## info link : https://www.kernel.org/doc/html/latest/kbuild/makefiles.html
## this is makefile required for compiling and loading the module in kernel

obj-m +=test_driver_code.o

##obj-m += hello-1.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
		
## this is test to edit split screens
