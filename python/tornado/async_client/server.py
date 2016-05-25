# -*- coding:utf-8 -*-
from socket import *
import time

def main( ) :
    HOST = ""
    PORT = 3316
    BUFSIZE = 1024
    ADDR = ( HOST , PORT )
    tcpSerSock = socket( AF_INET , SOCK_STREAM )
    tcpSerSock.bind( ADDR )
    tcpSerSock.listen( 5 )
    print "YoSQL bind port %d , starting ..." % PORT
    while 1 :
        print 'waiting for connection ...'
        tcpCliSock , addr = tcpSerSock.accept( )
        print '...connected from : ' , addr
        while 1 :
            try :
                data = tcpCliSock.recv( BUFSIZE )
                if not data :
                    break 
                print 'data = %s' % data
                i = data.find( '[' )
                j = data.find( ']' )
                if i != -1 and j != -1 :
                    sFuncId = data[i+1:j]
                    message = data[j+1:]
                    time.sleep( 2 )
                    SendToListener( "[%s] echo" % sFuncId )
            except Exception , e :
                print e
                break
        tcpCliSock.close( )
    tcpSerSock.close( )

def SendToListener( message ) :
    listenerSock = socket( AF_INET , SOCK_STREAM ) 
    listenerSock.connect( ('localhost' , 7800 ) )
    listenerSock.send( message )
    listenerSock.close( )
    print 'send to listener : %s ' % message
if __name__ == '__main__' :
    main( )
