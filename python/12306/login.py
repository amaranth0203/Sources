#!/usr/bin/env python_
#-*- coding=utf-8 -*-

def open_htmlcode_in_browser( html ) :
    import os , time , webbrowser
    tmp_file_name = "__temp__.html"
    webbrowser.register('firefox', None, webbrowser.GenericBrowser('firefox'), 1)
    with open( tmp_file_name , "w+" ) as f :
        f.write( html )
    webbrowser.get("firefox").open( "file://" + os.popen( "cygpath -w " + os.path.realpath( tmp_file_name ) ).read() )
    time.sleep( 2 ) 
    # os.remove( tmp_file_name )

def connect(self):
    import httplib
    import socket
    import ssl
    import urllib2

    "Connect to a host on a given (SSL) port."

    sock = socket.create_connection((self.host, self.port),
                                    self.timeout, self.source_address)
    if self._tunnel_host:
        self.sock = sock
        self._tunnel()

    self.sock = ssl.wrap_socket(sock, self.key_file, self.cert_file, ssl_version=ssl.PROTOCOL_TLSv1)

def test( ) :
    import httplib
    import urllib  
    import urllib2  
    import cookielib  
    import ssl
    pass
    headers = {
        "Host": "kyfw.12306.cn",
        "User-Agent" : "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:45.0) Gecko/20100101 Firefox/45.0" , 
        "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8",
        "Accept-Language": "zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3",
        "Accept-Encoding": "gzip, deflate, br",
        "Cookie": "JSESSIONID=0A01E81A98656CDBD9D1AD5F8145A2C3DCB6ACF5AC; __NRF=236AFC49CC2A32C859AE16A8CB6E6B38; BIGipServerotn=451412234.38945.0000; current_captcha_type=Z" ,
        "Connection": "keep-alive",
    }
    # data = {
        # "uid" : "qiyunhu" ,
        # "password" : "python" ,
        # "siteno" : "main" ,
    # }
    # data = urllib.urlencode(data) 
    httplib.HTTPSConnection.connect = connect
    cj = cookielib.CookieJar()
    opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(cj))
    # opener.open(url1, data)
    resp = opener.open("https://kyfw.12306.cn/otn/login/init")


    headers = {
        "Host": "kyfw.12306.cn",
        "User-Agent" : "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:45.0) Gecko/20100101 Firefox/45.0" , 
        "Accept": "image/png,image/*;q=0.8,*/*;q=0.5",
        "Accept-Language": "zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3",
        "Accept-Encoding": "gzip, deflate, br",
        "Referer" : "https://kyfw.12306.cn/otn/login/init" , 
        "Cookie": "JSESSIONID=0A02F02AC423C3EC66D310E21432E15325C108D71B; __NRF=560249D67A1CFF6E4D13819EA38A5898; BIGipServerotn=720372234.50210.0000; current_captcha_type=Z" ,
        "Connection": "keep-alive",
        "Cache-Control" : "max-age=0" ,
    }
    httplib.HTTPSConnection.connect = connect
    cj = cookielib.CookieJar()
    opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(cj))
    resp = opener.open("https://kyfw.12306.cn/otn/passcodeNew/getPassCodeNew?module=login&rand=sjrand&0.4838378324749879")
    with open('Verification.jpg','wb') as f :
        f.write(resp.read())



    # return resp.read().strip()

if __name__ == "__main__" :
    # open_htmlcode_in_browser( test( ) )
    test( )

