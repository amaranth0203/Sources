#!/usr/bin/env python 
#-*- coding=utf-8 -*-
__author__ = "qiyunhu"
__status__ = "test"
__version__ = "1.0.0"

def extract_data( ) :
    '''
    返回的数据格式
    [
        [ _D65_700lux_ ] ,
        [ _D65_100lux_ ] ,
        [ _D65_20lux_ ] ,
        [ _A_700lux_ ] ,
        [ _A_100lux_ ] ,
        [ _A_20lux_ ] ,
    ]
    '''
    import xlrd
    import os
    import sys
    target_filename = []
    target_data = []
    for dirname , dirnames , filenames in os.walk( '.' ) :
        for filename in filenames :
            if '_D65_' in filename :
                if '_700lux_' in filename :
                    target_filename.append( filename )
        for filename in filenames :
            if '_D65_' in filename :
                if '_100lux_' in filename :
                    target_filename.append( filename )
        for filename in filenames :
            if '_D65_' in filename :
                if '_20lux_' in filename :
                    target_filename.append( filename )
        for filename in filenames :
            if '_A_' in filename :
                if '_700lux_' in filename :
                    target_filename.append( filename )
        for filename in filenames :
            if '_A_' in filename :
                if '_100lux_' in filename :
                    target_filename.append( filename )
        for filename in filenames :
            if '_A_' in filename :
                if '_20lux_' in filename :
                    target_filename.append( filename )
    for f in target_filename :
        data = []
        # [ (84,9) , (84,12) , (90,1) , (90,3) , (90,4) , (90,5) ]
        workbook = xlrd.open_workbook( f )
        data.append( workbook.sheet_by_name( 'Sheet1' ).cell_value( 84 , 9 ) )
        data.append( workbook.sheet_by_name( 'Sheet1' ).cell_value( 84 , 12 ) )
        data.append( workbook.sheet_by_name( 'Sheet1' ).cell_value( 90 , 1 ) )
        data.append( workbook.sheet_by_name( 'Sheet1' ).cell_value( 90 , 3 ) )
        data.append( workbook.sheet_by_name( 'Sheet1' ).cell_value( 90 , 4 ) )
        data.append( workbook.sheet_by_name( 'Sheet1' ).cell_value( 90 , 5 ) )
        target_data.append( data )
    return target_data

if __name__ == '__main__' :
    data = extract_data( )
    try :
        with open( "__data__.txt" , "w" ) as f :
            f.write( " ".join( str(a) for b in data for a in b ) )
    except IOError :
        print "[-] Write Failed"
