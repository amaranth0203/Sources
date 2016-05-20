#!/usr/bin/env python_
#-*- coding=utf-8 -*-

if __name__ == "__main__" :
    data_fname = "tamper_data.txt"
    flag_pass = True
    with open( data_fname , "r" ) as f :
        for line in f.readlines( ) :
            if flag_pass and line.strip( )[-1:] != ':' :
                continue
            else :
                flag_pass = False
            if not flag_pass and line.strip( )[-1:] == ':' :
                print '--------------------'
                print line.strip( )
            if not flag_pass and line.strip( )[-1:] == ']' :
                print '"' \
                    + line[:line.index('[')].strip( ) \
                    + '" : "' \
                    + line[line.index('[')+1:line.index(']')] \
                    + '" ,'
    # raw_input( )