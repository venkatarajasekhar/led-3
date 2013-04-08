#
# 
#  Makefile for ARM/Linux user application
# 

SHELL = /bin/sh

CC = arm-linux-gcc

all: led

led: led.c
	${CC} -I../../include -g -o $@ $<

clean:
	rm -f *.o
	rm -f led
