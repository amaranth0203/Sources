#include <Python.h>
#include <stdio.h>

void 
demosaic(   
    PyObject* raw_list , 
    PyObject* r_list , 
    PyObject* g_list , 
    PyObject* b_list , 
    int w ,
    int h ) {
    // printf( "[+] %d\n" , raw_size ) ;
    // printf( "[+] %s %d\n" , raw , PyInt_AsLong( PyList_GetItem( raw_list , 1 ) ) ) ;
    // PyList_SetItem( raw_list , 0 , PyInt_FromLong( 44 ) ) ;
    // PyList_Append( raw_list , PyInt_FromLong( i ) ) ;
    // printf( "[+] %d\n" , raw[0] ) ;
    int i , j ;
    for( i = 1 ; i < h -1 ; i ++ ) {
        for( j = 1 ; j < w - 1 ; j ++ ) {
            if( i % 2 == 0 ) {
                if( j % 2 == 0 ) {
                } else {
                }
            } else {
                if( j % 2 == 0 ) {
                } else {
                }
            }
        }
    }
    return ;
}







