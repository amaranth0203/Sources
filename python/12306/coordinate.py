#!/usr/bin/env python_
#-*- coding=utf-8 -*-

def get_randCode( *args ) :
    '''
        index relation of eight images :
         1 2 3 4
         5 6 7 8
    '''
    coor = {
        1 : "40,44" ,
        2 : "110,44" ,
        3 : "180,44" , 
        4 : "250,44" ,
        5 : "40,120" ,
        6 : "110,120" ,
        7 : "180,120" , 
        8 : "250,120" ,
    }
    return ",".join( [ coor[i] for i in args ] )

if __name__ == "__main__" :
    import sys
    if len( sys.argv ) < 2 :
        exit( 0 )
    args = []
    for i in sys.argv[1:] :
        args.append( int( i ) )
    print get_randCode( *args )