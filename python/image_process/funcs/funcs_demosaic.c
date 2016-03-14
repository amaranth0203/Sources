#include <Python.h>
#include <stdio.h>
#include "funcs.h"

void 
demosaic(   
    PyObject* raw_list , 
    PyObject* r_list , 
    PyObject* g_list , 
    PyObject* b_list , 
    int w ,
    int h ) {

    int i , j , r_value , g_value , b_value ;

    for( i = 0 ; i < h ; i ++ ) {
        for( j = 0 ; j < w ; j ++ ) {
            r_value = g_value = b_value = 0 ;
            if( i == 0 || j == 0 || i == h - 1 || j == w - 1 ) { // edge and corner
                if(      i == 0     && j != 0 && j != w - 1 ) { // top
                    if( j % 2 == 0 ) { // G1
                        r_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+(j-1) ) ) 
                                + PyInt_AsLong( PyList_GetItem( raw_list , i*w+(j+1) ) ) ;
                        r_value >>= 1 ;
                        g_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+j ) ) 
                                + PyInt_AsLong( PyList_GetItem( raw_list , (i+1)*w+(j-1) ) ) 
                                + PyInt_AsLong( PyList_GetItem( raw_list , (i+1)*w+(j+1) ) ) ;
                        g_value /= 3 ;
                        b_value = PyInt_AsLong( PyList_GetItem( raw_list , (i+1)*w+j ) ) ;
                    }
                    else{ // r
                        r_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+j ) ) ;
                        g_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+(j-1) ) )
                                + PyInt_AsLong( PyList_GetItem( raw_list , i*w+(j+1) ) )
                                + PyInt_AsLong( PyList_GetItem( raw_list , (i+1)*w+j ) ) ;
                        g_value /= 3 ;
                        b_value = PyInt_AsLong( PyList_GetItem( raw_list , (i+1)*w+(j-1) ) ) 
                                + PyInt_AsLong( PyList_GetItem( raw_list , (i+1)*w+(j+1) ) ) ;
                        b_value >>= 1 ;
                    }
                }
                else if( i == h - 1 && j != 0 && j != w - 1 ) { // bottom
                    if( h % 2 == 0 ){
                        if( j % 2 == 0 ) { // b
                            r_value = PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+(j-1) ) ) 
                                    + PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+(j+1) ) ) ;
                            r_value >>= 1 ;
                            g_value = PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+j ) ) 
                                    + PyInt_AsLong( PyList_GetItem( raw_list , i*w+(j-1) ) ) 
                                    + PyInt_AsLong( PyList_GetItem( raw_list , i*w+(j+1) ) ) ;
                            g_value /= 3 ;
                            b_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+j ) ) ;
                        }
                        else{ // G2
                            r_value = PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+j ) ) ;
                            g_value = PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+(j-1) ) )
                                    + PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+(j+1) ) )
                                    + PyInt_AsLong( PyList_GetItem( raw_list , i*w+j ) ) ;
                            g_value /= 3 ;
                            b_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+(j-1) ) ) 
                                    + PyInt_AsLong( PyList_GetItem( raw_list , i*w+(j+1) ) ) ;
                            b_value >>= 1 ;
                        }
                    }
                    else{
                        if( j % 2 == 0 ) { // G1
                            r_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+(j-1) ) ) 
                                    + PyInt_AsLong( PyList_GetItem( raw_list , i*w+(j+1) ) ) ;
                            r_value >>= 1 ;
                            g_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+j ) ) 
                                    + PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+(j-1) ) ) 
                                    + PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+(j+1) ) ) ;
                            g_value /= 3 ;
                            b_value = PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+j ) ) ;
                        }
                        else{ // r
                            r_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+j ) ) ;
                            g_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+(j-1) ) )
                                    + PyInt_AsLong( PyList_GetItem( raw_list , i*w+(j+1) ) )
                                    + PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+j ) ) ;
                            g_value /= 3 ;
                            b_value = PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+(j-1) ) ) 
                                    + PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+(j+1) ) ) ;
                            b_value >>= 1 ;
                        }
                    }
                }
                else if( j == 0     && i != 0 && i != h - 1 ) { // left
                    if( i % 2 == 0 ) { // G1
                        r_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+(j+1) ) ) ;
                        g_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+j ) )
                                + PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+(j+1) ) )
                                + PyInt_AsLong( PyList_GetItem( raw_list , (i+1)*w+(j+1) ) ) ;
                        g_value /= 3 ;
                        b_value = PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+j ) ) 
                                + PyInt_AsLong( PyList_GetItem( raw_list , (i+1)*w+j ) ) ;
                        b_value >>= 1 ;
                    }
                    else{ // b
                        r_value = PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+(j+1) ) ) 
                                + PyInt_AsLong( PyList_GetItem( raw_list , (i+1)*w+(j+1) ) ) ;
                        r_value >>= 1 ;
                        g_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+(j+1) ) ) 
                                + PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+j ) ) 
                                + PyInt_AsLong( PyList_GetItem( raw_list , (i+1)*w+j ) ) ;
                        g_value /= 3 ;
                        b_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+j ) ) ;
                    }
                }
                else if( j == w - 1 && i != 0 && i != h - 1 ) { // right
                    if( w % 2 == 0 ){
                        if( i % 2 == 0 ) { // r
                            r_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+j ) ) ;
                            g_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+(j-1) ) )
                                    + PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+j ) )
                                    + PyInt_AsLong( PyList_GetItem( raw_list , (i+1)*w+j ) ) ;
                            g_value /= 3 ;
                            b_value = PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+(j-1) ) ) 
                                    + PyInt_AsLong( PyList_GetItem( raw_list , (i+1)*w+(j-1) ) ) ;
                            b_value >>= 1 ;
                        }
                        else{ // G2
                            r_value = PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+j ) ) 
                                    + PyInt_AsLong( PyList_GetItem( raw_list , (i+1)*w+j ) ) ;
                            r_value >>= 1 ;
                            g_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+j ) ) 
                                    + PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+(j-1) ) ) 
                                    + PyInt_AsLong( PyList_GetItem( raw_list , (i+1)*w+(j-1) ) ) ;
                            g_value /= 3 ;
                            b_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+(j-1) ) ) ;
                        }
                    }
                    else{
                        if( i % 2 == 0 ) { // G1
                            r_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+(j-1) ) ) ;
                            g_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+j ) )
                                    + PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+(j-1) ) )
                                    + PyInt_AsLong( PyList_GetItem( raw_list , (i+1)*w+(j-1) ) ) ;
                            g_value /= 3 ;
                            b_value = PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+j ) ) 
                                    + PyInt_AsLong( PyList_GetItem( raw_list , (i+1)*w+j ) ) ;
                            b_value >>= 1 ;
                        }
                        else{ // b
                        r_value = PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+(j-1) ) ) 
                                + PyInt_AsLong( PyList_GetItem( raw_list , (i+1)*w+(j-1) ) ) ;
                        r_value >>= 1 ;
                        g_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+(j-1) ) ) 
                                + PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+j ) ) 
                                + PyInt_AsLong( PyList_GetItem( raw_list , (i+1)*w+j ) ) ;
                        g_value /= 3 ;
                        b_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+j ) ) ;
                        }
                    }
                }
                else if( i == 0 && j == 0 ) { // top-left
                    r_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+(j+1) ) ) ;
                    g_value = PyInt_AsLong( PyList_GetItem( raw_list , (i+1)*w+j ) ) 
                            + PyInt_AsLong( PyList_GetItem( raw_list , i*w+(j+1) ) ) ;
                    g_value >>= 1 ;
                    b_value = PyInt_AsLong( PyList_GetItem( raw_list , (i+1)*w+j ) ) ;
                }
                else if( i == 0 && j == w - 1 ) { // top-right
                    if( j % 2 == 0 ) { // G1
                        r_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+(j-1) ) ) ;
                        g_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+j ) ) 
                                + PyInt_AsLong( PyList_GetItem( raw_list , (i+1)*w+(j-1) ) ) ;
                        g_value >>= 1 ;
                        b_value = PyInt_AsLong( PyList_GetItem( raw_list , (i+1)*w+j ) ) ;
                    }
                    else { // r
                        r_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+j ) ) ;
                        g_value = PyInt_AsLong( PyList_GetItem( raw_list , (i+1)*w+j ) ) 
                                + PyInt_AsLong( PyList_GetItem( raw_list , i*w+(j-1) ) ) ;
                        g_value >>= 1 ;
                        b_value = PyInt_AsLong( PyList_GetItem( raw_list , (i+1)*w+(j-1) ) ) ;
                    }
                }
                else if( i == h - 1 && j == 0 ) { // bottom-left
                    if( i % 2 == 0 ) { // G1
                        r_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+(j+1) ) ) ;
                        g_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+j ) )
                                + PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+(j+1) ) ) ;
                        g_value >>= 1 ;
                        b_value = PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+j ) ) ;
                    }
                    else { // b
                        r_value = PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+(j+1) ) ) ;
                        g_value = PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+j ) )
                                + PyInt_AsLong( PyList_GetItem( raw_list , i*w+(j+1) ) ) ;
                        g_value >>= 1 ;
                        b_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+j ) ) ;
                    }
                }
                else if( i == h - 1 && j == w - 1 ) { // bottom-right
                    if( i % 2 == 0 && j % 2 == 0 ) { // G1 
                        r_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+(j-1) ) ) ;
                        g_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+j ) )
                                + PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+(j-1) ) ) ;
                        g_value >>= 1 ;
                        b_value = PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+j ) ) ;
                    }
                    else if( i % 2 == 0 && j % 2 != 0 ) { // r
                        r_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+j ) ) ;
                        g_value = PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+j ) )
                                + PyInt_AsLong( PyList_GetItem( raw_list , i*w+(j-1) ) ) ;
                        g_value >>= 1 ;
                        b_value = PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+(j-1) ) ) ;
                    }
                    else if( i % 2 != 0 && j % 2 == 0 ) { // b
                        r_value = PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+(j-1) ) ) ;
                        g_value = PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+j ) )
                                + PyInt_AsLong( PyList_GetItem( raw_list , i*w+(j-1) ) ) ;
                        g_value >>= 1 ;
                        b_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+j ) ) ;
                    }
                    else if( i % 2 != 0 && j % 2 != 0 ) { // G2
                        r_value = PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+j ) ) ;
                        g_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+j ) )
                                + PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+(j-1) ) ) ;
                        g_value >>= 1 ;
                        b_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+(j-1) ) ) ;
                    }
                }
            } 
            else { // non-edge
                if( i % 2 == 0 && j % 2 == 0 ) { // G1
                    r_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+(j-1) ) )
                            + PyInt_AsLong( PyList_GetItem( raw_list , i*w+(j+1) ) ) ;
                    r_value >>= 1 ;
                    g_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+j ) )
                            + PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+(j-1) ) )
                            + PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+(j+1) ) )
                            + PyInt_AsLong( PyList_GetItem( raw_list , (i+1)*w+(j-1) ) )
                            + PyInt_AsLong( PyList_GetItem( raw_list , (i+1)*w+(j+1) ) ) ;
                    g_value /= 5 ;
                    b_value = PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+j ) ) ;
                            + PyInt_AsLong( PyList_GetItem( raw_list , (i+1)*w+j ) ) ;
                    b_value >>= 1 ;
                } 
                else if( i % 2 == 0 && j % 2 != 0 ) { // r
                    r_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+j ) ) ;
                    g_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+(j-1) ) )
                            + PyInt_AsLong( PyList_GetItem( raw_list , i*w+(j+1) ) )
                            + PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+j ) )
                            + PyInt_AsLong( PyList_GetItem( raw_list , (i+1)*w+j ) ) ;
                    g_value >>= 2 ;
                    b_value = PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+(j-1) ) )
                            + PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+(j+1) ) )
                            + PyInt_AsLong( PyList_GetItem( raw_list , (i+1)*w+(j-1) ) )
                            + PyInt_AsLong( PyList_GetItem( raw_list , (i+1)*w+(j+1) ) ) ;
                    b_value >>= 2 ;
                } 
                else if( i % 2 != 0 && j % 2 == 0 ) { // b
                    r_value = PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+(j-1) ) )
                            + PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+(j+1) ) )
                            + PyInt_AsLong( PyList_GetItem( raw_list , (i+1)*w+(j-1) ) )
                            + PyInt_AsLong( PyList_GetItem( raw_list , (i+1)*w+(j+1) ) ) ;
                    r_value >>= 2 ;
                    g_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+(j-1) ) )
                            + PyInt_AsLong( PyList_GetItem( raw_list , i*w+(j+1) ) )
                            + PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+j ) )
                            + PyInt_AsLong( PyList_GetItem( raw_list , (i+1)*w+j ) ) ;
                    g_value >>= 2 ;
                    b_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+j ) ) ;
                } 
                else if( i % 2 != 0 && j % 2 != 0 ) { // G2
                    r_value = PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+j ) ) ;
                            + PyInt_AsLong( PyList_GetItem( raw_list , (i+1)*w+j ) ) ;
                    r_value >>= 1 ;
                    g_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+j ) )
                            + PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+(j-1) ) )
                            + PyInt_AsLong( PyList_GetItem( raw_list , (i-1)*w+(j+1) ) )
                            + PyInt_AsLong( PyList_GetItem( raw_list , (i+1)*w+(j-1) ) )
                            + PyInt_AsLong( PyList_GetItem( raw_list , (i+1)*w+(j+1) ) ) ;
                    g_value /= 5 ;
                    b_value = PyInt_AsLong( PyList_GetItem( raw_list , i*w+(j-1) ) )
                            + PyInt_AsLong( PyList_GetItem( raw_list , i*w+(j+1) ) ) ;
                    b_value >>= 1 ;
                }
            } // end of if edge
            // g_value >>= 1 ;
            PyList_SetItem( r_list , i*w+j , PyInt_FromLong( r_value ) ) ;
            PyList_SetItem( g_list , i*w+j , PyInt_FromLong( g_value ) ) ;
            PyList_SetItem( b_list , i*w+j , PyInt_FromLong( b_value ) ) ;
        } // end of for j
    } // end of for i

    return ;

}







