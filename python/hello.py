#!/usr/bin/env python
# -*- coding: utf-8 -*-

' a test module '

__author__ = 'Ending'

import sys

def test( ) :
    args = sys.argv
    if len( args ) == 1 :
        print 'Hello , world!'
    elif len( args ) == 2 :
        print 'Hello , %s!' % args[1]
    else :
        print 'Too many arguments!'

if __name__ == '__main__' :
    test( )
#
#
#
#class Hello( object ) :
#    def hello( self , name = 'World' ) :
#        print( 'Hello , %s.' % name )

#def application( environ , start_response ) :
#    start_response( '200 OK__' , [ ( 'Content-Type' , 'text/html' ) ] )
#    return '<h1> Hello , %s! </h1>' % ( environ[ 'PATH_INFO' ][1:] or 'web' )
