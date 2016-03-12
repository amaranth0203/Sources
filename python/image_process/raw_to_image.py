import numpy as np
import struct
import timeit
import sys
sys.path.insert( 0 , './funcs' )
import funcs
from PIL import Image

filename = "3l8.raw"
time_last = timeit.default_timer( )

def print_time( str ) :
    global time_last
    time_cur = timeit.default_timer( )
    print "[+] {:4f} after {} ".format( time_cur - time_last , str )
    time_last = time_cur  
    

if __name__ == "__main__" :
    # w , h = 6 , 4
    w , h = 4208 , 3120
    with open( filename , "rb" ) as f :
        pass
        raw = f.read( w * h * 2 )
    print_time( "open file" )

    # raw to array
    raw_list = []
    funcs.str_to_int_array(  raw , len( raw ), raw_list )
    r_list , g_list , b_list = raw_list[:] , raw_list[:] , raw_list[:]
    print_time( "raw to array" )
    # raw to array end

    # image processing
    funcs.demosaic( raw_list , r_list , g_list , b_list , w , h ) ;
    funcs.brighten( r_list , g_list , b_list , 2.5 ) ;
    print_time( "image processing" )
    # image processing end

    # packed array to numpy
    r = np.array( r_list , dtype=np.uint8 )
    g = np.array( g_list , dtype=np.uint8 )
    b = np.array( b_list , dtype=np.uint8 )
    data = np.column_stack( ( r , g , b ) )
    data = data.reshape( h , w , 3 )
    print_time( "packed array to numpy" )
    # packed array to numpy end

    # save image
    img = Image.fromarray( data , 'RGB' )
    img.save( "test.bmp" )
    print_time( "save image" )
    # save image end
