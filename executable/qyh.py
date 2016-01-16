#!/usr/bin/env python_
#-*- coding=utf-8 -*-

import sys , os , subprocess , shlex , shutil
from ctypes import c_ulong , windll
from sys import platform as _platform
try:
    from configparser import ConfigParser
except ImportError:
    from ConfigParser import ConfigParser  # ver. < 3.0

def error_exit( str = "" ) :
    print_red( '[-] ' + sys._getframe().f_back.f_code.co_name + '() exit : ' + str + '\n' )
    exit( )

def check_args( *args ) :
    income = args[0]
    standard = args[1]
    for arg in income :
        if arg not in standard :
            print_red( '[-] ' + sys._getframe().f_back.f_code.co_name + '() error arg : ' + arg + '\n' )
            exit( -1 )

def trans_args( *args ) :
    income = args[0]
    standard = args[1]
    for arg in standard :
        if arg in income :
            yield True
        else :
            yield False

def print_color_test( ) :
    STD_OUTPUT_HANDLE_ID = c_ulong(0xfffffff5)
    windll.Kernel32.GetStdHandle.restype = c_ulong
    std_output_hdl = windll.Kernel32.GetStdHandle(STD_OUTPUT_HANDLE_ID)
    for color in xrange(16):
        windll.Kernel32.SetConsoleTextAttribute(std_output_hdl, color)
        print( "hello : " )
        print( color )

def print_none_color( str , *nouse ) :
    sys.stdout.write( str )

def print_colorful( str , color ) :
    if _platform == "win32" :
        STD_OUTPUT_HANDLE_ID = c_ulong(0xfffffff5)
        windll.Kernel32.GetStdHandle.restype = c_ulong
        std_output_hdl = windll.Kernel32.GetStdHandle(STD_OUTPUT_HANDLE_ID)
        windll.Kernel32.SetConsoleTextAttribute(std_output_hdl, color)
        sys.stdout.write( str )
        windll.Kernel32.SetConsoleTextAttribute(std_output_hdl, 7)
    elif _platform == "linux" or _platform == "linux2" or _platform == "cygwin" :
        color_code = {
            "none" : "\033[0m",
            2  : "\033[0;32m" ,
            4  : "\033[0;31m" ,
            6  : "\033[0;33m" ,
            10 : "\033[1;32m" ,
            12 : "\033[1;31m" ,
            14 : "\033[1;33m" ,
            15 : "\033[1;37m" ,
        }
        sys.stdout.write( color_code[color] + str + color_code["none"] )
    elif _platform == "darwin" :
        sys.stdout.write( str )
    else :
        sys.stdout.write( str )

print_color = print_colorful

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

def print_white( str ) :
    print_color( str , 15 )

def lexec( cmd , output = True ) :
#
#   执行命令
#   等到命令执行完毕之后
#   (再输出命令的输)出并返回命令执行结果
#
    print_yellow( "[!] " + cmd + "\n" )
    rc = os.popen( cmd ).read( )
    if output :
        print( rc )
    return rc

def lexec_( cmd ) :
#
#   执行命令
#   实时输出命令的输出
#   等到命令执行完毕之后
#   再返回命令执行结果
#
    print_yellow( "[!] " + cmd + "\n" )
    rc = ""
    process = subprocess.Popen( shlex.split( cmd ), stdout=subprocess.PIPE )
    while True:
        output = process.stdout.readline()
        if output == '' and process.poll() is not None:
            break
        if output:
            rc += output
            print( "[+] " + output.strip() )
    # rc = process.poll()
    print( ' ' )
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
        error_exit( "device not attached ><" )

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
        error_exit( "not root ><" )

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
        # print_red( "[-] not fastboot mode\n" )
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
#   前提：安卓设备（单一）已经打开debug模式
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
            print_green_light( "[+] wait for devices : " + try_count + "\n" )
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
        print_red( "[-] log not available ><\n" )
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
        log_list = [ line for line in f.readlines( ) if line[k_index_start:k_index_end] == keyword ]
    return log_list

def kill_process( *args ) :
#
#   前提：安卓设备（单一）已经打开debug模式并且root
#   ps之后查找进程名并结束
#
    process_name = args[0]
    print_green_light( "\n[+] kill process : " + process_name + "\n" )
    cmd_kill = 'adb shell "ps | grep ' + process_name + '"'
    rc = lexec( cmd_kill )
    if rc.strip() == "" :
        print_red( "[-] process " + process_name + " not exist ><" + "\n" )
        return
    kill_cmd = ""
    kill_cmd += "adb shell kill -9" + " " + rc.strip().split()[1]
    lexec( kill_cmd )
    return

def set_colorful( *args ) :
    if not len( args ) == 1 :
        print_red( "[-] invalid parameters\n" )
        return False
    if args[0].lower( ) not in ( 'true' , 'false' ) :
        print_red( "[-] invalid parameters\n" )
        return False
    config = ConfigParser( ) 
    config.read( os.path.dirname( os.path.realpath( __file__ ) ) + '/qyh.ini' )
    config.set( 'colorful' , 'flag' , args[0].lower( ) )
    with open( os.path.dirname( os.path.realpath( __file__ ) ) + '/qyh.ini' , 'wb' ) as ini_file :
        config.write( ini_file )
    read_global_config( )
    flag = config.get( 'colorful' , 'flag' ).lower( )
    print_green( "[+] colorful : " + flag + "\n" )
    return True

