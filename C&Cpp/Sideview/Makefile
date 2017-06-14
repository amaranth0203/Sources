CPPFLAGS=-g
ifeq ($(debug),1)
	CPPFLAGS+=-D_DEBUG_
endif

main:
	gcc $(CPPFLAGS) -c *.c
	gcc $(CPPFLAGS) -o a.exe *.o

clean:
	del *.exe *.o
