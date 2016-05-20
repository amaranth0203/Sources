#include <Python.h>
#include <stdio.h>
#include "funcs.h"

void awb(   
    PyObject* r_list , 
    PyObject* g_list , 
    PyObject* b_list ) {

    int i ;
    long long RSum , GSum , BSum ;
    float r_gain , g_gain , b_gain ;

    for( i = 0 ; i < PyList_GET_SIZE( r_list ) ; i ++ ) {
        RSum += PyInt_AsLong( PyList_GetItem( r_list , i ) ) ;
        GSum += PyInt_AsLong( PyList_GetItem( g_list , i ) ) ;
        BSum += PyInt_AsLong( PyList_GetItem( b_list , i ) ) ;
    }

    r_gain = ( float )( RSum + GSum + BSum ) / ( 3 * RSum ) ;
    g_gain = ( float )( RSum + GSum + BSum ) / ( 3 * GSum ) ;
    b_gain = ( float )( RSum + GSum + BSum ) / ( 3 * BSum ) ;

    printf( "[+] awb : r_gain , g_gain , b_gain = %g , %g , %g\n" , r_gain , g_gain , b_gain ) ;

    for( i = 0 ; i < PyList_GET_SIZE( r_list ) ; i ++ )
        PyList_SetItem( 
            r_list , 
            i , 
            PyInt_FromLong( 
                PyInt_AsLong( PyList_GetItem( r_list , i ) ) * r_gain < 255 ?
                PyInt_AsLong( PyList_GetItem( r_list , i ) ) * r_gain : 255
            ) 
        ) ;

    for( i = 0 ; i < PyList_GET_SIZE( g_list ) ; i ++ )
        PyList_SetItem( 
            g_list , 
            i , 
            PyInt_FromLong( 
                PyInt_AsLong( PyList_GetItem( g_list , i ) ) * g_gain < 255 ?
                PyInt_AsLong( PyList_GetItem( g_list , i ) ) * g_gain : 255
            ) 
        ) ;

    for( i = 0 ; i < PyList_GET_SIZE( b_list ) ; i ++ )
        PyList_SetItem( 
            b_list , 
            i , 
            PyInt_FromLong( 
                PyInt_AsLong( PyList_GetItem( b_list , i ) ) * b_gain < 255 ?
                PyInt_AsLong( PyList_GetItem( b_list , i ) ) * b_gain : 255
            ) 
        ) ;

    return ;

}