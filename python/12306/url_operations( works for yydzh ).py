#!/usr/bin/env python_
#-*- coding=utf-8 -*-

import urllib  
import urllib2  
import sys  
import requests  
import json  
import httplib  
import cookielib  
from BeautifulSoup import BeautifulSoup

def test_output( ) :
    print "[+] function : " + test_output.__name__

def open_htmlcode_in_browser( html ) :
    import webbrowser 
    import os  
    import time
    # print html
    tmp_file_name = "__temp__.html"
    webbrowser.register('firefox', None, webbrowser.GenericBrowser('firefox'), 1)
    #print webbrowser._tryorder
    with open( tmp_file_name , "w+" ) as f :
        f.write( html )
    webbrowser.get("firefox").open( "file://" + os.popen( "cygpath -w " + os.path.realpath( tmp_file_name ) ).read() )
    # time.sleep( 2 ) 
    os.remove( tmp_file_name )

def get_html( url ) :
    html = urllib2.urlopen( url ).read( )
    return html

def test( ) :
    pass
    headers = {
        "Host": "yydzh.com",
        "User-Agent" : "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:42.0) Gecko/20100101 Firefox/42.0" , 
        "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8",
        "Accept-Encoding": "gzip, deflate",
        "Accept-Language": "zh-TW,zh;q=0.8,en-US;q=0.5,en;q=0.3",
        "Referer": "http://yydzh.com/login.php",
        "Cookie": "fd160_lastpos=other; fd160_ol_offset=4753; CNZZDATA175294=cnzz_eid%3D1221809152-1448251562-%26ntime%3D1449653214; fd160_threadlog=%2C26%2C; fd160_readlog=%2C1834319%2C1834460%2C1834509%2C1834243%2C1834492%2C1834246%2C1834483%2C1833195%2C1834265%2C1834385%2C; fd160_lastvisit=62%091449654232%09%2Flogin.php%3F" ,
        "Connection": "keep-alive",
    }
    data = {
        "forward":"",
        "jumpurl":urllib.unquote("http%3A%2F%2Fyydzh.com%2F"),
        "step":"2",
        # "pwuser":urllib.unquote("%BD%F1%CD%ED%B3%D4%C5%A3%C8%E2") ,
        "pwuser":urllib.unquote("465056") ,
        "pwpwd":urllib.unquote("FORyydzh1212"),
        "question":"-1",
        "customquest":urllib.unquote("%CC%EC%BF%D5%CA%C7%B0%D7%B5%C4"),
        "answer":urllib.unquote("%B5%AB%D4%C6%CA%C7%BA%DA%B5%C4"),
        "hideid":"0",
        "cktime":"0",
    }
    data = urllib.urlencode(data) 

    url1 = "http://yydzh.com/login.php"
    # r = requests.post(url1, data=json.dumps(payload), headers=headers)
    # print data
    # req1 = urllib2.Request( url1 , data , headers) 
    # response1 = urllib2.urlopen( req1 )
    # cookie = response1.headers.get( 'Set-Cookie' )
    # print type(response1.read())

    url2 = "http://yydzh.com/"
    # req2 = urllib2.Request( url2 )
    # req2.add_header( 'cookie' , cookie )
    # response2 = urllib2.urlopen( req2 )
    # try:
        # return response1.read()
    # except urllib2.HTTPError, e:
        # print e.code
        # print e.msg
        # print e.headers
        # return e.fp.read()

    # url3 = "http://yydzh.com/hack/announce/announce.js?76367"
    # url4 = "http://yydzh.com/new.php?action=article&fidin=71_34_38_20_2_197_3_4_77_5_66_68_17_70_59_32_62_26_72_44_6_7_78_37_33_42_51_52_198_200_199_102&pre=1&num=8&length=35&order=2&style=2"
    # url5 = "http://yydzh.com/new.php?action=article&fidin=71_34_38_20_2_197_3_4_77_5_66_68_17_70_59_32_62_26_72_44_6_7_78_37_33_42_51_52_198_200_199_102&pre=1&num=8&length=37&order=1&style=2"
    # url6 = "http://yydzh.com/new.php?action=article&fidin=71_34_38_20_2_197_3_4_5_66_68_17_70_59_32_62_26_72_44_6_7&pre=1&num=8&length=35&order=3&style=2&timer=1"
    url7 = "http://yydzh.com/profile.php?action=show&uid=465056"
    url8 = "http://yydzh.com/hack.php?H_name=avatar&index=show&uid=465056"
    # session = requests.session( )
    # session.encoding = "utf-8"
    # r = session.post( url1 , data=data )
    # r_encoding = r.encoding
    # r = session.get( url2 )
    # r = session.get( url3 )
    # r = session.get( url4 )
    # r = session.get( url5 )
    # r = session.get( url6 )
    # r = session.get( url7 )
    # r = session.get( url8 )
    # print r.headers
    # return r.text.encode(r_encoding)
    cj = cookielib.CookieJar()
    opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(cj))
    opener.open(url1, data)
    resp = opener.open(url2)
    resp = opener.open(url8)
    # print resp.read()
    return resp.read().strip()

if __name__ == "__main__" :
    print "[+] testing functions..."
    # print get_html( "http://www.baidu.com" )
    open_htmlcode_in_browser( test( ) )
    # test( )
