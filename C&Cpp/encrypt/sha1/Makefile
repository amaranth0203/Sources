main : main.c funcs.c funcs.h
	gcc -c -g main.c -o main.o
	gcc -c -g funcs.c -o funcs.o
	gcc main.o funcs.o -o a.exe
dll : funcs.c
	gcc -c funcs.c -o funcs.o
	gcc -Wl,--exclude-all-symbols -g -shared -o sha1sum.dll funcs.o
caller : caller.c
	gcc -c -g caller.c -o caller.o
	gcc caller.o -o a.exe
clean :
	del *.exe *.o *.dll
