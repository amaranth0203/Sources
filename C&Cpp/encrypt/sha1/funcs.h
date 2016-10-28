#ifndef _FUNCS_H_
#define _FUNCS_H_
#include <stdint.h>

static uint32_t h0 = 0x67452301 ;
static uint32_t h1 = 0xefcdab89 ;
static uint32_t h2 = 0x98badcfe ;
static uint32_t h3 = 0x10325476 ;
static uint32_t h4 = 0xc3d2e1f0 ;

void print_512_bit_chunk( unsigned char* chunk ) ;
void print_128_bit_chunk( unsigned char* chunk ) ;
int pad_input( unsigned char* in_raw , unsigned char** in_padded ) ;
void break_512_chunk_into_w( unsigned char* chunk , uint32_t* w ) ;
uint32_t leftrotate( uint32_t x , uint32_t c ) ;
void print_result( uint32_t a0 , uint32_t b0 , uint32_t c0 , uint32_t d0 ) ;
__declspec( dllexport ) char* get_sha1sum( unsigned char* in ) ;
#endif
