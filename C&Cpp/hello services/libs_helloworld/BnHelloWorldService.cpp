#include <helloworld/BnHelloWorldService.h>
#include <binder/Parcel.h>
#include <string.h>

namespace android {

    status_t BnHelloWorldService::onTransact(   uint32_t code ,
                                                const Parcel& data ,
                                                Parcel* reply ,
                                                uint32_t flags/* = 0 */) 
    {
        switch( code ) {
        case HW_HELLOWORLD_ : {
            CHECK_INTERFACE( IHelloWorldService , data , reply ) ;
            const char* str ;
            str = data.readCString( ) ;
            char* result = "[+] wassup : " ;
            strcat( result , str ) ;
            reply->writeInt32( helloWorld( result ) ) ;
            return NO_ERROR ;
        } break ;
        default :
            return BBinder::onTransact( code , data , reply , flags ) ;
        }
    }

} ;