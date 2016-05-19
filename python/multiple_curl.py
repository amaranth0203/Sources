#!/usr/bin/evn python
# -*- coding=utf-8 -*-

def send_request( ) :
    import socket , sys
    url = "localhost"
    host = socket.gethostbyname( url )
    port = 80
    sock = socket.socket( )
    sock.connect( ( host , port ) )
    content = "GET / HTTP/1.0\r\n\r\n" 
    sock.send( content )
    
if __name__ == "__main__" :
    import threading , time

    threads = []
    for cnt in range( 1 , 10 ) :
        for i in range( 1 , 3255 ) :
            t = threading.Thread( target = send_request )
            t.daemon = True
            t.start( )
            threads.append( t )
        for t in threads :
            t.join( )
        time.sleep( 1 )
        print cnt 
