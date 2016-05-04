#!/usr/bin/evn python
# -*- coding=utf-8 -*-

def handle_request( ) :
    import socket
    sock = socket.socket( )
    host = "localhost"
    port = 80
    sock.bind( ( host , port ) ) ;
    sock.listen( 5 )

    while True :
        clientSock , addr = sock.accept( )
        print( "got a connection from {}".format( addr ) )
        header = ""
        while True :
            response = clientSock.recv( 1 )
            header += response
            if header[-4:] == "\r\n\r\n" :
                print header
                break
        content = ""
        content += "HTTP/1.0 200 OK\r\n" 
        content += "Content-Type: text/html\r\n" 
        content += "Set-cookie: name=wassup\r\n"
        content +=  "\r\n" 
        content += '<h1>hello<br/>hello</h1>' 
        clientSock.send( content ) 
        clientSock.close( )

if __name__ == '__main__' :
    handle_request( )