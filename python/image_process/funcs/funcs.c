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
    PyObject* list_int ;

    if( !PyArg_ParseTuple( args , "OiO" , &raw_obj , &raw_size , &list_int ) )
        return NULL ;
    PyObject_GetBuffer(raw_obj, &raw_buffer, PyBUF_SIMPLE) ;
    raw = (char*)raw_buffer.buf ;

    str_to_int_array( raw , raw_size , list_int ) ;
    Py_RETURN_NONE ;
}

static PyMethodDef FuncsMethods[] = {
    { "system" , funcs_system , METH_VARARGS , "Execute a shell command." } ,
    { "str_to_int_array" , funcs_str_to_int_array , METH_VARARGS , "convert bytes from str to int array" } ,
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

