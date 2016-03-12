#include <Python.h>
#include "funcs_str_to_int_array.c"
#include "funcs_demosaic.c"
#include "funcs_brighten.c"

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

void brighten(   
    PyObject* r_list , 
    PyObject* g_list , 
    PyObject* b_list , 
    float strength ) ;