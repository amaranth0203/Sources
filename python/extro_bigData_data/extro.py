#!/usr/bin/env python 
#-*- coding=utf-8 -*-
__author__ = "qiyunhu"
__status__ = "test"
__version__ = "1.0.0"

target_filename = "temp_table_x9_102_c"
keyword = "c_luma"
column = ( 37 , 38 , 41 , 42 , 65 , 66 )

def extract_data( ) :
    target_data = []
    with open( target_filename , "r" ) as f :
        for line in f.readlines( ) :
            if keyword in line :
                target_data.append( line.replace( "\x01" , " " ).replace( "\x02" , " " ).replace( "\x03" , " " ) )
    return target_data

def format_data( data ) :
    target_data = []
    for line in data :
        current_line = []
        for col in column :
            current_line.append( line.split( )[col] )
        # if( int( current_line[5] ) == 1 ) :
        #     target_data.append( "\t".join( current_line ) )
        target_data.append( "\t".join( current_line ) )
    return target_data
        

if __name__ == '__main__' :
    data = extract_data( )
    data = format_data( data )
    print data[0:1]
    try :
        with open( "__data__.txt" , "w" ) as f :
            # f.write( "".join( str(a) for b in data for a in b ) )
            f.write( "\n".join( data ) )
    except IOError :
        print "[-] Write Failed"
