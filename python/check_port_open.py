#!/usr/bin/env python_
#-*- coding=utf-8 -*-

def lexec( cmd , output = True , tips = True ) :
    import os
    if tips :
        print( "[!] " + str( cmd ) + "\n" )
    rc = os.popen( cmd ).read( )
    if output :
        print( rc )
    return rc

def check_port( ( addr , port ) ) :
    import socket
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.settimeout(0.2)
    try :
        s.connect((addr, port))
        print addr + ' pass'
    except :
        pass

if __name__ == "__main__" :
    #  import multiprocessing
    #  addrs = []
    #  for j in range( 1 , 255 ) :
        #  for i in range( 1 , 255 ) :
            #  addrs.append( [ "113.118.{}.{}".format( j , i ) , 2333 ] )
    #  addrs = tuple( addrs )
    #  p = multiprocessing.Pool( 50 )
    #  p.map( check_port , addrs )
    #  exit( )



    import Queue
    import threading
    threads = []
    for j in range( 1 , 255 ) :
        for i in range( 1 , 255 ) :
            t = threading.Thread( target = check_port , args = ( ( "113.118.{}.{}".format( j , i ) , 2333 ) , ) )
            t.daemon = True
            t.start( )
            threads.append( t )

    for t in threads :
        t.join( )