def push_lib( ) :

    config = ConfigParser( ) 
    config.read( os.path.dirname( os.path.realpath( __file__ ) ) + '/qyh.ini' )
    log_filename = config.get( 'push_lib' , 'log_file' )

    if not check_log( log_filename , "Install:" , 0 , 8 ) :
        exit( "[-] check_log() failed" )

    check_device( ) 

    check_root( )

    logs = read_log( log_filename , "Install:" , 0 , 8 )
    for index , log in enumerate( logs ) :
        cmd_push = 'adb push '
        cmd_push += log_filename[:log_filename.rfind('/')] + '/'
        cmd_push += log[log.find("out"):].strip() + ' '
        cmd_push += log[log.find("/system"):log.rfind("/")].strip() + " "
        print_none_color( "[+] push " )
        print_white( str( index + 1 ) + "/" + str( len( logs ) ) )
        print_none_color( " file(s) :\n" ) ;
        lexec_( cmd_push )
    return True

def flash_boot( ) :

    config = ConfigParser( ) 
    config.read( os.path.dirname( os.path.realpath( __file__ ) ) + '/qyh.ini' )
    log_filename = config.get( 'flash_boot' , 'log_file' )

    if not check_log( log_filename , "Target boot image:" , 0 , 18 ) :
        exit( "[-] check_log() failed" )

    if not check_fastboot_mode( ) :
        check_device( )
        reboot_fastboot( )

    lexec_( "fastboot bbk unlock_vivo" )

    for log in read_log( log_filename , "Target boot image:" , 0 , 18 ) :
        cmd_flash = "fastboot flash boot "
        cmd_flash += log_filename[:log_filename.rfind('/')] + '/'
        cmd_flash += log[log.find("out"):].strip() + " "
        lexec_( cmd_flash )

    lexec_( "fastboot reboot" )
    return True

def kill_camera_svr_and_clt( ) :

    check_device( )

    check_root( )

    config = ConfigParser( ) 
    config.read( os.path.dirname( os.path.realpath( __file__ ) ) + '/qyh.ini' )
    for process_name in config.get( 'kill_camera_svr_and_clt' , 'camera_process_name' ).split( ' ' ) :
        kill_process( process_name )
    return True

def kill_camera_service( ) :

    check_device( )

    check_root( )

    config = ConfigParser( ) 
    config.read( os.path.dirname( os.path.realpath( __file__ ) ) + '/qyh.ini' )
    for process_name in config.get( 'kill_camera_service' , 'camera_process_name' ).split( ' ' ) :
        kill_process( process_name )
    return True

def kill_camera_client( ) :

    check_device( )

    check_root( )

    config = ConfigParser( ) 
    config.read( os.path.dirname( os.path.realpath( __file__ ) ) + '/qyh.ini' )
    for process_name in config.get( 'kill_camera_client' , 'camera_process_name' ).split( ' ' ) :
        kill_process( process_name )
    return True

def start_camera( ) :

    check_device( )

    config = ConfigParser( ) 
    config.read( os.path.dirname( os.path.realpath( __file__ ) ) + '/qyh.ini' )
    cmd = config.get( "start_camera" , "command" )
    lexec( cmd )
    return True

def take_picture( ) :

    check_device( ) 

    config = ConfigParser( ) 
    config.read( os.path.dirname( os.path.realpath( __file__ ) ) + '/qyh.ini' )
    cmd = config.get( "take_picture" , "command" )
    lexec( cmd )
    return True

def power_button( ) :

    check_device( )

    config = ConfigParser( ) 
    config.read( os.path.dirname( os.path.realpath( __file__ ) ) + '/qyh.ini' )
    cmd = config.get( "power_button" , "command" )
    lexec( cmd )
    return True

def unlock_screen( ) :

    check_device( )

    config = ConfigParser( ) 
    config.read( os.path.dirname( os.path.realpath( __file__ ) ) + '/qyh.ini' )
    cmd = config.get( "unlock_screen" , "command" )
    lexec( cmd )
    return True

def log_fname( ) :

    config = ConfigParser( ) 
    config.read( os.path.dirname( os.path.realpath( __file__ ) ) + '/qyh.ini' )
    print_green( "[+] push_lib   : " + config.get( "push_lib" , "log_file" ) + "\n" )
    print_green( "[+] check_lib  : " + config.get( "check_log" , "log_file" ) + "\n" )
    print_green( "[+] flash_boot : " + config.get( "flash_boot" , "log_file" ) + "\n" )
    return True

