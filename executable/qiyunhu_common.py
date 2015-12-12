#!/usr/bin/env python_
#-*- coding=utf-8 -*-

#
#   大部分是使用adb/fastboot用到的函数
#
import sys , os , subprocess , shlex

def lexec( cmd ) :
#
#   执行命令
#   等到命令执行完毕之后
#   再输出命令的输出并返回命令执行结果
#
    print "[!] " + cmd
    rc = os.popen( cmd ).read( )
    print rc
    return rc

def lexec_( cmd ) :
#
#   执行命令
#   实时输出命令的输出
#   等到命令执行完毕之后
#   再返回命令执行结果
#
    print "[*] " + cmd
    rc = ""
    process = subprocess.Popen( shlex.split( cmd ), stdout=subprocess.PIPE )
    while True:
        output = process.stdout.readline()
        if output == '' and process.poll() is not None:
            break
        if output:
            rc += output
            print "[+] " + output.strip()
    # rc = process.poll()
    return rc

def check_device( ) :
#
#   查找安卓设备（单一）
#   （确认是否打开debug模式）
#
#   找到已经打开debug模式的设备时返回True
#   没找到返回False
#
    rc = lexec( "adb devices" )
    if rc.strip().replace( "devices" , "" ).find( "device" ) > 0 :
        return True
    else :
        print "[-] device not attached ><\n"
        return False

def check_root( ) :
#
#   前提：安卓设备（单一）已经打开debug模式
#   确认是否已经root
#
#   已经root返回True
#   没有root返回False
#
    rc = lexec ( "adb remount" ) 
    if rc.find( "succeeded" ) > 0 :
        print "[+] already root\n"
        return True
    else :
        print "[-] not root\n"
        return False

def check_fastboot_mode( ) :
#
#   安卓设备（单一）若是fastboot模式则返回True
#   安卓设备（单一）若不是fastboot模式则返回False
#
    rc = lexec( "fastboot devices" )
    if rc.strip().find( "fastboot" ) > 0 :
        print "[+] fastboot mode\n"
        return True
    else :
        print "[-] not fastboot mode\n"
        return False

def root_device( ) :
#
#   前提：安卓设备（单一）已经打开debug模式
#   对设备进行root
#
    rc = lexec( "adb root" )
    if rc.strip().find( "as root" ) > 0 :
        return True
    else :
        rc = lexec( "adb vivoroot" )
        if rc.strip().find( "as root" ) > 0 :
            return True
        else :
            return False
 
def reboot_fastboot( ) :    
#
#   前提：安卓设备（单一）已经打开debug模式并且root
#
#   尝试进入fastboot模式
#   进入成功返回True
#   进入失败返回False
#   
    lexec( "adb reboot bootloader" )
    try_count = 14
    while( try_count > 0 ) :
        rc = lexec( "fastboot devices" ) ;
        if rc.find( "fastboot" ) :
            print "[+] enter fastboot mode\n"
            return True
        else :
            print "[+] wait for devices : " + try_count
            try_count -= 1
    # lexec( "fastboot continue" )
    return False

def check_log( log_filename , keyword , k_index_start , k_index_end ) :
#
#   首先确认log文件是否存在
#   接在log文件中查找是否含有包含keyword的行（log是否有效）
#   
#   log_filename    :   log文件名
#   keyword         :   记录的关键字
#   k_index_start   :   keyword在每一行出现的起始位置
#   k_index_end     :   keyword在每一行出现的结束位置
#
#   log有效返回True
#   log无效返回False
#
    try :
        with open( log_filename , 'r' ) as f :
            for line in f.readlines( ) :
                if line[k_index_start:k_index_end] == keyword :
                    print "[+] log available\n"
                    return True ;
        print "[+] log not available ><\n"
        return False
    except IOError :
        print "[-] log file not exist ><\n"
        return False

def read_log( log_filename , keyword , k_index_start , k_index_end ) :
#
#   在log文件中查找含有keyword的行
#   
#   log_filename    :   log文件名
#   keyword         :   记录的关键字
#   k_index_start   :   keyword在每一行出现的起始位置
#   k_index_end     :   keyword在每一行出现的结束位置
#
#   返回值log_list中为log文件里keyword位置正确的每一行
#
    log_list = []
    with open( log_filename , 'r' ) as f :
        for line in f.readlines( ) :
            if line[k_index_start:k_index_end] == keyword :
                log_list.append( line.strip() )
    return log_list

def kill_process( process_name ) :
#
#   前提：安卓设备（单一）已经打开debug模式并且root
#   ps之后查找进程名并结束
#
    print "\n\n[+] kill camera serivce : "
    rc = lexec( "adb shell ps | grep " + process_name )
    if rc.strip() == "" :
        print "[-] process " + process_name + " not exist ><"
        return
    kill_cmd = ""
    kill_cmd += "adb shell kill -9" + " "
    kill_cmd += os.popen( "adb shell ps | grep " + process_name + " | gawk '{print $2}'" ).read( ) + " "
    lexec( kill_cmd )
    return

def kill_camera_service( ) :
    kill_process( "org.codeaurora.snapcam" )
    kill_process( "com.android.camera" )
    kill_process( "mm-qcamera-daemon" )

if __name__ == "__main__" :
    # if reboot_fastboot( ) :
        # print True
    # else :
        # print False
    pass























