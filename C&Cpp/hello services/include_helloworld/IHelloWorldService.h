#ifndef _IHELLOWORLDSERVICE_H_
#define _IHELLOWORLDSERVICE_H_

#include <binder/IInterface.h>

namespace android {

    enum{
        HW_HELLOWORLD_ = IBinder::FIRST_CALL_TRANSACTION ,
        // HW_HELLOWORLD_
    } ;
    // uint32_t HW_HELLOWORLD = 1 ;

    class IHelloWorldService: public IInterface {
        public :
        DECLARE_META_INTERFACE( HelloWorldService ) ;
        virtual status_t helloWorld( const char* str ) = 0 ;
    } ;

} ;

#endif

