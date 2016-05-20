CC=gcc
CXX=g++
RM=rm -f
# CPPFLAGS=-g -I $(PYTHON_HOME)\include
# LDFLAGS=-g -shared
# LDLIBS=-L $(PYTHON_HOME)\libs -lpython27 
SRCS=main.c
TARGET=main.exe

OBJS=$(subst .c,.o,$(SRCS))

all : $(TARGET)

show :
	@echo processor_architechture = $(PROCESSOR_ARCHITECTURE)
	@echo operating system = ${OS}
	@echo cppflags = ${CPPFLAGS}
	@echo ldflags  = ${LDFLAGS}

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

ifeq ($(OS),Windows_NT) 
    ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
        # CPPFLAGS += -D AMD64
        # LDFLAGS += -D AMD64
        CPPFLAGS += -D MS_WIN64
        LDFLAGS += -D MS_WIN64
    endif
    ifeq ($(PROCESSOR_ARCHITECTURE),x86)
        CPPFLAGS += -D IA32
        LDFLAGS += -D IA32
    endif
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        CCFLAGS += -D LINUX
        LDFLAGS += -D LINUX
    endif
    ifeq ($(UNAME_S),Darwin)
        CCFLAGS += -D OSX
        LDFLAGS += -D OSX
    endif
    UNAME_P := $(shell uname -p)
    ifeq ($(UNAME_P),x86_64)
        CCFLAGS += -D AMD64
        LDFLAGS += -D AMD64
    endif
    ifneq ($(filter %86,$(UNAME_P)),)
        CCFLAGS += -D IA32
        LDFLAGS += -D IA32
    endif
    ifneq ($(filter arm%,$(UNAME_P)),)
        CCFLAGS += -D ARM
        LDFLAGS += -D ARM
    endif
	CPPFLAGS=$(shell root-config --cflags)
	LDFLAGS=$(shell root-config --ldflags)
	LDLIBS=$(shell root-config --libs)
endif

# from http://stackoverflow.com/questions/2481269/how-to-make-simple-c-makefile