def check_lib_log( *args ) :

    config = ConfigParser( ) 
    config.read( os.path.dirname( os.path.realpath( __file__ ) ) + '/qyh.ini' )
    log_filename = config.get( 'push_lib' , 'log_file' )

    check_args( args , ( 'count' , 'list' ) )

    flag_count , flag_list = tuple( trans_args( args , ( 'count' , 'list' ) ) )

    if check_log( log_filename , "Install:" , 0 , 8 ) :
        print_green_light( "[+] check lib log success" + "\n" ) ;
        if flag_count :
            logs = read_log( log_filename , "Install:" , 0 , 8 )
            print_none_color( "[+] Install: " )
            print_white( str( len( logs ) ) )
            print_none_color( " file(s)\n" )
        if flag_list :
            logs = read_log( log_filename , "Install:" , 0 , 8 )
            for index , log in enumerate( logs ) :
                print_none_color( "[+] " )
                print_white( str( index + 1 ) + "/" + str( len( logs ) ) + " : " )
                print_none_color( log[log.find('/system'):].strip() + '\n' ) ;
    else :
        print_red( "[-] check lib log failed" + "\n" ) ;

    return True

def logcat_with_dmesg( ) :

    config = ConfigParser( ) 
    config.read( os.path.dirname( os.path.realpath( __file__ ) ) + '/qyh.ini' )
    print_yellow( config.get( "logcat_with_dmesg" , "command" ) + "\n" )
    return True

def mobicat( ) :
    config = ConfigParser( ) 
    config.read( os.path.dirname( os.path.realpath( __file__ ) ) + '/qyh.ini' )
    print_yellow( '\n'.join( str( f ) for f in config.get ( "mobicat" , "command" ).split( "\"" ) ) + '\n' )
    return True

def metadata( ) :
    config = ConfigParser( ) 
    config.read( os.path.dirname( os.path.realpath( __file__ ) ) + '/qyh.ini' )
    print_yellow( '\n'.join( str( f ) for f in config.get ( "metadata" , "command" ).split( "\"" ) ) + '\n' )
    return True

def open_source_dir( ) :
    if _platform == "win32" or _platform == "cygwin" :
        cmd_open = "explorer "
        cmd_open += os.path.dirname( os.path.realpath( __file__ ) )
        lexec( cmd_open )
    elif _platform == "linux" or _platform == "linux2" :
        cmd_open = "nautilus "
        cmd_open += os.path.dirname( os.path.realpath( __file__ ) )
        lexec( cmd_open )
    elif _platform == "darwin" :
        print_green( "[+] Wow, this is Mac OS\n" ) ;
    else :
        print_red( "[-]unknow os\n" ) ;
    return True

def dump_jpeg( *args ) :

    check_args( args , ( 'meta' , 'snapraw' , 'all' , 'del_only' ) )

    flag_meta , flag_snapraw , flag_all , flag_del_only =\
        tuple( trans_args( args , ( 'meta' , 'snapraw' , 'all' , 'del_only' ) ) )

    check_device( )

    file = []

    ### dump metadata
    if flag_meta or flag_all :
        rc = lexec( 'adb shell ls /data' , False )
        if 'snapshot' in rc : ### grep condition "no such file or directory" out
            file += [ '/data/' + f for f in rc.split( ) if 'snapshot' in f ]
        rc = lexec( 'adb shell ls /data/misc/camera' , False )
        if '.raw' in rc or '.yuv' in rc or '.bin' in rc : ### grep condition "no such file or directory" out
            file += [ '/data/misc/camera/' + f for f in rc.split( ) if '.raw' in f or '.yuv' in f or '.bin' in f ]

    ### dump raw from Snapdragon Camera
    if flag_snapraw or flag_all :
        rc = lexec( 'adb shell ls /sdcard/DCIM/camera/raw' , False )
        if '.raw' in rc : ### grep condition "no such file or directory" out
            file += [ '/sdcard/DCIM/camera/raw/' + f for f in rc.split( ) if '.raw' in f ]

    ### dump photo from VivoCamera
    rc = lexec( 'adb shell ls /sdcard/' + u'\u76f8\u673a'.encode('utf-8') , False )
    if '.jpg' in rc : ### grep condition "no such file or directory" out
        file += [ '/sdcard/' + u'\u76f8\u673a'.encode('utf-8') + '/' + f for f in rc.split( ) if '.jpg' in f ]

    ### tips
    print( "------------------ file(s) to process start ------------------" )
    print( '\n'.join( str( f ) for f in file ) )
    print( "------------------ file(s) to process end --------------------" )
    ### pull command
    if not flag_del_only :
        for index , f in enumerate( file ) :
            print "[+] " + str( index + 1 ) + "/" + str( len( file ) ) + " file(s) :"
            lexec( 'adb pull ' + f + ' .' )
    ### rm files dumped
    for index , f in enumerate( file ) :
        print "[+] " + str( index + 1 ) + "/" + str( len( file ) ) + " file(s) :"
        lexec( 'adb shell rm ' + f )

    return True

