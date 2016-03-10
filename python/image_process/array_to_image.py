import numpy as np
import struct
import timeit
from PIL import Image

filename = "3l8.raw"

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

if __name__ == "__main__" :
    # w , h = 7 , 4
    w , h = 4208 , 3120
    time_start = timeit.default_timer( )
    with open( filename , "rb" ) as f :
        pass
        raw = f.read( w * h * 2 )
        time_a = timeit.default_timer( ) ####
        r_array = []
        for i in range( 0 , len( raw ) / 2 ) :
            r_array.append( ( ord( raw[i*2] ) | ( ord( raw[i*2+1] ) << 8 ) ) * 256 / 1024 ) 
        time_b = timeit.default_timer( ) ####
        r = np.array( r_array , dtype=np.uint8 )
        time_c = timeit.default_timer( ) ####
        g = r
        b = r
        data = np.column_stack( ( r , g , b ) )
        time_d = timeit.default_timer( ) ####
    data = data.reshape( h , w , 3 )
    time_e = timeit.default_timer( ) ####
    # data = demosaic( data , w , h )
    time_f = timeit.default_timer( ) ####
    img = Image.fromarray( data , 'RGB' )
    time_g = timeit.default_timer( ) ####
    img.save( "test.bmp" )
    img.show()
    time_h = timeit.default_timer( ) ####
    print "%d , %d , %d , %d , %d , %d , %d , %d" % (
        time_a - time_start ,
        time_b - time_start ,
        time_c - time_start ,
        time_d - time_start ,
        time_e - time_start ,
        time_f - time_start ,
        time_g - time_start ,
        time_h - time_start ,
    )