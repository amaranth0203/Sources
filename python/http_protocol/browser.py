#!/usr/bin/evn python
# -*- coding=utf-8 -*-

def send_request( ) :
    import socket , sys
    url = "www.baidu.com"
    host = socket.gethostbyname( url )
    port = 80
    sock = socket.socket( )
    sock.connect( ( host , port ) )
    content = "GET / HTTP/1.0\r\n\r\n" 
    sock.send( content )
    response = sock.recv( 1 ) 
    while response != "" :
        sys.stdout.write( response )
        response = sock.recv( 1 )

if __name__ == '__main__' :
    send_request( )