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
        "Host" : "kyfw.12306.cn" ,
        "User-Agent" : "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:45.0) Gecko/20100101 Firefox/45.0" ,
        "Accept" : "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8" ,
        "Accept-Language" : "zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3" ,
        "Accept-Encoding" : "gzip, deflate, br" ,
        "Referer" : "https://kyfw.12306.cn/otn/index/initMy12306" ,
        "Cookie" : "JSESSIONID=0A01E81BFC374B1186CB2B99C4E35C30FD3A7AAE07; __NRF=180D3D788B8C0D10608BA75A920E7AC0; BIGipServerotn=468189450.64545.0000; current_captcha_type=Z" ,
        "Connection" : "keep-alive" ,
        "Cache-Control" : "max-age=0" ,
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
    print '[+] 1 : \n{} \n{} \n{}'.format( resp.getcode( ) , resp.url , resp.geturl( ) )
    # print resp.headers.getheaders('set-cookie')
    cookie = resp.headers.getheaders('set-cookie')
    cookie = '; '.join( cookie )
    # print cookie
    # exit()

    # 2 获取图片验证码 - start
    headers = {
        "Host" : "kyfw.12306.cn" ,
        "User-Agent" : "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:45.0) Gecko/20100101 Firefox/45.0" ,
        "Accept" : "image/png,image/*;q=0.8,*/*;q=0.5" ,
        "Accept-Language" : "zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3" ,
        "Accept-Encoding" : "gzip, deflate, br" ,
        "Referer" : "https://kyfw.12306.cn/otn/login/init" ,
        "Cookie" : cookie ,
        "Connection" : "keep-alive" ,
        "Cache-Control" : "max-age=0" ,
    }
    # headers['Cookie'] = cookie
    httplib.HTTPSConnection.connect = connect
    cj = cookielib.CookieJar()
    opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(cj))
    resp = opener.open("https://kyfw.12306.cn/otn/passcodeNew/getPassCodeNew?module=login&rand=sjrand&0.4838378324749877")
    print '[+] 2 : \n{} \n{} \n{}'.format( resp.getcode( ) , resp.url , resp.geturl( ) )
    with open('Verification.jpg','wb') as f :
        f.write(resp.read())
    # 2 获取图片验证码 - end

    # 手动输入选项，生成randCode - start
    import coordinate
    coor = raw_input( 'coor?' )
    args = [ int( i ) for i in coor.split( ) ]
    randCode = coordinate.get_randCode( *args )
    # 手动输入选项，生成randCode - end

    # 3 提交验证码 - start
    headers = {
        # "Host" : "kyfw.12306.cn" ,
        "User-Agent" : "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:45.0) Gecko/20100101 Firefox/45.0" ,
        "Accept" : "*/*" ,
        # "Accept-Language" : "zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3" ,
        "Accept-Encoding" : "gzip, deflate, br" ,
        "Content-Type" : "application/x-www-form-urlencoded; charset=UTF-8" ,
        "X-Requested-With" : "XMLHttpRequest" ,
        "Referer" : "https://kyfw.12306.cn/otn/login/init" ,
        # "Content-Length" : "30" ,
        "Cookie" : cookie ,
        # "Connection" : "keep-alive" ,
    }
    data = {
        "randCode" : randCode ,
        "rand" : "sjrand" ,
    }
    data = urllib.urlencode(data) 
    httplib.HTTPSConnection.connect = connect
    cj = cookielib.CookieJar()
    opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(cj))
    resp = opener.open("https://kyfw.12306.cn/otn/passcodeNew/checkRandCodeAnsyn", data)
    print '[+] 3 : \n{} \n{} \n{}'.format( resp.getcode( ) , resp.url , resp.geturl( ) )
    print resp.read()
    # print resp.headers.getheaders('set-cookie')
    # resp = opener.open("https://kyfw.12306.cn/otn/passcodeNew/checkRandCodeAnsyn")
    # 3 提交验证码 - end
    exit()

    # 4 使用提交的验证码登陆 - start
    headers = {
        "Host" : "kyfw.12306.cn" ,
        "User-Agent" : "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:45.0) Gecko/20100101 Firefox/45.0" ,
        "Accept" : "*/*" ,
        "Accept-Language" : "zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3" ,
        "Accept-Encoding" : "gzip, deflate, br" ,
        "Content-Type" : "application/x-www-form-urlencoded; charset=UTF-8" ,
        "X-Requested-With" : "XMLHttpRequest" ,
        "Referer" : "https://kyfw.12306.cn/otn/login/init" ,
        "Content-Length" : "88" ,
        "Cookie" : "JSESSIONID=0A01E81BFC3863DA6E23D55FB9F8FA26E8908D64A4; __NRF=180D3D788B8C0D10608BA75A920E7AC0; BIGipServerotn=468189450.64545.0000; current_captcha_type=Z" ,
        "Connection" : "keep-alive" ,
    }
    data = {
        "loginUserDTO.user_name" : "845687884%40qq.com" ,
        "userDTO.password" : "Echo920805" ,
        "randCode" : randCode ,
    }
    data = urllib.urlencode(data) 
    httplib.HTTPSConnection.connect = connect
    cj = cookielib.CookieJar()
    opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(cj))
    resp = opener.open("https://kyfw.12306.cn/otn/login/loginAysnSuggest")
    print '[+] 4 : \n{} \n{} \n{}'.format( resp.getcode( ) , resp.url , resp.geturl( ) )
    # resp = opener.open("https://kyfw.12306.cn/otn/login/loginAysnSuggest")
    # 4 使用提交的验证码登陆 - end

    # 5 json check - start
    headers = {
        "Host" : "kyfw.12306.cn" ,
        "User-Agent" : "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:45.0) Gecko/20100101 Firefox/45.0" ,
        "Accept" : "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8" ,
        "Accept-Language" : "zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3" ,
        "Accept-Encoding" : "gzip, deflate, br" ,
        "Referer" : "https://kyfw.12306.cn/otn/login/init" ,
        "Cookie" : "JSESSIONID=0A01E81BFC3863DA6E23D55FB9F8FA26E8908D64A4; __NRF=180D3D788B8C0D10608BA75A920E7AC0; BIGipServerotn=468189450.64545.0000; current_captcha_type=Z" ,
        "Connection" : "keep-alive" ,
    }
    data = {
        "_json_att" : "" ,
    }
    data = urllib.urlencode(data) 
    httplib.HTTPSConnection.connect = connect
    cj = cookielib.CookieJar()
    opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(cj))
    resp = opener.open("https://kyfw.12306.cn/otn/login/userLogin", data)
    print '[+] 5 : \n{} \n{} \n{}'.format( resp.getcode( ) , resp.url , resp.geturl( ) )
    # resp = opener.open("https://kyfw.12306.cn/otn/login/userLogin")
    # 5 json check - end

    # 6 获取登陆后的页面 - start
    headers = {
        "Host" : "kyfw.12306.cn" ,
        "User-Agent" : "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:45.0) Gecko/20100101 Firefox/45.0" ,
        "Accept" : "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8" ,
        "Accept-Language" : "zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3" ,
        "Accept-Encoding" : "gzip, deflate, br" ,
        "Referer" : "https://kyfw.12306.cn/otn/login/init" ,
        "Cookie" : "JSESSIONID=0A01E81BFC3863DA6E23D55FB9F8FA26E8908D64A4; __NRF=180D3D788B8C0D10608BA75A920E7AC0; BIGipServerotn=468189450.64545.0000; current_captcha_type=Z" ,
        "Connection" : "keep-alive" ,
    }
    # data = {
        # "loginUserDTO.user_name" : "845687884%40qq.com" ,
        # "userDTO.password" : "Echo920805" ,
        # "randCode" : randCode ,
    # }
    # data = urllib.urlencode(data) 
    httplib.HTTPSConnection.connect = connect
    cj = cookielib.CookieJar()
    opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(cj))
    # opener.open(url1, data)
    resp = opener.open("https://kyfw.12306.cn/otn/index/initMy12306")
    # 6 获取登陆后的页面 - end
    return resp.read().strip()



    # return resp.read().strip()

if __name__ == "__main__" :
    open_htmlcode_in_browser( test( ) )
    # test( )

