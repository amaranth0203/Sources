#include <Python.h>
#include <stdio.h>
#include "funcs.h"

void brighten(   
    PyObject* r_list , 
    PyObject* g_list , 
    PyObject* b_list ,
    float strength ) {

    int i , size = PyList_GET_SIZE( r_list ) ;
    
    for( i = 0 ; i < size ; i ++ ) {
        PyList_SetItem( r_list , i , PyInt_FromLong(
            PyInt_AsLong( PyList_GetItem( r_list , i ) ) * strength < 255 ? 
            PyInt_AsLong( PyList_GetItem( r_list , i ) ) * strength : 255 ) ) ;
        PyList_SetItem( g_list , i , PyInt_FromLong(
            PyInt_AsLong( PyList_GetItem( g_list , i ) ) * strength < 255 ? 
            PyInt_AsLong( PyList_GetItem( g_list , i ) ) * strength : 255 ) ) ;
        PyList_SetItem( b_list , i , PyInt_FromLong(
            PyInt_AsLong( PyList_GetItem( b_list , i ) ) * strength < 255 ? 
            PyInt_AsLong( PyList_GetItem( b_list , i ) ) * strength : 255 ) ) ;
    }

    return ;

}