def dump_lib( *args ) :

    spe_dir = False

    if len( args ) > 1 :
        print_red( "[+] to much args\n" )
        return False

    if len( args ) == 1 :
        try :
            os.stat( args[0] )
        except :
            os.makedirs( args[0] )
        spe_dir = True

    config = ConfigParser( ) 
    config.read( os.path.dirname( os.path.realpath( __file__ ) ) + '/qyh.ini' )
    log_filename = config.get( 'push_lib' , 'log_file' )

    if not check_log( log_filename , "Install:" , 0 , 8 ) :
        print_red( "[-] check lib log failed" + "\n" ) ;
        return True
    else :
        print_green_light( "[+] check lib log success" + "\n" ) ;

    logs = read_log( log_filename , "Install:" , 0 , 8 )
    for index , log in enumerate( logs ) :
        fsrc = log_filename[:log_filename.rfind('/')] + '/'
        fsrc += log[log.find("out"):].strip()
        if not spe_dir :
            fdst = os.getcwd( ).replace( '\\' , '/' ) + '/'
        else :
            fdst = args[0] + '/'
        fdst += log[log.find("/system")+1:log.rfind("/")].strip() + "/"
        try :
            os.stat( fdst )
        except :
            os.makedirs( fdst )
        print_none_color( "[+] copy " )
        print_white( str( index + 1 ) + "/" + str( len( logs ) ) )
        print_none_color( " file(s) : " + fsrc[fsrc.find('/system'):] + '\n' ) ;
        shutil.copy( fsrc , fdst )

    return True

def main_menu( ) :
    sys.stdout.write( ' ' + os.path.basename( sys.argv[0] ) + ' [\n' )
    sys.stdout.write( '           set_colorful (');print_green('scf');sys.stdout.write(') [ ' );print_yellow('"true" | "false"');sys.stdout.write(' ]              \n' )
    sys.stdout.write( '           push_lib (' );print_green('pl');sys.stdout.write(')               \n' )
    sys.stdout.write( '           flash_boot (' );print_green('fb');sys.stdout.write(')             \n' )
    sys.stdout.write( '           kill_camera_svr_and_clt (' );print_green('kc');sys.stdout.write(')\n' )
    sys.stdout.write( '           kill_camera_service (' );print_green('kcs');sys.stdout.write(')   \n' )
    sys.stdout.write( '           kill_camera_client (' );print_green('kcc');sys.stdout.write(')    \n' )
    sys.stdout.write( '           start_camera (' );print_green('sc');sys.stdout.write(')           \n' )
    sys.stdout.write( '           take_picture (' );print_green('tp');sys.stdout.write(')           \n' )
    sys.stdout.write( '           power_button (' );print_green('pb');sys.stdout.write(')           \n' )
    sys.stdout.write( '           unlock_screen (' );print_green('us');sys.stdout.write(')          \n' )
    sys.stdout.write( '           log_fname (' );print_green('lf');sys.stdout.write(')              \n' )
    sys.stdout.write( '           check_lib_log (' );print_green('cll');sys.stdout.write(') [ ');print_yellow('"count" | "list"');sys.stdout.write(' ]        \n' )
    sys.stdout.write( '           logcat_with_dmesg (' );print_green('ld');sys.stdout.write(')      \n' )
    sys.stdout.write( '           open_source_dir (' );print_green('osd');sys.stdout.write(')      \n' )
    sys.stdout.write( '           dump_jpeg (' );print_green('dj');sys.stdout.write(') [ ');print_yellow('"meta" | "snapraw" | "all" | "del_only"');sys.stdout.write(' ]     \n' )
    sys.stdout.write( '           dump_lib (' );print_green('dl');sys.stdout.write(') [ ');print_yellow('path');sys.stdout.write(' ]     \n' )
    sys.stdout.write( '        ]\n' )

def read_global_config( ) :
    global print_color
    config = ConfigParser( ) 
    config.read( os.path.dirname( os.path.realpath( __file__ ) ) + '/qyh.ini' )
    flag = config.get( 'colorful' , 'flag' ).lower( )
    print_color = print_colorful if flag == "true" else print_none_color
    return True

qyh_f = {
    "set_colorful"              : set_colorful ,
    "scf"                       : set_colorful ,
    "push_lib"                  : push_lib , 
    "pl"                        : push_lib , 
    "flash_boot"                : flash_boot ,    
    "fb"                        : flash_boot ,    
    "kill_camera_svr_and_clt"   : kill_camera_svr_and_clt , 
    "kc"                        : kill_camera_svr_and_clt , 
    "kill_camera_service"       : kill_camera_service , 
    "kcs"                       : kill_camera_service , 
    "kill_camera_client"        : kill_camera_client , 
    "kcc"                       : kill_camera_client , 
    "start_camera"              : start_camera , 
    "sc"                        : start_camera , 
    "take_picture"              : take_picture , 
    "tp"                        : take_picture , 
    "power_button"              : power_button , 
    "pb"                        : power_button , 
    "unlock_screen"             : unlock_screen , 
    "us"                        : unlock_screen , 
    "log_fname"                 : log_fname , 
    "lf"                        : log_fname , 
    "check_lib_log"             : check_lib_log , 
    "cll"                       : check_lib_log , 
    "logcat_with_dmesg"         : logcat_with_dmesg , 
    "ld"                        : logcat_with_dmesg , 
    "mobicat"                   : mobicat , 
    "metadata"                  : metadata , 
    "open_source_dir"           : open_source_dir ,
    "osd"                       : open_source_dir ,
    "dump_jpeg"                 : dump_jpeg ,
    "dj"                        : dump_jpeg ,
    "dump_lib"                  : dump_lib ,
    "dl"                        : dump_lib ,
}

