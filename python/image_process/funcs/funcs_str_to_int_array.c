#include <Python.h>
#include <stdio.h>
#include "funcs.h"

void 
str_to_int_array( 
    const unsigned char* raw , 
    int raw_size , 
    PyObject* raw_list ) {
    // printf( "[+] %d\n" , raw_size ) ;
    // printf( "[+] %s %d\n" , raw , PyInt_AsLong( PyList_GetItem( raw_list , 1 ) ) ) ;
    // PyList_SetItem( raw_list , 0 , PyInt_FromLong( 44 ) ) ;
    // PyList_Append( raw_list , PyInt_FromLong( i ) ) ;
    // printf( "[+] %d\n" , raw[0] ) ;
    int i ;
    for( i = 0 ; i < raw_size ; i += 2 )
        PyList_Append( 
            raw_list ,
            PyInt_FromLong( ( (raw[i])|(raw[i+1]<<8) ) * 256 / 1024 ) ) ;
    return ;
}