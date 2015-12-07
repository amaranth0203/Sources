#include <stdio.h>
#include <windows.h>

#define SLEEP_TIME 5000
#define LOGFILE "C:\\testServices\\memstatus.txt"

int WriteToLog( const char* str )
{
    FILE* log ;
    log = fopen( LOGFILE , "a+" ) ;
    if( log == NULL ) 
        return -1 ;
    fprintf( log , "%s\n" , str ) ;
    fclose( log ) ;
    return 0 ;
}

SERVICE_STATUS ServicesStatus ;
SERVICE_STATUS_HANDLE hStatus ;

void ServiceMain( int argc , char** argv ) ;
void ControlHandler( DWORD request ) ;
int InitService( ) ;

int main( )
{
    SERVICE_TABLE_ENTRY ServiceTable[2] ;
    ServiceTable[0].lpServiceName = "MemoryStatus" ;
    ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain ;

    ServiceTable[1].lpServiceName = NULL ;
    ServiceTable[1].lpServiceProc = NULL ;

    StartServiceCtrlDispatcher( ServiceTable ) ;
    return 0 ;
}

void ServiceMain( int argc , char** argv ) { } 
