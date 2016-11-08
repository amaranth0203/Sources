main :
	windres qyh_adb.rc -o qyh_adb.rc.o
	gcc -c qyh_adb.c
	gcc -o qyh_adb qyh_adb.o qyh_adb.rc.o -mwindows

clean:
	del *.o
	del *.exe