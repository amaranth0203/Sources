
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

SRCS_DLL=funcs.c
OBJS_DLL=$(subst .c,.o,$(SRCS_DLL))

SRCS_CALLER=caller.c
OBJS_CALLER=$(subst .c,.o,$(SRCS_CALLER))

all:main dll caller

include .de
.de: $(SRCS)
	$(CC) $(CPPFLAGS) -MM $^ >> $(call FixPath,"./.de")

main:$(OBJS_MAIN)
	$(CC) $(LDFLAGS) -o main$(EXE) $(OBJS_MAIN) $(LDLIBS)

dll:$(OBJS_DLL)
	$(CC) $(LDFLAGS) -shared -o sha1sum$(DLL) $(OBJS_DLL) $(LDLIBS)

caller:$(OBJS_CALLER)
	$(CC) $(LDFLAGS) -o caller$(EXE) $(OBJS_CALLER) $(LDLIBS)

clean :
	$(RM) *.o
dist-clean:
	$(RM) .de *.o *.exe *.dll *.out *.so main$(EXE) caller$(EXE)
