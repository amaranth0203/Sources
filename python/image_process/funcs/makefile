CC=gcc
RM=rm -f
ifeq ($(OS),Windows_NT)
    CPPFLAGS=-D MS_WIN64 -g -shared -I $(PYTHON_HOME)\include -L $(PYTHON_HOME)\libs -lpython27 
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
# gcc -g -D MS_WIN64 -shared -c -I D:\executable\Python27_64bit\include -o funcs_brighten.o funcs_brighten.c                 
# gcc -g -D MS_WIN64 -shared -c -I D:\executable\Python27_64bit\include -o funcs_demosaic.o funcs_demosaic.c                 
# gcc -g -D MS_WIN64 -shared -c -I D:\executable\Python27_64bit\include -o funcs_str_to_int_array.o funcs_str_to_int_array.c 
# gcc -g -D MS_WIN64 -shared -c -I D:\executable\Python27_64bit\include -o funcs.o funcs.c                                   
# gcc -g -D MS_WIN64 -shared    -I D:\executable\Python27_64bit\include -o funcs.pyd funcs.o funcs_brighten.o funcs_demosaic.o funcs_str_to_int_array.o -L D:\executable\Python27_64bit\libs -lpython27 

