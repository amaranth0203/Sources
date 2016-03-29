#!/usr/bin/env python_
#-*- coding=utf-8 -*-

def get_randCode( *args ) :
    '''
        index relation of eight images :
         1 2 3 4
         5 6 7 8
    '''
    coor = {
        1 : "40%2C44" ,
        2 : "110%2C44" ,
        3 : "180%2C44" , 
        4 : "250%2C44" ,
        5 : "40%2C120" ,
        6 : "110%2C120" ,
        7 : "180%2C120" , 
        8 : "250%2C120" ,
    }
    return "%2C".join( [ coor[i] for i in args ] )

if __name__ == "__main__" :
    import sys
    if len( sys.argv ) < 2 :
        exit( 0 )
    args = []
    for i in sys.argv[1:] :
        args.append( int( i ) )
    print get_randCode( *args )