class qyh_base( object ) :

    def read_config( self , section , keyword ) :
        return self.config.get( section , keyword )

    def write_config( self , section , keyword , value ) :
        self.config.set( section , keyword , value )
        with open( os.path.dirname( os.path.realpath( __file__ ) ) + '/qyh.ini' , 'wb' ) as ini_file :
            self.config.write( ini_file )

    def __init__( self , ) :
        self.config = ConfigParser( ) 
        self.config.read( os.path.dirname( os.path.realpath( __file__ ) ) + '/qyh.ini' )
        flag_color = self.read_config( 'colorful' , 'flag' )
        self.print_color = self.print_colorful if flag_color == 'true' else self.print_none_color
        pass

    def set_colorful( self , *args ) :
        if not len( args ) == 1 :
            self.print_red( "[-] invalid parameters\n" )
            return False
        if args[0].lower( ) not in ( 'true' , 'false' ) :
            self.print_red( "[-] invalid parameters\n" )
            return False
        self.write_config( 'colorful' , 'flag' , args[0].lower( ) )
        self.__init__( )
        flag = self.read_config( 'colorful' , 'flag' ).lower( )
        self.print_green( "[+] colorful : " + flag + "\n" )
        return True

    def print_color_test( self , ) :
        STD_OUTPUT_HANDLE_ID = c_ulong(0xfffffff5)
        windll.Kernel32.GetStdHandle.restype = c_ulong
        std_output_hdl = windll.Kernel32.GetStdHandle(STD_OUTPUT_HANDLE_ID)
        for color in xrange(16):
            windll.Kernel32.SetConsoleTextAttribute(std_output_hdl, color)
            print( "hello : " )
            print( color )

    def print_none_color( self , str , *nouse ) :
        sys.stdout.write( str )

    def print_colorful( self , str , color ) :
        if _platform == "win32" :
            STD_OUTPUT_HANDLE_ID = c_ulong(0xfffffff5)
            windll.Kernel32.GetStdHandle.restype = c_ulong
            std_output_hdl = windll.Kernel32.GetStdHandle(STD_OUTPUT_HANDLE_ID)
            windll.Kernel32.SetConsoleTextAttribute(std_output_hdl, color)
            sys.stdout.write( str )
            windll.Kernel32.SetConsoleTextAttribute(std_output_hdl, 7)
        elif _platform == "linux" or _platform == "linux2" or _platform == "cygwin" :
            color_code = {
                "none" : "\033[0m",
                2  : "\033[0;32m" ,
                4  : "\033[0;31m" ,
                6  : "\033[0;33m" ,
                10 : "\033[1;32m" ,
                12 : "\033[1;31m" ,
                14 : "\033[1;33m" ,
                15 : "\033[1;37m" ,
            }
            sys.stdout.write( color_code[color] + str + color_code["none"] )
        elif _platform == "darwin" :
            sys.stdout.write( str )
        else :
            sys.stdout.write( str )

    def print_green_light( self , str ) :
        self.print_color( str , 2 )

    def print_red_light( self , str ) :
        self.print_color( str , 4 )

    def print_yellow_light( self , str ) :
        self.print_color( str , 6 ) ;

    def print_green( self , str ) :
        self.print_color( str , 10 ) 

    def print_red( self , str ) :
        self.print_color( str , 12 ) 

    def print_yellow( self , str ) :
        self.print_color( str , 14 ) 

    def print_white( self , str ) :
        self.print_color( str , 15 )

    def error_exit( self , str = "" ) :
        self.print_red( '[-] ' + sys._getframe().f_back.f_code.co_name + '() exit : ' + str + '\n' )
        exit( )

    def check_args( self , *args ) :
        income = args[0]
        standard = args[1]
        for arg in income :
            if arg not in standard :
                self.error_exit( '[-] ' + sys._getframe().f_back.f_code.co_name + '() error arg : ' + arg + '\n' )
                # exit( -1 )

    def trans_args( self , *args ) :
        income = args[0]
        standard = args[1]
        for arg in standard :
            if arg in income :
                yield True
            else :
                yield False

    def lexec( self , cmd , output = True ) :
        self.print_yellow( "[!] " + cmd + "\n" )
        rc = os.popen( cmd ).read( )
        if output :
            print( rc )
        return rc

    def lexec_( self , cmd ) :
        self.print_yellow( "[!] " + cmd + "\n" )
        rc = ""
        process = subprocess.Popen( shlex.split( cmd ), stdout=subprocess.PIPE )
        while True:
            output = process.stdout.readline()
            if output == '' and process.poll() is not None:
                break
            if output:
                rc += output
                print( "[+] " + output.strip() )
        # rc = process.poll()
        print( ' ' )
        return rc

