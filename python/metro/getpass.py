#!/usr/bin/env pythoy
# -*- coding: gbk -*-
__author__ = 'QiYunhu-13111020'
'''
    用于windows和linux下边密码输入功能
                         2014-12-14
'''
import platform , getpass
#先判断操作系统，然后返回各个操作系统适用的getpass方法
def getpass( ) :
    Os = platform.system( )
    if( Os == 'Linux' ) :
        import sys , tty , termios

#linux下边要自己实现getch()
#用于获得键盘输入的字符码（跳过缓冲区）
        def getch( ) :
            fd = sys.stdin.fileno( )
            old_settings = termios.tcgetattr(fd)
            try :
                tty.setraw( sys.stdin.fileno( ) )
                ch = sys.stdin.read( 1 )
            finally :
                termios.tcsetattr( fd, termios.TCSADRAIN , old_settings )
            return ch
#参数是输入密码的时候显示的字符
        def getpass( maskchar = "*" ) :
            password = ""
            while True :
                ch = getch( )
#输入回车的时候表示输入结束
                if ch == '\r' or ch == '\n' :
                    print ""
                    return password
#输入了退格键
                elif ch == '\b' or ord(ch) == 127 :
                    if len(password) > 0 :
                        sys.stdout.write( "\b \b" )
                        password = password[:-1]
#正常字符
                else :
                    if maskchar != None :
                        sys.stdout.write( maskchar )
                    password += ch
        return getpass
    elif( Os == 'Windows' ) :
        import msvcrt , sys
        def getpass( maskchar = '*' ) :
            password = ""
            while True :
                ch = msvcrt.getch( )
                if ch in '\r\n' :
                    print ""
                    return password
                elif ch == '\b' :
                    if len( password ) > 0 :
                        sys.stdout.write( '\b \b' )
                        password = password[:-1]
                else :
                    password += ch
                    sys.stdout.write( maskchar )
        return getpass
    else :
        return getpass.getpass

#测试用
if __name__ == '__main__' :
    print "Enter password : "
    testGetPass = getpass( )
    password = testGetPass( '#' )
#    password = getpass( )
    print password 
