#ifndef _FUNCS_H_
#define _FUNCS_H_
#include <stdint.h>

void print_512_bit_chunk( unsigned char* chunk ) ;
void print_128_bit_chunk( unsigned char* chunk ) ;
int pad_input( unsigned char* in_raw , unsigned char** in_padded ) ;
void break_512_chunk_into_w( unsigned char* chunk , uint32_t* w ) ;
uint32_t leftrotate( uint32_t x , uint32_t c ) ;
void print_result( uint32_t a0 , uint32_t b0 , uint32_t c0 , uint32_t d0 ) ;
char* get_sha1sum( unsigned char* in ) ;
#endif
