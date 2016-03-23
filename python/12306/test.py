#!/usr/bin/env python_
#-*- coding=utf-8 -*-

import webbrowser 
import urllib  
import urllib2  
import os  
import sys  
import requests  
import json  
import httplib  
import cookielib  
import time
from BeautifulSoup import BeautifulSoup

def test_output( ) :
    print "[+] function : " + test_output.__name__

def open_htmlcode_in_browser( html ) :
    tmp_file_name = "__temp__.html"
    webbrowser.register('firefox', None, webbrowser.GenericBrowser('firefox'), 1)
    with open( tmp_file_name , "w+" ) as f :
        f.write( html )
    webbrowser.get("firefox").open( "file://" + os.popen( "cygpath -w " + os.path.realpath( tmp_file_name ) ).read() )
    # time.sleep( 2 ) 
    # os.remove( tmp_file_name )

def get_html( url ) :
    html = urllib2.urlopen( url ).read( )
    return html

def test( ) :
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
    urls = [
        "http://192.168.2.125:8080/smartsys/openTop.action                              " ,"http://192.168.2.125:8080/smartsys/externalInfoProject.action?idx=240&project=PD1516A&hdversion=PD1516AMA&findtype=unifiedProject&unifiedProjectse=&allprojectinfo.unifiedProject=PD1516A&startdate=&enddate=&apkversion=&page=1"
    ]
    cj = cookielib.CookieJar()
    opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(cj))
    opener.open(url1, data)
    # resp = opener.open(url2)
    for url in urls :
        resp = opener.open( url )
    return resp.read().strip()

if __name__ == "__main__" :
    print "[+] testing functions..."
    open_htmlcode_in_browser( test( ) )
    # test( )
