main :
	rc qyh_adb.rc 
	cl /w /c qyh_adb.c
	cl /w /c main.c
	link *.obj qyh_adb.RES /SUBSYSTEM:WINDOWS /LIBPATH:"D:\Program Files (x86)\vs2015\lib" kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib OpenGL32.Lib /OUT:qyh_adb.exe

clean:
	del *.res
	del *.obj
	del *.exe
