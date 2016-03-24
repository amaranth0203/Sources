#!/usr/bin/env python_
#-*- coding=utf-8 -*-

def get_last_two_record( ) :
    log_file_name = "AP.log"
    with open( log_file_name , "r" ) as f :
        lines = [ line for line in f.readlines( ) if line[0:4] == "repo" ]
    return lines[-2:]

if __name__ == "__main__" :
    last , current = get_last_two_record( )[0] , get_last_two_record( )[1]
    if last.strip() == current.strip() :
        print '[+] pass'
    else :
        print '[*] >>>>>>>>>>>>>>>>>>>>>>>> AP address updated <<<<<<<<<<<<<<<<<<<<<<<<<<<<'