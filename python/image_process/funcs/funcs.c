#include <Python.h>
#include "funcs.h"

static PyObject* FuncsError ;

static PyObject*
funcs_system( PyObject* self , PyObject* args ) {
    const char* command ;
    int sts ;

    if( !PyArg_ParseTuple( args , "s" , &command ) )
        return NULL ;
    sts = system( command ) ;
    if( sts < 0 ) {
        PyErr_SetString( FuncsError , "System command failed" ) ;
        return NULL ;
    }
    return Py_BuildValue( "i" , sts ) ;
}

static PyObject*
funcs_str_to_int_array( PyObject* self , PyObject* args ) { 
    const unsigned char* raw ;
    int raw_size ;
    PyObject* raw_obj ;
    Py_buffer raw_buffer ;
    PyObject* raw_list ;

    if( !PyArg_ParseTuple( args , "OiO" , &raw_obj , &raw_size , &raw_list ) )
        return NULL ;
    PyObject_GetBuffer(raw_obj, &raw_buffer, PyBUF_SIMPLE) ;
    raw = (char*)raw_buffer.buf ;

    str_to_int_array( raw , raw_size , raw_list ) ;
    Py_RETURN_NONE ;
}

static PyObject*
funcs_demosaic( PyObject* self , PyObject* args ) { 
    PyObject* raw_list ;
    PyObject* r_list ;
    PyObject* g_list ;
    PyObject* b_list ;
    int w , h ;
    

    if( !PyArg_ParseTuple( args , "OOOOii" , &raw_list , &r_list , &g_list , &b_list , &w , &h ) )
        return NULL ;

    demosaic( raw_list , r_list , g_list , b_list , w , h ) ;
    Py_RETURN_NONE ;
}

static PyObject*
funcs_brighten( PyObject* self , PyObject* args ) { 
    PyObject* r_list ;
    PyObject* g_list ;
    PyObject* b_list ;    
    float strength ;

    if( !PyArg_ParseTuple( args , "OOOf" , &r_list , &g_list , &b_list , &strength ) )
        return NULL ;

    brighten( r_list , g_list , b_list , strength ) ;
    Py_RETURN_NONE ;
}

static PyMethodDef FuncsMethods[] = {
    { "system" , funcs_system , METH_VARARGS , "Execute a shell command." } ,
    { "str_to_int_array" , funcs_str_to_int_array , METH_VARARGS , "convert bytes from str to int array" } ,
    { "demosaic" , funcs_demosaic , METH_VARARGS , "Do demosaic" } ,
    { "brighten" , funcs_brighten , METH_VARARGS , "Do brighten" } ,
    { NULL , NULL , 0 , NULL }
} ;

PyMODINIT_FUNC
initfuncs( void ) {
    PyObject *m ;

    m = Py_InitModule( "funcs" , FuncsMethods ) ;
    if( m == NULL )
        return ;

    FuncsError = PyErr_NewException( "funcs.error" , NULL , NULL ) ;
    Py_INCREF( FuncsError ) ;
    PyModule_AddObject( m , "error" , FuncsError ) ;
}

