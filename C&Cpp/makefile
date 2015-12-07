CC=gcc
CXX=g++
RM=rm -f
ifeq ($(OS),Windows_NT)
else
	CPPFLAGS=-g $(shell root-config --cflags)
	LDFLAGS=-g $(shell root-config --ldflags)
	LDLIBS=$(shell root-config --libs)
endif

SRCS=services.c
OBJS=$(subst .c,.o,$(SRCS))

all: tool

tool: $(OBJS)
	$(CXX) $(LDFLAGS) -o serv $(OBJS) $(LDLIBS) 

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) *~ .depend

include .depend


# from http://stackoverflow.com/questions/2481269/how-to-make-simple-c-makefile