class qyh_adb( qyh_base , object ) :

    def check_device( self , ) :
        rc = self.lexec( "adb devices" )
        if rc.strip().replace( "devices" , "" ).find( "device" ) > 0 :
            self.print_green_light( "[+] device found\n" ) ;
            return True
        else :
            self.error_exit( "device not attached ><" )

    def check_root( self , ) :
        rc = self.lexec( "adb remount" ) 
        if rc.find( "succeeded" ) > 0 :
            self.print_green_light( "[+] already root\n" )
            return True
        else :
            self.error_exit( "not root ><" )

    def check_fastboot_mode( self , ) :
        rc = self.lexec( "fastboot devices" )
        if rc.strip().find( "fastboot" ) > 0 :
            self.print_green_light( "[+] fastboot mode\n" )
            return True
        else :
            return False

    def root_device( self , ) :
        # rc = self.lexec( "adb root" )
        rc = self.lexec( "adb vivoroot" )
        if rc.strip().find( "as root" ) > 0 :
            return True
        else :
            rc = self.lexec( "adb vivoroot" )
            if rc.strip().find( "as root" ) > 0 :
                return True
            else :
                return False
     
    def reboot_fastboot( self , ) :    
        self.lexec( "adb reboot bootloader" )
        try_count = 14
        while( try_count > 0 ) :
            rc = self.lexec( "fastboot devices" ) ;
            if rc.find( "fastboot" ) :
                self.print_green_light( "[+] enter fastboot mode\n" )
                return True
            else :
                self.print_green_light( "[+] wait for devices : " + try_count + "\n" )
                try_count -= 1
        # lexec( "fastboot continue" )
        return False

    def check_log( self , log_filename , keyword , k_index_start , k_index_end ) :
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
                        self.print_green_light( "[+] log available\n" )
                        return True ;
            self.error_exit( "[-] log not available ><\n" )
        except IOError :
            self.error_exit( "[-] log file not exist ><\n" )

    def read_log( self , log_filename , keyword , k_index_start , k_index_end ) :
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
            log_list = [ line for line in f.readlines( ) if line[k_index_start:k_index_end] == keyword ]
        return log_list

    def kill_process( self , *args ) :
    #
    #   前提：安卓设备（单一）已经打开debug模式并且root
    #   ps之后查找进程名并结束
    #
        process_name = args[0]
        self.print_green_light( "\n[+] kill process : " + process_name + "\n" )
        cmd_kill = 'adb shell "ps | grep ' + process_name + '"'
        rc = self.lexec( cmd_kill )
        if rc.strip() == "" :
            self.print_red( "[-] process " + process_name + " not exist ><" + "\n" )
            return
        kill_cmd = ""
        kill_cmd += "adb shell kill -9" + " " + rc.strip().split()[1]
        self.lexec( kill_cmd )
        return

    def push_lib( self , ) :
        log_filename = self.read_config( 'push_lib' , 'log_file' )

        self.check_log( log_filename , "Install:" , 0 , 8 ) 
        self.check_device( ) 
        self.check_root( )

        logs = self.read_log( log_filename , "Install:" , 0 , 8 )
        for index , log in enumerate( logs ) :
            cmd_push = 'adb push '
            cmd_push += log_filename[:log_filename.rfind('/')] + '/'
            cmd_push += log[log.find("out"):].strip() + ' '
            cmd_push += log[log.find("/system"):log.rfind("/")].strip() + " "
            self.print_none_color( "[+] push " )
            self.print_white( str( index + 1 ) + "/" + str( len( logs ) ) )
            self.print_none_color( " file(s) :\n" ) ;
            self.lexec_( cmd_push )
        return True

    def flash_boot( self , ) :
        config = ConfigParser( ) 
        config.read( os.path.dirname( os.path.realpath( __file__ ) ) + '/qyh.ini' )
        log_filename = config.get( 'flash_boot' , 'log_file' )

        self.check_log( log_filename , "Target boot image:" , 0 , 18 )
        if not self.check_fastboot_mode( ) :
            self.check_device( )
            self.reboot_fastboot( )

        self.lexec_( "fastboot bbk unlock_vivo" )
        for log in self.read_log( log_filename , "Target boot image:" , 0 , 18 ) :
            cmd_flash = "fastboot flash boot "
            cmd_flash += log_filename[:log_filename.rfind('/')] + '/'
            cmd_flash += log[log.find("out"):].strip() + " "
            self.lexec_( cmd_flash )
        self.lexec_( "fastboot reboot" )

        return True

    def kill_camera_svr_and_clt( self , ) :
        self.check_device( )
        self.check_root( )

        for process_name in self.read_config( 'kill_camera_svr_and_clt' , 'camera_process_name' ).split( ' ' ) :
            self.kill_process( process_name )
        return True

    def kill_camera_service( self , ) :
        self.check_device( )
        self.check_root( )

        for process_name in self.read_config( 'kill_camera_service' , 'camera_process_name' ).split( ' ' ) :
            self.kill_process( process_name )
        return True

    def kill_camera_client( self , ) :
        self.check_device( )
        self.check_root( )

        for process_name in self.read_config( 'kill_camera_client' , 'camera_process_name' ).split( ' ' ) :
            self.kill_process( process_name )
        return True

    def start_camera( self , ) :
        self.check_device( )
        self.lexec( self.read_config( "start_camera" , "command" ) )
        return True

    def take_picture( self , ) :
        self.check_device( )
        self.lexec( self.read_config( "take_picture" , "command" ) )
        return True

    def power_button( self , ) :
        self.check_device( )
        self.lexec( self.read_config( "power_button" , "command" ) )
        return True

    def unlock_screen( self , ) :
        self.check_device( )
        self.lexec( self.read_config( "unlock_screen" , "command" ) )
        return True

    def log_fname( self , ) :
        self.print_green( "[+] push_lib   : " + self.read_config( "push_lib" , "log_file" ) + "\n" )
        self.print_green( "[+] check_lib  : " + self.read_config( "check_log" , "log_file" ) + "\n" )
        self.print_green( "[+] flash_boot : " + self.read_config( "flash_boot" , "log_file" ) + "\n" )
        return True

    def check_lib_log( self , *args ) :
        log_filename = self.read_config( 'push_lib' , 'log_file' )

        self.check_args( args , ( 'count' , 'list' ) )
        self.check_log( log_filename , "Install:" , 0 , 8 )

        flag_count , flag_list = tuple( self.trans_args( args , ( 'count' , 'list' ) ) )
        self.print_green_light( "[+] check lib log success" + "\n" ) ;
        if flag_count :
            logs = self.read_log( log_filename , "Install:" , 0 , 8 )
            self.print_none_color( "[+] Install: " )
            self.print_white( str( len( logs ) ) )
            self.print_none_color( " file(s)\n" )
        if flag_list :
            logs = self.read_log( log_filename , "Install:" , 0 , 8 )
            for index , log in enumerate( logs ) :
                self.print_none_color( "[+] " )
                self.print_white( str( index + 1 ) + "/" + str( len( logs ) ) + " : " )
                self.print_none_color( log[log.find('/system'):].strip() + '\n' ) ;

        return True

    def logcat_with_dmesg( self , ) :
        self.print_yellow( self.read_config( "logcat_with_dmesg" , "command" ) + "\n" )
        return True

    def mobicat( self , ) :
        self.print_yellow( '\n'.join( str( f ) for f in self.read_config( "mobicat" , "command" ).split( "\"" ) ) + '\n' )
        return True

    def metadata( self , ) :
        self.print_yellow( '\n'.join( str( f ) for f in self.read_config( "metadata" , "command" ).split( "\"" ) ) + '\n' )
        return True

    def dump_jpeg( self , *args ) :

        self.check_args( args , ( 'meta' , 'snapraw' , 'all' , 'del_only' ) )

        flag_meta , flag_snapraw , flag_all , flag_del_only =\
            tuple( self.trans_args( args , ( 'meta' , 'snapraw' , 'all' , 'del_only' ) ) )

        self.check_device( )

        file = []

        ### dump metadata
        if flag_meta or flag_all :
            rc = self.lexec( 'adb shell ls /data' , False )
            if 'snapshot' in rc : ### grep condition "no such file or directory" out
                file += [ '/data/' + f for f in rc.split( ) if 'snapshot' in f ]
            rc = self.lexec( 'adb shell ls /data/misc/camera' , False )
            if '.raw' in rc or '.yuv' in rc or '.bin' in rc : ### grep condition "no such file or directory" out
                file += [ '/data/misc/camera/' + f for f in rc.split( ) if '.raw' in f or '.yuv' in f or '.bin' in f ]

        ### dump raw from Snapdragon Camera
        if flag_snapraw or flag_all :
            rc = self.lexec( 'adb shell ls /sdcard/DCIM/camera/raw' , False )
            if '.raw' in rc : ### grep condition "no such file or directory" out
                file += [ '/sdcard/DCIM/camera/raw/' + f for f in rc.split( ) if '.raw' in f ]

        ### dump photo from VivoCamera
        rc = self.lexec( 'adb shell ls /sdcard/' + u'\u76f8\u673a'.encode('utf-8') , False )
        if '.jpg' in rc : ### grep condition "no such file or directory" out
            file += [ '/sdcard/' + u'\u76f8\u673a'.encode('utf-8') + '/' + f for f in rc.split( ) if '.jpg' in f ]

        ### tips
        print( "------------------ file(s) to process start ------------------" )
        print( '\n'.join( str( f ) for f in file ) )
        print( "------------------ file(s) to process end --------------------" )
        ### pull command
        if not flag_del_only :
            for index , f in enumerate( file ) :
                print "[+] " + str( index + 1 ) + "/" + str( len( file ) ) + " file(s) :"
                self.lexec( 'adb pull ' + f + ' .' )
        ### rm files dumped
        for index , f in enumerate( file ) :
            print "[+] " + str( index + 1 ) + "/" + str( len( file ) ) + " file(s) :"
            self.lexec( 'adb shell rm ' + f )

        return True

    def dump_lib( self , *args ) :
        spe_dir = False
        if len( args ) > 1 :
            self.print_red( "[+] to much args\n" )
            return False
        if len( args ) == 1 :
            try :
                os.stat( args[0] )
            except :
                os.makedirs( args[0] )
            spe_dir = True

        log_filename = self.read_config( 'push_lib' , 'log_file' )
        self.check_log( log_filename , "Install:" , 0 , 8 )
        logs = self.read_log( log_filename , "Install:" , 0 , 8 )
        for index , log in enumerate( logs ) :
            fsrc = log_filename[:log_filename.rfind('/')] + '/'
            fsrc += log[log.find("out"):].strip()
            if not spe_dir :
                fdst = os.getcwd( ).replace( '\\' , '/' ) + '/'
            else :
                fdst = args[0] + '/'
            fdst += log[log.find("/system")+1:log.rfind("/")].strip() + "/"
            try :
                os.stat( fdst )
            except :
                os.makedirs( fdst )
            self.print_none_color( "[+] copy " )
            self.print_white( str( index + 1 ) + "/" + str( len( logs ) ) )
            self.print_none_color( " file(s) : " + fsrc[fsrc.find('/system'):] + '\n' ) ;
            shutil.copy( fsrc , fdst )

        return True

