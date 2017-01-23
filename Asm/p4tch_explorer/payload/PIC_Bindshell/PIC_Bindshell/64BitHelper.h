#ifndef _64BitHelper_H_
#define _64BitHelper_H_

#if defined(_WIN64)
extern VOID AlignRSP( VOID );


extern void jmp_end_of_file( ) ;
extern void end_of_file_x64( ) ;

VOID Begin( VOID )
{
	// Call the ASM stub that will guarantee 16-byte stack alignment.
	// The stub will then call the ExecutePayload.
	AlignRSP();
}
void wrapper_jmp_end_of_file( ) {
	jmp_end_of_file( ) ;
}
#endif

#endif // end of _64BitHelper_H_