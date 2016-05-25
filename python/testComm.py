from multiprocessing import Process , Queue
import os , time , random

def write( q ) :
    for value in range( 11 )[ 1 : ] :
        print 'Put %s to queue...' % value
        q.put( value )
        time.sleep( random.random( ) )

def read( q ) :
    while True :
        value = q.get( True )
        print 'Get %s from queue.' % value

if __name__ == '__main__' :
    q = Queue( )
    pw = Process( target = write , args = ( q , ) )
    pr = Process( target = read , args = ( q , ) )
    pw.start( )
    pr.start( )
    pw.join( )
    pr.terminate( )
