#!/usr/bin/env python_
#-*- coding=utf-8 -*-

import qiyunhu_common
from qiyunhu_common import * ;  

import signal
import sys

process1 = ""
process2 = ""

def signal_handler(signal, frame):
    process1.kill()
    process2.kill()
    sys.exit(0)  

if __name__ == "__main__" :

    signal.signal(signal.SIGINT, signal_handler)

    if not check_device( ) :
        exit( "[-] check_device() failed" )

    if not check_root( ) :
        if not root_device( ) :
            exit( "[-] root_device() failed" )

    cmd = "adb shell '(cat /proc/kmsg | while read LINE; do echo \\\\06kernel\\\\0$LINE\\\\0 > /dev/log/main; done)'"
    process1 = subprocess.Popen( shlex.split( cmd ), stdout=subprocess.PIPE )
    process2 = subprocess.Popen( shlex.split( "adb logcat -v time" ) , stdout=subprocess.PIPE )
    while True:
        output = process2.stdout.readline()
        if output == '' and process2.poll() is not None:
            break
        if output:
            print output.strip()






    # cmd = "adb shell '(cat /proc/kmsg | while read LINE; do echo \\\\06kernel\\\\0$LINE\\\\0 > /dev/log/main; done)' | adb logcat -v time"
    # os.system( cmd )

