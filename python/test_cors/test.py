#!/usr/bin/evn python
# -*- coding=utf-8 -*-

def send_request( ) :
    import socket , sys
    url = "localhost"
    host = socket.gethostbyname( url )
    port = 80
    sock = socket.socket( )
    sock.connect( ( host , port ) )
    content = "POST /handle_request.php HTTP/1.1\r\n"
    content += "Host: localhost\r\n"
    content += "User-Agent: python command line\r\n"
    content += "Content-Type: application/x-www-form-urlencoded\r\n"
    content += "X-Requested-With: XMLHttpRequest\r\n"
    content += "Content-Length: 20\r\n"
    content += "\r\n"
    sock.send( content )
    content = "&action=get_messages"
    sock.send( content )
    header = ""
    while True :
        response = sock.recv( 1 )
        header += response
        if header[-4:] == "\r\n\r\n" :
            print header
            break
    start = header.rfind( 'Content-Length' )
    length = header[ start : len( header ) ].find( '\r\n' )
    content_length = int( header[ start : start + length ].split( ':' )[1].strip( ) )
    # print content_length
    raw = []
    while True :
        raw.append( sock.recv( content_length ) )
        if len( "".join( raw ) ) == content_length :
            break
    print "".join( raw )
    # print len( "".join( raw ) )
    # print '[+] write page to file "out.html"...'
    # import zlib
    # with open( "out.html" , "w" ) as f : 
        # f.write( zlib.decompress( "".join( raw ) , 16 + zlib.MAX_WBITS ) )
    # print '    Done'

if __name__ == '__main__' :
    send_request( )

