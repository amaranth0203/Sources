import threading
import time
from socket import *
lCallback = {}
iFuncId = 0

def StartListener( ) :
    global iFuncId
    global lCallback
    HOST = ""
    PORT = 7800
    BUFSIZE = 1024
    ADDR = ( HOST , PORT )
    tcpSerSock = socket( AF_INET , SOCK_STREAM )
    tcpSerSock.bind( ADDR )
    tcpSerSock.listen( 5 )
    print "Listener bind port %d , starting ... " % PORT 
    while 1 :
        print "waiting for connection ..."
        tcpCliSock , addr = tcpSerSock.accept( )
        print "...connected from : " , addr
        while 1 :
            try :
                data = tcpCliSock.recv( BUFSIZE )
                if not data :
                    break 
                print 'data = %s' % data
                i = data.find( '[' )
                j = data.find( ']' )
                if i != -1 and j != -1 :
                    iFuncId = int( data[i+1:j] )
                    message = data[j+1:]
                    func = lCallback.get( iFuncId , None )
                    if func :
                        func( )
                        del lCallback[iFuncId]
            except Exception , e :
                print e 
                break
        tcpCliSock.close( )
    tcpSerSock.close( )

def MyCallback( ) :
    print 'callback called !!!!!!'

def Send( callback , message ) :
    global iFuncId
    global lCallback
    lCallback[iFuncId] = callback
    listenerSock = socket( AF_INET , SOCK_STREAM )
    listenerSock.connect( ( 'localhost' , 3316 ) )
    listenerSock.send( "[%d] %s" % ( iFuncId , message ) )
    listenerSock.close( )
    iFuncId += 1
    print 'send message to YoSQL : %s' % message

def DoSomeThing( ) :
    print '............DoSomeThing............'

if __name__ == '__main__' :
    t = threading.Thread( target = StartListener )
    t.setDaemon( True )
    t.start( )
    DoSomeThing( )
    DoSomeThing( )
    Send( MyCallback , 'hahaha' )
    i = 0 
    while i < 20 :
        i += 1
        DoSomeThing( )
        try :
            time.sleep( 0.5 )
        except Exception , e :
            print e 
            break
    print '>>>>>>>>>>>>>>>>>>>>>>>'
