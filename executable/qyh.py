#!/usr/bin/env python_
#-*- coding=utf-8 -*-

import sys , os , subprocess , shlex
from ctypes import *
try:
    from configparser import ConfigParser
except ImportError:
    from ConfigParser import ConfigParser  # ver. < 3.0


def print_color_test( ) :
    # from ctypes import *
    STD_OUTPUT_HANDLE_ID = c_ulong(0xfffffff5)
    windll.Kernel32.GetStdHandle.restype = c_ulong
    std_output_hdl = windll.Kernel32.GetStdHandle(STD_OUTPUT_HANDLE_ID)
    for color in xrange(16):
        windll.Kernel32.SetConsoleTextAttribute(std_output_hdl, color)
        print "hello : " 
        print color

def print_color( str , color ) :
    STD_OUTPUT_HANDLE_ID = c_ulong(0xfffffff5)
    windll.Kernel32.GetStdHandle.restype = c_ulong
    std_output_hdl = windll.Kernel32.GetStdHandle(STD_OUTPUT_HANDLE_ID)
    windll.Kernel32.SetConsoleTextAttribute(std_output_hdl, color)
    print( str )
    windll.Kernel32.SetConsoleTextAttribute(std_output_hdl, 15)

def print_green_light( str ) :
    print_color( str , 2 )

def print_red_light( str ) :
    print_color( str , 4 )

def print_yellow_light( str ) :
    print_color( str , 6 ) ;

def print_green( str ) :
    print_color( str , 10 ) 

def print_red( str ) :
    print_color( str , 12 ) 

def print_yellow( str ) :
    print_color( str , 14 ) 

def lexec( cmd ) :
#
#   执行命令
#   等到命令执行完毕之后
#   再输出命令的输出并返回命令执行结果
#
    print_yellow( "[!] " + cmd )
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
    print_yellow( "[!] " + cmd )
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
    print ' '
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
        print_green_light( "[+] device found\n" ) ;
        return True
    else :
        print_red( "[-] device not attached ><\n" )
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
        print_green_light( "[+] already root\n" )
        return True
    else :
        print_red( "[-] not root\n" )
        return False

def check_fastboot_mode( ) :
#
#   安卓设备（单一）若是fastboot模式则返回True
#   安卓设备（单一）若不是fastboot模式则返回False
#
    rc = lexec( "fastboot devices" )
    if rc.strip().find( "fastboot" ) > 0 :
        print_green_light( "[+] fastboot mode\n" )
        return True
    else :
        print_red( "[-] not fastboot mode\n" )
        return False

def root_device( ) :
#
#   前提：安卓设备（单一）已经打开debug模式
#   对设备进行root
#
    # rc = lexec( "adb root" )
    rc = lexec( "adb vivoroot" )
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
            print_green_light( "[+] enter fastboot mode\n" )
            return True
        else :
            print_green_light( "[+] wait for devices : " + try_count )
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
                    print_green_light( "[+] log available\n" )
                    return True ;
        print_red( "[+] log not available ><\n" )
        return False
    except IOError :
        print_red( "[-] log file not exist ><\n" )
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
    print_green_light( "\n[+] kill process : " + process_name )
    rc = lexec( "adb shell ps | find \"" + process_name + "\"" )
    if rc.strip() == "" :
        print_red( "[-] process " + process_name + " not exist ><" )
        return
    kill_cmd = ""
    kill_cmd += "adb shell kill -9" + " "
    kill_cmd += os.popen( "adb shell ps | find \"" + process_name + "\"" ).read().split()[1] + " "
    lexec( kill_cmd )
    return

