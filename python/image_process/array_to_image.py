import numpy as np
import struct
import timeit
import sys
sys.path.insert( 0 , './funcs' )
import funcs
from PIL import Image

filename = "3l8.raw"

if __name__ == "__main__" :
    w , h = 7 , 4
    # w , h = 4208 , 3120
    time_start = timeit.default_timer( ) ####
    with open( filename , "rb" ) as f :
        pass
        raw = f.read( w * h * 2 )
    print timeit.default_timer( ) - time_start ####
# raw to array
    raw_list = []
    funcs.str_to_int_array(  raw , len( raw ), raw_list )
    print timeit.default_timer( ) - time_start ####
# raw to array end
# demosaic
    r_list , g_list , b_list = [] , [] , []
    funcs.demosaic( raw_list , r_list , g_list , b_list , w , h ) ;
    exit()
# demosaic end
# pack array to numpy
    r = np.array( raw_list , dtype=np.uint8 )
    g = r
    b = r
    data = np.column_stack( ( r , g , b ) )
    data = data.reshape( h , w , 3 )
    print timeit.default_timer( ) - time_start ####
# pack array to numpy end
# save and show image
    img = Image.fromarray( data , 'RGB' )
    img.save( "test.bmp" )
    print timeit.default_timer( ) - time_start ####
# save and show image end


def demosaic( data , w , h ) :
    for i in range( 1 , h-1 ) :
        for j in range( 1 , w-1 ) :
            if i % 2 == 0 :
                if j % 2 == 0 :
                        data[i,j,0] = ( data[i,j-1,0] + data[i,j+1,0] ) / 2 # this point's R
                        data[i,j,2] = ( data[i-1,j,2] + data[i+1,j,2] ) / 2 # this point's B
                else :
                        data[i,j,1] = ( data[i,j-1,1] + data[i,j+1,1] + data[i-1,j,1] + data[i+1,j,1] ) / 4 # this point's G
                        data[i,j,2] = ( data[i-1,j-1,2] + data[i-1,j+1,2] + data[i+1,j-1,2] + data[i+1,j+1,2] ) / 4 # this point's B
            else :
                if j % 2 == 0 :
                        data[i,j,0] = ( data[i-1,j-1,0] + data[i-1,j+1,0] + data[i+1,j-1,0] + data[i+1,j+1,0] ) / 4 # this point's R
                        data[i,j,1] = ( data[i,j-1,1] + data[i,j+1,1] + data[i-1,j,1] + data[i+1,j,1] ) / 4 # this point's G
                else :
                        data[i,j,0] = ( data[i-1,j,0] + data[i+1,j,0] ) / 2 # this point's R
                        data[i,j,2] = ( data[i,j-1,2] + data[i,j+1,2] ) / 2 # this point's B
    return data
