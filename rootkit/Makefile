CC				= gcc
CFALGS			= -Wall
ROOTKIT			:= rootkit
HEADERS			:=$(PWD)/headers
obj-m			:= $(ROOTKIT).o
#$(ROOTKIT)-y	+= file.o 
#$(ROOTKIT)-y    += module.o 
#$(ROOTKIT)-y	+= fops.o 
#$(ROOTKIT)-y	+= keylogger.o 
#$(ROOTKIT)-y	+= module.o 
#$(ROOTKIT)-y	+= process.o 
#$(ROOTKIT)-y	+= rootkit.o 
#$(ROOTKIT)-y	+= sockets.o 
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean

