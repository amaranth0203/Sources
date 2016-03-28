#!/usr/bin/env python_
#-*- coding=utf-8 -*-

if __name__ == "__main__" :
    data_fname = "tamper_data.txt"
    with open( data_fname , "r" ) as f :
        for line in f.readlines( ) :
            if line.strip( )[-1:] != ':' :
                pass
            else :
                print line
    # raw_input( )