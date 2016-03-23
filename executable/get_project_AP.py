#!/usr/bin/env python_
#-*- coding=utf-8 -*-

def test( ) :
    import urllib  
    import urllib2  
    import cookielib  
    pass
    headers = {
        "Host": "192.168.2.125:8080",
        "User-Agent" : "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:44.0) Gecko/20100101 Firefox/44.0" , 
        "Accept": "application/json, text/javascript, */*; q=0.01",
        "Accept-Language": "zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3",
        "Accept-Encoding": "gzip, deflate",
        "Content-Type" : "application/x-www-form-urlencoded; charset=UTF-8" ,
        "X-Requested-With" : "XMLHttpRequest" ,
        "Referer": "http://192.168.2.125:8080/smartsys/",
        "Content-Length" : "39" ,
        "Cookie": "JSESSIONID=C7145D3848D4AE05E66849C92D963A97" ,
        # "Connection": "keep-alive",
    }
    data = {
        "uid" : "qiyunhu" ,
        "password" : "python" ,
        "siteno" : "main" ,
    }
    data = urllib.urlencode(data) 

    url1 = "http://192.168.2.125:8080/smartsys/userlogin.action"
    url2 = "http://192.168.2.125:8080/smartsys/externalInfoProject.action?idx=240&project=PD1516A&hdversion=PD1516AMA&findtype=unifiedProject&unifiedProjectse=&allprojectinfo.unifiedProject=PD1516A&startdate=&enddate=&apkversion=&page=1"
    cj = cookielib.CookieJar()
    opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(cj))
    opener.open(url1, data)
    resp = opener.open(url2)
    return resp.read().strip()

def getAP( html ) :
    from lxml import etree
    import lxml.html
    html = lxml.html.fromstring( html )
    results = html.xpath( "//input[@id='AP']" )
    return results[0].value

def print_and_log_to_file( AP_addr ) :
    import datetime
    log_file_name = "AP.log"
    print AP_addr
    #print datetime.datetime.now( )
    with open( log_file_name , "a+" ) as f :
        f.write( "\r\n" )
        f.write( str( datetime.datetime.now( ) ).split( '.' )[0] ) 
        f.write( "\r\n" )
        f.write( AP_addr )
        f.write( "\r\n" )

if __name__ == "__main__" :
    #print getAP( test( ) ) 
    print_and_log_to_file( getAP( test( ) ) )