#!/usr/bin/evn python
# -*- coding=utf-8 -*-

def send_request( ) :
    import socket , sys
    url = "yydzh.com"
    host = socket.gethostbyname( url )
    host = "59.56.110.160"
    port = 80
    sock = socket.socket( )
    sock.connect( ( host , port ) )
    content = "POST /login.php? HTTP/1.1\r\n"
    content += "Host: www.yydzh.com\r\n"
    content += "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:45.0) Gecko/20100101 Firefox/45.0\r\n"
    content += "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
    content += "Accept-Language: zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3\r\n"
    content += "Accept-Encoding: gzip, deflate\r\n"
    content += "Referer: http://yydzh.com/login.php\r\n"
    content += "Cookie: fd160_lastpos=other; CNZZDATA175294=cnzz_eid%3D2094742159-1458002100-%26ntime%3D1462005917; fd160_threadlog=%2C51%2C102%2C72%2C10%2C26%2C; fd160_ol_offset=24153; fd160_readlog=%2C1877047%2C1877064%2C1877113%2C1877004%2C1877105%2C1877101%2C1877130%2C1877117%2C; fd160_ipfrom=9f917242af1f1b4ad730110017dc0af2%09%B9%E3%B6%AB%CA%A1%C9%EE%DB%DA%CA%D0+%C1%AA%CD%A8; fd160_lastvisit=2%091462006638%09%2Flogin.php%3F\r\n"
    content += "Content-Type: application/x-www-form-urlencoded\r\n"
    content += "Content-Length: 224\r\n"
    content += "\r\n"
    sock.send( content )
    content = "forward=&jumpurl=http%3A%2F%2Fyydzh.com%2F&step=2&pwuser=%BD%F1%CD%ED%B3%D4%C5%A3%C8%E2&pwpwd=FORyydzh1212&question=-1&customquest=%CC%EC%BF%D5%CA%C7%B0%D7%B5%C4&answer=%B5%AB%D4%C6%CA%C7%BA%DA%B5%C4&hideid=0&cktime=31536000"
    sock.send( content )
    content = "GET / HTTP/1.1\r\n"
    content += "Host: www.yydzh.com\r\n"
    content += "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:45.0) Gecko/20100101 Firefox/45.0\r\n"
    content += "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
    content += "Accept-Language: zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3\r\n"
    content += "Accept-Encoding: gzip, deflate\r\n"
    content += "Referer: http://yydzh.com/login.php\r\n"
    content += "Cookie: fd160_lastpos=other; CNZZDATA175294=cnzz_eid%3D2094742159-1458002100-%26ntime%3D1462005917; fd160_threadlog=%2C51%2C102%2C72%2C10%2C26%2C; fd160_ol_offset=24153; fd160_readlog=%2C1877047%2C1877064%2C1877113%2C1877004%2C1877105%2C1877101%2C1877130%2C1877117%2C; fd160_ipfrom=9f917242af1f1b4ad730110017dc0af2%09%B9%E3%B6%AB%CA%A1%C9%EE%DB%DA%CA%D0+%C1%AA%CD%A8; fd160_lastvisit=2%091462006638%09%2Flogin.php%3F\r\n"
    content += "\r\n"
    sock.send( content )
    header = ""
    while True :
        response = sock.recv( 1 )
        header += response
        if header[-4:] == "\r\n\r\n" :
            print header
            break
    while True :
        response = sock.recv( 1 )
        header += response
        if header[-4:] == "\r\n\r\n" :
            print header
            break
    start = header.rfind( 'Content-Length' )
    length = header[ start : len( header ) ].find( '\r\n' )
    content_length = int( header[ start : start + length ].split( ':' )[1].strip( ) )
    print content_length
    raw = []
    while True :
        raw.append( sock.recv( content_length ) )
        if len( "".join( raw ) ) == content_length :
            break
    print len( "".join( raw ) )
    import zlib
    print zlib.decompress( "".join( raw ) , 16 + zlib.MAX_WBITS )
    
    # sock.send( content )
    # response = sock.recv( 1024 ) 
    # while response != "" :
        # sys.stdout.write( response )
        # response = sock.recv( 1024 )
    # content = "GET / HTTP/1.1\r\n"
    # content += "Host: www.yydzh.com\r\n"
    # content += "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:45.0) Gecko/20100101 Firefox/45.0\r\n"
    # content += "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
    # content += "Accept-Language: zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3\r\n"
    # content += "Accept-Encoding: gzip, deflate\r\n"
    # content += "Referer: http://yydzh.com/login.php\r\n"
    # content += "Cookie: fd160_lastpos=index; CNZZDATA175294=cnzz_eid%3D2094742159-1458002100-%26ntime%3D1462005917; fd160_threadlog=%2C51%2C102%2C72%2C10%2C26%2C; fd160_ol_offset=1358; fd160_readlog=%2C1877047%2C1877064%2C1877113%2C1877004%2C1877105%2C1877101%2C1877130%2C1877117%2C; fd160_ipfrom=9f917242af1f1b4ad730110017dc0af2%09%B9%E3%B6%AB%CA%A1%C9%EE%DB%DA%CA%D0+%C1%AA%CD%A8; fd160_lastvisit=2%091462184724%09%2Flogin.php%3F\r\n"
    # content += "\r\n"
    # sock.send( content )
    # header = ""
    # while True :
        # response = sock.recv( 1 )
        # header += response
        # if header[-4:] == "\r\n\r\n" :
            # print header
            # break

if __name__ == '__main__' :
    send_request( )

