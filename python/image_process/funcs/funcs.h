#include <Python.h>
#include "str_to_int_array.c"
#include "demosaic.c"

void str_to_int_array( 
    const unsigned char* raw , 
    int raw_size , 
    PyObject* raw_list ) ;

void demosaic(   
    PyObject* raw_list , 
    PyObject* r_list , 
    PyObject* g_list , 
    PyObject* b_list , 
    int w ,
    int h ) ;