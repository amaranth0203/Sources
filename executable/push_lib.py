#!/usr/bin/env python_
#-*- coding=utf-8 -*-

#
# 编译库完成之后有提示 Install: out...
# 根据这个提示来push库文件
# vendor里边的log，kill camera之后就可以生效
# framework里边的log必须要重启才能生效
#

import qiyunhu_common
from qiyunhu_common import * ;

log_filename = "compile_lib.log"

def push_lib( log_list ) :
    for log in log_list :
        push_cmd = "adb push "
        push_cmd += log[log.find("out"):].strip() + " "
        push_cmd += log[log.find("/system"):log.rfind("/")].strip() + " "
        lexec( push_cmd )
    return
     

if __name__ == "__main__" :

    if not check_log( log_filename , "Install:" , 0 , 8 ) :
        exit( "[-] check_log() failed" )

    if not check_device( ) :
        exit( "[-] check_device() failed" )

    if not check_root( ) :
        if not root_device( ) :
            exit( "[-] root_device() failed" )

    push_lib( read_log( log_filename , "Install:" , 0 , 8 ) )
    # kill_camera_service( )