class qyh( qyh_adb , qyh_base , object ) :

    def open_source_dir( self ) :
        if _platform == "win32" or _platform == "cygwin" :
            cmd_open = "explorer "
            cmd_open += os.path.dirname( os.path.realpath( __file__ ) )
            self.lexec( cmd_open )
        elif _platform == "linux" or _platform == "linux2" :
            cmd_open = "nautilus "
            cmd_open += os.path.dirname( os.path.realpath( __file__ ) )
            self.lexec( cmd_open )
        elif _platform == "darwin" :
            self.print_green( "[+] Wow, this is Mac OS\n" ) ;
        else :
            self.print_red( "[-]unknow os\n" ) ;
        return True

    def main_menu( self ) :
        sys.stdout.write( ' ' + os.path.basename( sys.argv[0] ) + ' [\n' )
        sys.stdout.write( '           set_colorful (');self.print_green('scf');sys.stdout.write(') [ ' );self.print_yellow('"true" | "false"');sys.stdout.write(' ]              \n' )
        sys.stdout.write( '           push_lib (' );self.print_green('pl');sys.stdout.write(')               \n' )
        sys.stdout.write( '           flash_boot (' );self.print_green('fb');sys.stdout.write(')             \n' )
        sys.stdout.write( '           kill_camera_svr_and_clt (' );self.print_green('kc');sys.stdout.write(')\n' )
        sys.stdout.write( '           kill_camera_service (' );self.print_green('kcs');sys.stdout.write(')   \n' )
        sys.stdout.write( '           kill_camera_client (' );self.print_green('kcc');sys.stdout.write(')    \n' )
        sys.stdout.write( '           start_camera (' );self.print_green('sc');sys.stdout.write(')           \n' )
        sys.stdout.write( '           take_picture (' );self.print_green('tp');sys.stdout.write(')           \n' )
        sys.stdout.write( '           power_button (' );self.print_green('pb');sys.stdout.write(')           \n' )
        sys.stdout.write( '           unlock_screen (' );self.print_green('us');sys.stdout.write(')          \n' )
        sys.stdout.write( '           log_fname (' );self.print_green('lf');sys.stdout.write(')              \n' )
        sys.stdout.write( '           check_lib_log (' );self.print_green('cll');sys.stdout.write(') [ ');self.print_yellow('"count" | "list"');sys.stdout.write(' ]        \n' )
        sys.stdout.write( '           logcat_with_dmesg (' );self.print_green('ld');sys.stdout.write(')      \n' )
        sys.stdout.write( '           open_source_dir (' );self.print_green('osd');sys.stdout.write(')      \n' )
        sys.stdout.write( '           dump_jpeg (' );self.print_green('dj');sys.stdout.write(') [ ');self.print_yellow('"meta" | "snapraw" | "all" | "del_only"');sys.stdout.write(' ]     \n' )
        sys.stdout.write( '           dump_lib (' );self.print_green('dl');sys.stdout.write(') [ ');self.print_yellow('path');sys.stdout.write(' ]     \n' )
        sys.stdout.write( '        ]\n' )

if __name__ == "__main__" :
    # q = qyh( )
    # q.print_red( 'aaa\n' )
    # if len( sys.argv ) > 1 :
        # q.set_colorful( sys.argv[1:] )
    # q.main_menu( )
    # q.print_green( 'pass\n' )
    # exit( 0 )
    pass
    read_global_config( )
    if not len( sys.argv ) < 2 :
        if sys.argv[1].lower() in qyh_f :
            if not qyh_f[sys.argv[1].lower()]( *sys.argv[2:] ) :
                pass
        else :
            main_menu( )
    else :
        main_menu( )





