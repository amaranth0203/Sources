#!/usr/bin/python

def log( func ) :
    def wrapper( * args , ** kw ) :
        print 'begin'
        return func( * args , ** kw )
    print 'end'
    return

@log
def now( ) :
    print '2014-11-18'

now( )
