CC=gcc
RM=rm -f
ifeq ($(OS),Windows_NT)
    CPPFLAGS=-D MS_WIN64 -g -shared -I D:\executable\Python27_64bit\include -L D:\executable\Python27_64bit\libs -lpython27 
else
	CPPFLAGS=-g $(shell root-config --cflags)
	LDFLAGS=-g $(shell root-config --ldflags)
	LDLIBS=$(shell root-config --libs)
endif

SRCS=funcs.c

all: 
	$(CC) -o funcs.pyd $(SRCS) $(CPPFLAGS)

clean:
	$(RM) *.pyd


# from http://stackoverflow.com/questions/2481269/how-to-make-simple-c-makefile
