#!/usr/bin/evn python
# -*- coding=utf-8 -*-

def send_request( ) :
    import socket , sys
    url = "www.yydzh.com"
    host = socket.gethostbyname( url )
    port = 80
    sock = socket.socket( )
    sock.connect( ( host , port ) )
    content = "POST /login.php? HTTP/1.1\r\n"
    content += "Host: www.yydzh.com\r\n"
    content += "User-Agent: python command line\r\n"
    content += "Content-Type: application/x-www-form-urlencoded\r\n"
    content += "Content-Length: 224\r\n"
    content += "\r\n"
    sock.send( content )
    content = "forward=&jumpurl=http%3A%2F%2Fyydzh.com%2F&step=2&pwuser=%BD%F1%CD%ED%B3%D4%C5%A3%C8%E2&pwpwd=FORyydzh1212&question=-1&customquest=%CC%EC%BF%D5%CA%C7%B0%D7%B5%C4&answer=%B5%AB%D4%C6%CA%C7%BA%DA%B5%C4&hideid=0&cktime=31536000"
    sock.send( content )
    header = ""
    while True :
        response = sock.recv( 1 )
        header += response
        if header[-4:] == "\r\n\r\n" :
            # print header
            break
    cookies = [] 
    for line in header.split( '\r\n' ) :
        if 'set-cookie' in line.lower( ) :
            cookies.append( line )
    target_cookie = []
    for cookie in cookies :
        for field in cookie[12:].split( ';' ) :
            if  '=' in field and\
                'expires=' not in field.lower( ) and\
                'path=' not in field.lower( ) and\
                'domain=' not in field.lower( ) :
                target_cookie.append( field )
    print '[+] get cookie : '
    print '\n'.join( target_cookie )


    content = "GET / HTTP/1.1\r\n"
    content += "Host: www.yydzh.com\r\n"
    content += "User-Agent: python command line\r\n"
    content += "Accept-Encoding: gzip, deflate\r\n"
    content += "Cookie: " + '; '.join( target_cookie ) + "\r\n"
    content += "\r\n"
    sock.send( content )
    header = ""
    while True :
        response = sock.recv( 1 )
        header += response
        if header[-4:] == "\r\n\r\n" :
            # print header
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
    # print len( "".join( raw ) )
    print '[+] write page to file "out.html"...'
    import zlib
    with open( "out.html" , "w" ) as f : 
        f.write( zlib.decompress( "".join( raw ) , 16 + zlib.MAX_WBITS ) )
    print '    Done'

if __name__ == '__main__' :
    send_request( )

