#!/usr/bin/env python_
#-*- coding=utf-8 -*-

def test( keyword ) :
    import urllib  
    import urllib2  
    import cookielib  

    # print urllib.quote( keyword.decode('gbk').encode('utf-8') )
    # exit( )
    url = "http://dict.youdao.com/w/" + urllib.quote( keyword.decode( 'gbk' ).encode( 'utf-8' ) )
    opener = urllib2.build_opener()
    resp = opener.open(url)
    return resp.read().strip()

def get_result( html ) :
    import lxml.html
    html = lxml.html.fromstring( html )
    results = html.xpath( "//div[@class='trans-container']" )
    return results[0].getchildren()[0].getchildren()[0].text_content()

if __name__ == "__main__" :
    import sys
    print " ".join( get_result( test( sys.argv[1] ) ).split( ) )
    # test( sys.argv[1] )






