all :
	gcc -c testDll.c -o testDll.o
	gcc -shared testDll.o -o testDll.dll
	gcc testCallDll.c -L./ -ltestDll -o testCallDll.exe
clean :
	del *.exe & del *.o & del *.dll