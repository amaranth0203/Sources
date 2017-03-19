
ifeq ($(debug),1)
	CPPFLAGS+=-D_DEBUG_
endif

CC=gcc
CXX=g++
ifeq ($(OS),Windows_NT)
	RM=del /f /s /q
	FixPath=$(subst /,\,$1)
	CPPFLAGS=-g -c
	LDFLAGS=
	LDLIBS=
	EXE=.exe
	DLL=.dll
else
	RM=rm -f
	FixPath=$1
	CPPFLAGS=-g -c -fPIC
	LDFLAGS=
	LDLIBS=-ldl
	EXE=
	DLL=.so
endif

SRCS=*.c

SRCS_MAIN=main.c funcs.c
OBJS_MAIN=$(subst .c,.o,$(SRCS_MAIN))

all:main

include .de
.de: $(SRCS)
	$(CC) $(CPPFLAGS) -MM $^ >> $(call FixPath,"./.de")

main:$(OBJS_MAIN)
	$(CC) $(LDFLAGS) -o main$(EXE) $(OBJS_MAIN) $(LDLIBS)

clean :
	$(RM) *.o
dist-clean:
	$(RM) .de *.o *.exe *.dll *.out *.so main$(EXE) caller$(EXE)
