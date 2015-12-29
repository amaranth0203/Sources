#!/usr/bin/env python_
#-*- coding=utf-8 -*-

# import qiyunhu_common
# from qiyunhu_common import * ;  

import signal
import sys

process1 = ""
process2 = ""

def signal_handler(signal, frame):
    process1.kill()
    process2.kill()
    sys.exit(0)  

if __name__ == "__main__" :

    print( '''
adb shell '(cat /proc/kmsg | while read LINE; do echo \\\\06kernel\\\\0$LINE\\\\0 > /dev/log/main; done)'

adb logcat -v time -f /dev/kmsg
adb shell cat /proc/kmsg
            ''')

    exit( 0 )

    signal.signal(signal.SIGINT, signal_handler)

    if not check_device( ) :
        exit( "[-] check_device() failed" )

    if not check_root( ) :
        if not root_device( ) :
            exit( "[-] root_device() failed" )

#    cmd1 = "adb shell '(cat /proc/kmsg | while read LINE; do echo \\\\06kernel\\\\0$LINE\\\\0 > /dev/log/main; done)'"
    cmd1 = "adb logcat -v time -f /dev/kmsg"
    process1 = subprocess.Popen( shlex.split( cmd1 ), stdout=subprocess.PIPE )
#    cmd2 = "adb logcat -v time"
    cmd2 = "adb shell cat /proc/kmsg "
    process2 = subprocess.Popen( shlex.split( cmd2 ) , stdout=subprocess.PIPE )
    while True:
        output = process2.stdout.readline()
        if output == '' and process2.poll() is not None:
            break
        if output:
            print output.strip()






    # cmd = "adb shell '(cat /proc/kmsg | while read LINE; do echo \\\\06kernel\\\\0$LINE\\\\0 > /dev/log/main; done)' | adb logcat -v time"
    # os.system( cmd )
