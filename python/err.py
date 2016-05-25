# err.py

#def foo( s ) :
#    n = int( s )
#    return 10 / n
#
#def bar( s ) :
#    try : 
#        return foo( s ) * 2
#    except StandardError , e :
#        print 'Error!'
##        raise
#
#def main( ) :
#    bar( '0' ) 
#
#main( )

import pdb
s = '0'
n = int( s )
pdb.set_trace( )
print 10 / n 
