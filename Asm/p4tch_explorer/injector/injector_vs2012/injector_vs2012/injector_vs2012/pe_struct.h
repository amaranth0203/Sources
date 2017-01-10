#include <Windows.h>

/*
typedef struct _IMAGE_DOS_HEADER
{
     WORD e_magic;
     WORD e_cblp;
     WORD e_cp;
     WORD e_crlc;
     WORD e_cparhdr;
     WORD e_minalloc;
     WORD e_maxalloc;
     WORD e_ss;
     WORD e_sp;
     WORD e_csum;
     WORD e_ip;
     WORD e_cs;
     WORD e_lfarlc;
     WORD e_ovno;
     WORD e_res[4];
     WORD e_oemid;
     WORD e_oeminfo;
     WORD e_res2[10];
     LONG e_lfanew;
} IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;
*/

typedef struct _NEWPEINF //壳程序备份加壳后的PE信息
{
	LONG  e_lfanew;                //1.定位PE标签地址
	WORD  NumberOfSections;        //2.PE节计数
	WORD  SizeOfOptionalHeader;    //3.PE扩展头大小
	DWORD SizeOfCode;              //4.带有IMAGE_SCN_CNT_CODE属性的所有节的总大小
	DWORD SizeOfInitializedData;   //5.所有由已初始化的数据组成的节的总大小
	DWORD SizeOfUninitializedData; //6.所有由未初始化的数据组成的节的总大小
	DWORD AddressOfEntryPoint;     //7.程序入口，文件中首先被执行的代码的第一个字节的RVA
	DWORD BaseOfCode;              //8.加载进内存之后代码的第一个字节的RVA
	DWORD ImageBase;               //9.预加载地址，文件在内存中的首选加载地址
	DWORD SectionAlignment;        //10.内存节对齐粒度
	DWORD FileAlignment;           //11.pe节的文件对齐值
	DWORD SizeOfImage;             //12.PE内存中的映像尺寸
	DWORD SizeOfHeaders;           //13.PE所有节头加节表的大小
	DWORD NumberOfRvaAndSizes;     //14
	IMAGE_DATA_DIRECTORY DataDirectory[16];//15.数据目录
	DWORD SizeOfRawData;           //16.所有节在文件中对齐后大小
}NEWPEINF,*PNEWPEINF;