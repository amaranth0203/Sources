
ifeq ($(debug),1)
	CPPFLAGS+=-D_DEBUG_
endif

CC=gcc
CXX=g++
ifeq ($(OS),Windows_NT)
	RM=del /f /q
	FixPath=$(subst /,\,$1)
	CPPFLAGS=-g -c
	LDFLAGS=
	LDLIBS=-static-libgcc snap7.lib -lws2_32
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

SRCS=*.cpp

SRCS_MAIN=main_duo_lu_tong.cpp funcs.cpp snap7.cpp
OBJS_MAIN=$(subst .cpp,.o,$(SRCS_MAIN))

all:main

include .de
.de: $(SRCS)
	$(CXX) $(CPPFLAGS) -MM $^ >> $(call FixPath,"./.de")

main:$(OBJS_MAIN)
	$(CXX) $(LDFLAGS) -o main_duo_lu_tong$(EXE) $(OBJS_MAIN) $(LDLIBS)
svr:
	g++ -o server.exe server.cpp snap7.cpp funcs.cpp -static-libgcc snap7.lib -lws2_32
test :
	g++ -o client.exe client.cpp snap7.cpp -static-libgcc snap7.lib

clean :
	$(RM) *.o
dist-clean:
	$(RM) .de *.o *.exe *.out *.so main$(EXE) caller$(EXE)
