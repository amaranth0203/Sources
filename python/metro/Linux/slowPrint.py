#!/usr/bin/env python
# -*- coding: utf-8 -*-
__author__ = 'QiYunhu-13111020'
'''
    简单实现缓慢一个字符一个字符地输出

                                2014-11-26
'''

from time import sleep
import sys
#第二个参数是每个字符输出的时候的间隔时间
def slowPrint( s , time = 0.0009 ) :
    for ch in s :
        sys.stdout.write( ch )
        sys.stdout.flush( )
        sleep( time )
#测试用
if __name__ == '__main__' :
    slowPrint( "[+] Every one is a moon and has a dark side which never shows to others." )
    slowPrint( "[+] Every one is a moon and has a dark side which never shows to others." , 0.1 )
    slowPrint( "[+] Every one is a moon and has a dark side which never shows to others." )
