#!/usr/bin/env python

import sys
sys.setrecursionlimit( 1000000 )

f = [
[0,0,0,0,0,0,0,0,0],
[0,0,0,7,0,2,0,0,0],
[4,1,0,0,0,0,0,0,0],
[0,0,7,0,0,0,9,0,2],
[8,0,0,0,1,0,0,0,5],
[0,0,0,0,0,0,7,0,0],
[0,0,0,0,4,0,0,1,0],
[0,6,0,0,0,0,0,3,0],
[0,0,2,9,0,0,0,0,0],
]
a = []
used = [ 0 , 0 , 0 , 0 , 0 ]
def test( a ) :
    if len( a ) == 4 :
        print a
        return 
    else :
        for i in range( 1 , 5 ) :
            if( used[i] == 0 ) :
                a.append( i )
                used[i] = 1
                test( a )
                a.pop( )
                used[i] = 0
        
test( a )




def display( f ) :
    for i in range( 0 , 8 ) :
        for j in range( 0 , 8 ) :
            sys.stdout.write( '%s ' % str( f[i][j] ) )
        print ' '

