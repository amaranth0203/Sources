
    ;; ------------------------------ how to compile ------------------------------
    ;; nasm -f win64 messageBox64bit.asm -o messageBox64bit.obj
    ;; golink /console messageBox64bit.obj
    ;; ------------------------------ how to compile ------------------------------

bits 64
section .text
global start

start:  
    jmp real_start
    
;get strings	
getUser32:
	call returnGetUser32
	db  'user32.dll'
	db	0x00
getTitle:
	call returnGetTitle
	db  'This is fun!'
	db	0x00
getText:
	call returnGetText
	db  '0xdeadbeef'
	db	0x00

;Hashing section to resolve a function address	
GetProcessAddress:		
	mov r13, rcx                     ;base address of dll loaded 
	mov eax, [r13d + 0x3c]           ;skip DOS header and go to PE header
	mov r14d, [r13d + eax + 0x88]    ;0x88 offset from the PE header is the export table. 

	add r14d, r13d                  ;make the export table an absolute base address and put it in r14d.
	mov r10d, [r14d + 0x18]         ;go into the export table and get the numberOfNames 
	mov ebx, [r14d + 0x20]          ;get the AddressOfNames offset. 
	add ebx, r13d                   ;AddressofNames base. 
	
find_function_loop:	
	jecxz find_function_finished   ;if ecx is zero, quit :( nothing found. 
	dec r10d                       ;dec ECX by one for the loop until a match/none are found
	mov esi, [ebx + r10d * 4]      ;get a name to play with from the export table. 
	add esi, r13d                  ;esi is now the current name to search on. 
	
find_hashes:
	xor edi, edi
	xor eax, eax
	cld			
	
continue_hashing:	
	lodsb                         ;get into al from esi
	test al, al                   ;is the end of string resarched?
	jz compute_hash_finished
	ror dword edi, 0xd            ;ROR13 for hash calculation!
	add edi, eax		
	jmp continue_hashing
	
compute_hash_finished:
	cmp edi, edx                  ;edx has the function hash
	jnz find_function_loop        ;didn't match, keep trying!
	mov ebx, [r14d + 0x24]        ;put the address of the ordinal table and put it in ebx. 
	add ebx, r13d                 ;absolute address
	xor ecx, ecx                  ;ensure ecx is 0'd. 
	mov cx, [ebx + 2 * r10d]      ;ordinal = 2 bytes. Get the current ordinal and put it in cx. ECX was our counter for which # we were in. 
	mov ebx, [r14d + 0x1c]        ;extract the address table offset
	add ebx, r13d                 ;put absolute address in EBX.
	mov eax, [ebx + 4 * ecx]      ;relative address
	add eax, r13d	
	
find_function_finished:
	ret 

real_start: 
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
    push rax
    push rcx
    push rdx
    push rbx
    push rsp
    push rbp
    push rsi
    push rdi
    
;get dll base addresses
	sub rsp, 28h                     ;reserve stack space for called functions
	and rsp, 0fffffffffffffff0h      ;make sure stack 16-byte aligned   
 
	mov r12, [gs:60h]                ;peb
	mov r12, [r12 + 0x18]            ;Peb --> LDR
	mov r12, [r12 + 0x20]            ;Peb.Ldr.InMemoryOrderModuleList
	mov r12, [r12]                   ;2st entry
	mov r15, [r12 + 0x20]            ;ntdll.dll base address!
	mov r12, [r12]                   ;3nd entry
	mov r12, [r12 + 0x20]            ;kernel32.dll base address!
 
;find address of loadLibraryA from kernel32.dll which was found above. 
	mov rdx, 0xec0e4e8e
	mov rcx, r12
	call GetProcessAddress         
 
;import user32
	jmp getUser32
returnGetUser32:
	pop rcx
	call rax                        ;load user32.dll
	
;get messageBox address
	mov rdx, 0xbc4da2a8
	mov rcx, rax
	call GetProcessAddress  
	mov rbx, rax

;messageBox
	xor r9, r9                     ;uType
	jmp getText
returnGetText:
	pop r8                         ;lpCaption
	jmp getTitle
returnGetTitle:
	pop rdx                        ;lpTitle
	xor rcx, rcx                   ;hWnd
	call rbx                       ;display message box	
	
;ExitProcess
	;; mov rdx, 0x2d3fcd70				
	;; mov rcx, r15
	;; call GetProcessAddress
	;; xor  rcx, rcx                  ;uExitCode
	;; call rax     
    
    ;; -------------------------------------------------------------
    ;; stand-alone message-box program ends
    ;; -------------------------------------------------------------


    add rsp, 28h                     ;reserve stack space for called functions
    pop rdi
    pop rsi
    pop rbp
    pop rsp
    pop rbx
    pop rdx
    pop rcx
    pop rax
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
