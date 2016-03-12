CC=gcc
CXX=g++
RM=rm -f
ifeq ($(OS),Windows_NT) 
	CPPFLAGS=-g -D MS_WIN64 -I $(PYTHON_HOME)\include
	LDFLAGS=-g -D MS_WIN64 -shared
	LDLIBS=-L $(PYTHON_HOME)\libs -lpython27 
else
	CPPFLAGS=-g $(shell root-config --cflags)
	LDFLAGS=-g $(shell root-config --ldflags)
	LDLIBS=$(shell root-config --libs)
endif

SRCS=funcs.c funcs_str_to_int_array.c funcs_demosaic.c funcs_brighten.c
OBJS=$(subst .c,.o,$(SRCS))
TARGET=funcs.pyd

all : $(TARGET)
	$(info ${OS})

$(TARGET) : $(OBJS) 
	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS) 

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend ;

clean :
	$(RM) $(OBJS) 

dist-clean : clean
	$(RM) $(TARGET) .depend

include .depend

# from http://stackoverflow.com/questions/2481269/how-to-make-simple-c-makefile
# gcc -g -D MS_WIN64 -c -I D:\binary\Python27_64\include -o funcs_brighten.o funcs_brighten.c                 
# gcc -g -D MS_WIN64 -c -I D:\binary\Python27_64\include -o funcs_demosaic.o funcs_demosaic.c                 
# gcc -g -D MS_WIN64 -c -I D:\binary\Python27_64\include -o funcs_str_to_int_array.o funcs_str_to_int_array.c 
# gcc -g -D MS_WIN64 -c -I D:\binary\Python27_64\include -o funcs.o funcs.c                                   
# gcc -g -D MS_WIN64 -shared -o funcs.pyd funcs.o funcs_brighten.o funcs_demosaic.o funcs_str_to_int_array.o -L D:\binary\Python27_64\libs -lpython27 

