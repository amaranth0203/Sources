#!/usr/bin/env python_
#-*- coding=utf-8 -*-

import qiyunhu_common
from qiyunhu_common import * ;

if __name__ == "__main__" :

    if not check_device( ) :
        exit( "[-] check_device() failed" )

    if not check_root( ) :
        if not root_device( ) :
            exit( "[-] root_device() failed" )

    kill_camera_service( )