def push_lib( ) :

    config = ConfigParser( ) 
    config.read( os.path.dirname( os.path.realpath( __file__ ) ) + '/qyh.ini' )
    log_filename = config.get( 'push_lib' , 'log_file' )

    if not check_log( log_filename , "Install:" , 0 , 8 ) :
        exit( "[-] check_log() failed" )

    if not check_device( ) :
        exit( "[-] check_device() failed" )

    if not check_root( ) :
        if not root_device( ) :
            exit( "[-] root_device() failed" )

    for log in read_log( log_filename , "Install:" , 0 , 8 ) :
        cmd_push = 'adb push '
        cmd_push += os.path.dirname( log_filename ).replace( '\\' , '/' ) + '/'
        cmd_push += log[log.find("out"):].strip() + ' '
        cmd_push += log[log.find("/system"):log.rfind("/")].strip() + " "
        lexec_( cmd_push )

def kill_camera( ) :

    if not check_device( ) :
        exit( "[-] check_device() failed" )

    if not check_root( ) :
        if not root_device( ) :
            exit( "[-] root_device() failed" )

    config = ConfigParser( ) 
    config.read( os.path.dirname( os.path.realpath( __file__ ) ) + '/qyh.ini' )
    for process_name in config.get( 'kill_camera' , 'camera_process_name' ).split( ' ' ) :
        kill_process( process_name )

def start_camera( ) :

    if not check_device( ) :
        exit( "[-] check_device() failed" )

    config = ConfigParser( ) 
    config.read( os.path.dirname( os.path.realpath( __file__ ) ) + '/qyh.ini' )
    cmd = config.get( "start_camera" , "command" )
    lexec( cmd )

def take_picture( ) :

    if not check_device( ) :
        exit( "[-] check_device() failed" )

    config = ConfigParser( ) 
    config.read( os.path.dirname( os.path.realpath( __file__ ) ) + '/qyh.ini' )
    cmd = config.get( "take_picture" , "command" )
    lexec( cmd )

def power_button( ) :

    if not check_device( ) :
        exit( "[-] check_device() failed" )

    config = ConfigParser( ) 
    config.read( os.path.dirname( os.path.realpath( __file__ ) ) + '/qyh.ini' )
    cmd = config.get( "power_button" , "command" )
    lexec( cmd )

def unlock_screen( ) :

    if not check_device( ) :
        exit( "[-] check_device() failed" )

    config = ConfigParser( ) 
    config.read( os.path.dirname( os.path.realpath( __file__ ) ) + '/qyh.ini' )
    cmd = config.get( "unlock_screen" , "command" )
    lexec( cmd )

def log_fname( ) :

    config = ConfigParser( ) 
    config.read( os.path.dirname( os.path.realpath( __file__ ) ) + '/qyh.ini' )
    print_green( "[+] push_lib  : " + config.get( "push_lib" , "log_file" ) )
    print_green( "[+] check_lib : " + config.get( "check_log" , "log_file" ) )

def mobicat( ) :
    config = ConfigParser( ) 
    config.read( os.path.dirname( os.path.realpath( __file__ ) ) + '/qyh.ini' )
    for cmd in config.get ( "mobicat" , "command" ).split( "\"" ) :
        lexec( cmd )

def main_menu( ) :
    print ' ' + os.path.basename( sys.argv[0] ) + ' [                             '
    print '           push_lib (pl)       |  '
    print '           kill_camera (kc)    |  '
    print '           start_camera (sc)   |  '
    print '           take_picture (tp)   |  '
    print '           power_button (pb)   |  '
    print '           unlock_screen (us)  |  '
    print '           log_fname (lf)      |  '
    print '        ]'

if __name__ == "__main__" :
    pass
    if len( sys.argv ) < 2 :
        main_menu( )
    else :
        for arg in sys.argv[1:] :
            if arg == "push_lib" or arg == "pl" :
                push_lib( )
            elif arg == "kill_camera" or arg == "kc" :
                kill_camera( )
            elif arg == "start_camera" or arg == "sc" :
                start_camera( )
            elif arg == "take_picture" or arg == "tp" :
                take_picture( )
            elif arg == "power_button" or arg == "pb" :
                power_button( )
            elif arg == "unlock_screen" or arg == "us" :
                unlock_screen( )
            elif arg == "log_fname" or arg == "lf" :
                log_fname( )
            elif arg == "mobicat" :
                mobicat( )
            else :
                main_menu( )





