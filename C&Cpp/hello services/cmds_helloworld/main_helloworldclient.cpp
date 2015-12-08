#define LOG_TAG "main_helloworldclient"

#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>

#include <utils/Log.h>
#include <utils/RefBase.h>
#include <helloworld/IHelloWorldService.h>

using namespace android ;

int main( int argc , char** argv )
{
    ALOGI( "HelloWorldService client is now starting" ) ;
    sp<IServiceManager> sm = defaultServiceManager( ) ;
    sp<IBinder> b ;
    sp<IHelloWorldService> sHelloWorldService ;

    do {
        b = sm -> getService( String16( "android.apps.IHelloWorldService" ) )  ;
        if( b != 0 )
            break ;
        ALOGI( "HelloWorldService is not working, waiting..." ) ;
        usleep( 500000 ) ;
    } while( true ) ;

    sHelloWorldService = interface_cast< IHelloWorldService >( b ) ;
    sHelloWorldService -> helloWorld( "hello, world" ) ;

    return ( 0 ) ;
}