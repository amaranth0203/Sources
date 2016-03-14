#ifndef _FUNCS_H_
#define _FUNCS_H_

#include <Python.h>

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

void awb(   
    PyObject* r_list , 
    PyObject* g_list , 
    PyObject* b_list ) ;

#endif

