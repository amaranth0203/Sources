#!/usr/bin/python

'''
    A simple script to switch github identificaton keygen
    between only two user
    by rename the rsa file
    using regex

    Simply using 'ssh -T git@github.com' for testing

                2014-11-22
'''

__author__ = 'Ending'

import os , re

def re_name( file_name ) :
    m = re.match( r'^(.*)(_rsa).?(.*)$' , file_name )
    if m.group( 3 ) == 'pub.bak' :
        os.rename( m.group( 0 ) , m.group( 1 ) + m.group( 2 ) + '.' + 'pub' )
        print '[+] Successfully switched my username for github to : %s ' % m.group( 1 )
    if m.group( 3 ) == '' :
        os.rename( m.group( 0 ) , m.group( 0 ) + '.bak' )
    if m.group( 3 ) == 'pub' :
        os.rename( m.group( 0 ) , m.group( 0 ) + '.bak' )
    if m.group( 3 ) == 'bak' :
        os.rename( m.group( 0 ) , m.group( 1 ) + m.group( 2 ) )
        
 
files = [ x for x in os.listdir( '/root/.ssh' ) if re.match( r'^.*_rsa.*$' , x ) ]

for file_name in files :
    re_name( file_name )

