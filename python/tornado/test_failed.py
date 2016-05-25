import time

def callback( flag ) :
    print '1 ' + flag
    print '2 ' + flag
    print '3 ' + flag
    print '4 ' + flag
    print '5 ' + flag
    print '6 ' + flag
    print '7 ' + flag
    print '8 ' + flag
    print '9 ' + flag


def caller( flag , func ) :
    func( flag )

caller( '1' , callback ) ;
caller( '2' , callback ) ;
caller( '3' , callback ) ;
