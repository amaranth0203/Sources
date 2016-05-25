#!/usr/bin/env python
# -*- coding:utf-8 -*-

import tornado.httpserver
import tornado.ioloop
import slowPrint
import time

def handle_request( request ) :
    message = "Hello World from Tornado Http Server"
    time.sleep(3)
    request.write( "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n%s" % (
        len( message ) , message ) )
    request.finish( )
    

http_server = tornado.httpserver.HTTPServer( handle_request )
http_server.listen( 8080 )
tornado.ioloop.IOLoop.instance().start()
