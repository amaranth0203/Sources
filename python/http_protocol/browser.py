#!/usr/bin/evn python
# -*- coding=utf-8 -*-

def send_request( ) :
    import socket , sys
    url = "my.csdn.net"
    host = socket.gethostbyname( url )
    port = 80
    sock = socket.socket( )
    sock.connect( ( host , port ) )
    content = "GET /echofocus HTTP/1.1\r\nHost: my.csdn.net\r\n\r\n" 
    sock.send( content )
    response = sock.recv( 1 ) 
    while response != "" :
        sys.stdout.write( response )
        response = sock.recv( 1 )

if __name__ == '__main__' :
    send_request( )
