CC=gcc
CXX=g++
RM=rm -f
ifeq ($(OS),Windows_NT) 
    ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
        # CPPFLAGS += -D AMD64
        CPPFLAGS += -D MS_WIN64
    endif
    ifeq ($(PROCESSOR_ARCHITECTURE),x86)
        CPPFLAGS += -D IA32
    endif
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        CCFLAGS += -D LINUX
    endif
    ifeq ($(UNAME_S),Darwin)
        CCFLAGS += -D OSX
    endif
    UNAME_P := $(shell uname -p)
    ifeq ($(UNAME_P),x86_64)
        CCFLAGS += -D AMD64
    endif
    ifneq ($(filter %86,$(UNAME_P)),)
        CCFLAGS += -D IA32
    endif
    ifneq ($(filter arm%,$(UNAME_P)),)
        CCFLAGS += -D ARM
    endif
    CPPFLAGS=$(shell root-config --cflags)
    LDFLAGS=$(shell root-config --ldflags)
    LDLIBS=$(shell root-config --libs)
endif
CPPFLAGS+=-I $(PYTHON_HOME)\include 
LDFLAGS+=-shared  
LDLIBS+=-L $(PYTHON_HOME)\libs -lpython27
SRCS=funcs.c funcs_str_to_int_array.c funcs_demosaic.c funcs_brighten.c funcs_awb.c
TARGET=funcs.pyd

OBJS=$(subst .c,.o,$(SRCS))

all : $(TARGET)

show :
	@echo processor_architechture = $(PROCESSOR_ARCHITECTURE)
	@echo operating system = ${OS}
	@echo cppflags = ${CPPFLAGS}
	@echo ldflags  = ${LDFLAGS}

release : $(TARGET)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS) -DNDEBUG -O3 -s

debug : $(TARGET)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS) -DEBUG -O0

$(TARGET) : $(OBJS) 
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS) 

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
# gcc -g -D MS_WIN64 -c -I D:\executable\Python27_64bit\include -o funcs_awb.o funcs_awb.c 
# gcc -g -D MS_WIN64 -c -I D:\executable\Python27_64bit\include -o funcs_brighten.o funcs_brighten.c                 
# gcc -g -D MS_WIN64 -c -I D:\executable\Python27_64bit\include -o funcs_demosaic.o funcs_demosaic.c                 
# gcc -g -D MS_WIN64 -c -I D:\executable\Python27_64bit\include -o funcs_str_to_int_array.o funcs_str_to_int_array.c 
# gcc -g -D MS_WIN64 -c -I D:\executable\Python27_64bit\include -o funcs.o funcs.c                                   
# gcc -g -D MS_WIN64 -shared -o funcs.pyd funcs.o funcs_awb.o funcs_brighten.o funcs_demosaic.o funcs_str_to_int_array.o -L D:\executable\Python27_64bit\libs -lpython27 

