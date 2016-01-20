#!/usr/bin/env python_
#-*- coding=utf-8 -*-

class qyh_base( object ) :

    def open_source_dir( self ) :
        '''@
        [+] callable
        [+] visible
        @short : osd
        @'''
        import os
        from sys import platform as _platform
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

    def read_config( self , section , keyword ) :
        return self.config.get( section , keyword )

    def write_config( self , section , keyword , value ) :
        import os
        self.config.set( section , keyword , value )
        with open( os.path.dirname( os.path.realpath( __file__ ) ) + '/qyh.ini' , 'wb' ) as ini_file :
            self.config.write( ini_file )

    def __init__( self , ) :
        try:
            from configparser import ConfigParser
        except ImportError:
            from ConfigParser import ConfigParser  # ver. < 3.0
        import os
        self.config = ConfigParser( ) 
        self.config.read( os.path.dirname( os.path.realpath( __file__ ) ) + '/qyh.ini' )
        flag_color = self.read_config( 'colorful' , 'flag' )
        self.print_color = self.print_colorful if flag_color == 'true' else self.print_none_color

    def set_colorful( self , *args ) :
        '''@
        [+] callable
        [+] visible
        @short : scf
        @args : "true" - use colorful print
        @args : "false" - not use colorful print
        @'''
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
        from ctypes import c_ulong , windll
        STD_OUTPUT_HANDLE_ID = c_ulong(0xfffffff5)
        windll.Kernel32.GetStdHandle.restype = c_ulong
        std_output_hdl = windll.Kernel32.GetStdHandle(STD_OUTPUT_HANDLE_ID)
        for color in xrange(16):
            windll.Kernel32.SetConsoleTextAttribute(std_output_hdl, color)
            print( "hello : " )
            print( color )

    def print_none_color( self , str , *nouse ) :
        import sys
        sys.stdout.write( str )

    def print_colorful( self , str , color ) :
        import sys
        from sys import platform as _platform
        if _platform == "win32" :
            from ctypes import c_ulong , windll
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
        import sys
        self.print_red( '[-] ' + sys._getframe().f_back.f_code.co_name + '() exit : ' + str + '\n' )
        exit( )

    def check_args( self , *args ) :
        import sys
        income = args[0]
        standard = args[1]
        # print type( args[0] )
        # print args[0]
        # raw_input()
        for arg in income :
            if arg not in standard :
                self.error_exit( '[-] ' + sys._getframe().f_back.f_code.co_name + '() error arg : ' + arg + '\n' )

    def trans_args( self , *args ) :
        income = args[0]
        standard = args[1]
        for arg in standard :
            if arg in income :
                yield True
            else :
                yield False

    def lexec( self , cmd , output = True , tips = True ) :
        import os
        if tips :
            self.print_yellow( "[!] " + cmd + "\n" )
        rc = os.popen( cmd ).read( )
        if output :
            print( rc )
        return rc

    def lexec_( self , cmd ) :
        import subprocess , shlex
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

class qyh_adb( qyh_base ) :

    def select_log_file( self , *args ) :
        '''@
        [+] callable
        [+] visible
        @short : slf
        @'''
        # select lib log file starts
        indexes = []
        current_file = self.read_config( "push_lib" , "log_file" )
        current_index = -1
        files = self.read_config( "push_lib" , "log_files" ).split("|")
        self.print_green_light( '\ncurrent -> {}\n\n'.format( current_file ) )
        for index,file in enumerate( files ) :
            print "[+] {} ---> {}".format( index + 1 , file )
            indexes.append( index )
            if file == current_file :
                current_index = index
        selected = -1
        selected_raw = raw_input( "[+] which one ?\n")
        if selected_raw == "" :
            selected = current_index
        else :
            try :
                selected = int( selected_raw ) - 1
            except :
                self.error_exit( '[-] bad option {}'.format( selected_raw ) )
        if selected not in indexes :
            self.error_exit( '[-] bad option {}'.format( selected + 1 ) )
        self.write_config( "push_lib" , "log_file" , files[selected] )
        # select lib log file ends

        # select bootimg log file starts
        print( '' )
        indexes = []
        current_file = self.read_config( "flash_boot" , "log_file" )
        current_index = -1
        files = self.read_config( "flash_boot" , "log_files" ).split("|")
        self.print_green_light( '\ncurrent -> {}\n\n'.format( current_file ) )
        for index,file in enumerate( files ) :
            print "[+] {} ---> {}".format( index + 1 , file )
            indexes.append( index )
            if file == current_file :
                current_index = index
        selected = -1
        selected_raw = raw_input( "[+] which one ?\n")
        if selected_raw == "" :
            selected = current_index
        else :
            try :
                selected = int( selected_raw ) - 1
            except :
                self.error_exit( '[-] bad option {}'.format( selected_raw ) )
        if selected not in indexes :
            self.error_exit( '[-] bad option {}'.format( selected + 1 ) )
        self.write_config( "flash_boot" , "log_file" , files[selected] )
        # select bootimg log file ends

        self.log_file( )
        pass

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

    def push_lib( self , *args ) :
        '''@
        [+] callable
        [+] visible
        @short : pl
        @args : backup - backup lib file to current directory before push lib file to phone
        @'''
        import datetime , os
        self.check_args( args , ( 'backup' , ) )
        flag_backup , =\
            tuple( self.trans_args( args , ( 'backup' , ) ) )

        log_filename = self.read_config( 'push_lib' , 'log_file' )

        self.check_log( log_filename , "Install:" , 0 , 8 ) 
        self.check_device( ) 
        self.check_root( )

        if flag_backup :
            folder_name = "backup_lib_" + str( datetime.datetime.now() ).split('.')[0].replace( '-' , '' ).replace( ':' , '' ).replace( ' ' , '_' )
            try :
                os.stat( folder_name )
            except :
                os.makedirs( folder_name )

        logs = self.read_log( log_filename , "Install:" , 0 , 8 )
        if flag_backup :
            for index , log in enumerate( logs ) :
                dir = folder_name + log[log.find("/system"):log.rfind("/")].strip()
                try :
                    os.stat( dir )
                except :
                    os.makedirs( dir )
                cmd_pull = 'adb pull '
                cmd_pull += log[log.find("/system"):].strip() + ' '
                cmd_pull += dir
                self.print_none_color( "[+] backup " )
                self.print_white( str( index + 1 ) + "/" + str( len( logs ) ) )
                self.print_none_color( " file(s) :\n" ) ;
                self.lexec_( cmd_pull )
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
        '''@
        [+] callable
        [+] visible
        @short : fb
        @'''
        log_filename = self.read_config( 'flash_boot' , 'log_file' )

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
        '''@
        [+] callable
        [+] visible
        @short : kc
        @'''
        self.check_device( )
        self.check_root( )

        for process_name in self.read_config( 'kill_camera_svr_and_clt' , 'camera_process_name' ).split( ' ' ) :
            self.kill_process( process_name )
        return True

    def kill_camera_service( self , ) :
        '''@
        [+] callable
        [+] visible
        @short : kcs
        @'''
        self.check_device( )
        self.check_root( )

        for process_name in self.read_config( 'kill_camera_service' , 'camera_process_name' ).split( ' ' ) :
            self.kill_process( process_name )
        return True

    def kill_camera_client( self , ) :
        '''@
        [+] callable
        [+] visible
        @short : kcc
        @'''
        self.check_device( )
        self.check_root( )

        for process_name in self.read_config( 'kill_camera_client' , 'camera_process_name' ).split( ' ' ) :
            self.kill_process( process_name )
        return True

    def start_camera( self , ) :
        '''@
        [+] callable
        [+] visible
        @short : sc
        @'''
        self.check_device( )
        self.lexec( self.read_config( "start_camera" , "command" ) )
        return True

    def take_picture( self , ) :
        '''@
        [+] callable
        [+] visible
        @short : tp
        @'''
        self.check_device( )
        self.lexec( self.read_config( "take_picture" , "command" ) )
        return True

    def power_button( self , ) :
        '''@
        [+] callable
        [+] visible
        @short : pb
        @'''
        self.check_device( )
        self.lexec( self.read_config( "power_button" , "command" ) )
        return True

    def unlock_screen( self , ) :
        '''@
        [+] callable
        [+] visible
        @short : us
        @'''
        self.check_device( )
        self.lexec( self.read_config( "unlock_screen" , "command" ) )
        return True

    def log_file( self , ) :
        '''@
        [+] callable
        [+] visible
        @short : lf
        @'''
        self.print_green( "[+] push_lib   : " + self.read_config( "push_lib" , "log_file" ) + "\n" )
        # self.print_green( "[+] check_lib  : " + self.read_config( "check_log" , "log_file" ) + "\n" )
        self.print_green( "[+] flash_boot : " + self.read_config( "flash_boot" , "log_file" ) + "\n" )
        return True

    def check_lib_log( self , *args ) :
        '''@
        [+] callable
        [+] visible
        @short : cll
        @args : "count" - print amount of lib files to be installed
        @args : "list" - list all lib files to be installed
        @'''
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
        '''@
        [+] callable
        [+] visible
        @short : lwd
        @'''
        self.print_yellow( self.read_config( "logcat_with_dmesg" , "command" ) + "\n" )
        return True

    def mobicat( self , ) :
        '''@
        [+] callable
        @'''
        self.print_yellow( '\n'.join( str( f ) for f in self.read_config( "mobicat" , "command" ).split( "\"" ) ) + '\n' )
        return True

    def metadata( self , ) :
        '''@
        [+] callable
        @'''
        self.print_yellow( '\n'.join( str( f ) for f in self.read_config( "metadata" , "command" ).split( "\"" ) ) + '\n' )
        return True

    def dump_jpeg( self , *args ) :
        '''@
        [+] callable
        [+] visible
        @short : dj
        @args : "all" - dump VivoCamera's jpeg & snapdragonCamera's raw & metadata
        @args : "meta" - dump VivoCamera's jpeg & metadata
        @args : "snapraw" - dump VivoCamera's jpeg & snapdragonCamera's raw
        @args : "del_only" - only delete file, not dump
        @'''
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
        '''@
        [+] callable
        [+] visible
        @short : dl
        @args : path - path to save system/ folder, auto mkdir if no exist
        @'''
        import os , shutil 
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

class qyh_svr( qyh_base ) :
    from SocketServer import TCPServer , ThreadingMixIn , BaseRequestHandler

    def __init__( self ) :
        pass
        super( qyh_svr , self ).__init__( ) ;
        self.svr_addr = self.svr_getaddr( )

    class ThreadedTCPRequestHandler( BaseRequestHandler ) :
        def handle( self ) :
            import threading , os
            data = self.request.recv( 65536 )
            cur_thread = threading.current_thread( )
            # self.request.sendall( 'wrapper : {}'.format(data) )
            self.request.sendall( '[!] ' + data + '\r\n' + os.popen( data ).read( ) )

    class ThreadedTCPServer( ThreadingMixIn , TCPServer ) :
        pass

    def svr_info( self ) :
        '''@
        [+] callable
        [+] visible
        @short : svi
        @'''
        pid = self.svr_getpid( )
        addr = self.svr_getaddr( )
        if self.svr_check( False ) :
            self.print_none_color( '[+] addr seted in ini file          : '.format( addr ) )
            self.print_green_light( "{}\n".format( addr ) )
            self.print_none_color( '[+] qyh_svr listening               : '.format( addr ) )
            self.print_green_light( '{}\n'.format( self.svr_getaddr( ) ) )
        else :
            self.print_red( '[+] qyh_svr daemon die\n' )
            self.print_none_color( '[+] addr seted in ini file : '.format( addr ) )
            self.print_green_light( "{}\n".format( addr ) )
            exit()

    def svr_reset( self , output = True , flag_kill = True ) :
        '''@
        [+] callable
        [+] visible
        @short : svr
        @'''
        from sys import platform as _platform
        if _platform == "win32" or _platform == "cygwin" :
            cmd_kill = "taskkill /im pythonw.exe /f"
        elif _platform == "linux" or _platform == "linux2" :
            cmd_kill = "pkill python"
        elif _platform == "darwin" :
            error_exit( 'function not implements in MacOS' )
        else :
            error_exit( 'Unknow Operating System' )
        self.svr_setpid( -1 )
        if flag_kill :
            self.lexec( cmd_kill , output , output )

    def svr_set( self ) :
        '''@
        [+] callable
        [+] visible
        @short : svt
        @'''
        import socket
        hostname = socket.gethostname( )
        ip = socket.gethostbyname( hostname )
        ip_ = raw_input( "[+] ip : " + ip + " ?\n" )
        if len( ip_ ) > 0 :
            ip = ip_
        port = 2334
        port_ = raw_input( "[+] port : 2334 ?\n" )
        if len( port_ ) > 0 :
            port = int( port_ )
        addr = ( ip , port )
        if socket.socket( socket.AF_INET , socket.SOCK_STREAM ).connect_ex( addr ) == 0 :
            self.error_exit( '[-] addr : ' + ':'.join(str(i) for i in addr) + ' not available\n' )
        self.print_none_color( '[+] set addr as : ' )
        self.print_green_light(  ':'.join(str(i) for i in addr) + '\n' )
        self.svr_setaddr( addr )

    def svr_check( self , flag_output = True ) :
        '''@
        [+] callable
        @short : svc
        @'''
        import socket
        try :
            s = socket.socket( socket.AF_INET , socket.SOCK_STREAM )
            s.settimeout( 0.1 )
            s.connect( self.svr_getaddr( ) )
            s.close( )
        except :
            if flag_output :
                self.print_red( '[-] remote server down\n' )
            return False
        if flag_output :
            self.print_green_light( '[+] remote server alive\n' )
        return True

    def svr_start( self ) :
        '''@
        [+] callable
        [+] visible
        @short : svs
        @'''
        import subprocess , os , sys
        if not self.svr_check( False ) :
            self.svr_reset( False , flag_kill = False)
            cmd_fwall_pass = 'netsh advfirewall firewall add rule name="qyh_svr" dir=out program="' + str( sys.executable ) + '" security=authenticate action=allow'
            self.lexec( cmd_fwall_pass , False , False )
            cmd_fwall_pass = 'netsh advfirewall firewall add rule name="qyh_svr" protocol=TCP dir=in localport=2334 action=allow'
            self.lexec( cmd_fwall_pass , False , False )
            cmd = 'pythonw ' + os.path.dirname( os.path.realpath( __file__ ) ).replace( '\\' , '/' ) + '/qyh.py svd'
            process = subprocess.Popen( cmd )
        else :
            self.error_exit( 'daemon is already running...' )

    def svr_daemon( self ) :
        '''@
        [+] callable
        @short : svd
        @'''
        import time , os , threading , signal
        self.svr_setpid( os.getpid() )
        server = self.ThreadedTCPServer( self.svr_addr , self.ThreadedTCPRequestHandler )
        server_thread = threading.Thread( target=server.serve_forever )
        server_thread.daemon = True
        server_thread.start()
        server_thread.join()

    def svr_kill( self ) :
        '''@
        [+] callable
        [+] visible
        @short : svk
        @'''
        import signal , os
        pid = self.svr_getpid( )
        if pid > 0 :
            try :
                os.kill( pid , signal.SIGTERM )
                self.write_config( "svr" , "pid" , -1 )
            except :
                self.error_exit( '!!!!!!!!!!!! os.kill exception' )
        else :
            self.error_exit( 'daemon down' )
        cmd_fwall_del = 'netsh advfirewall firewall del rul name="qyh_svr"'
        self.lexec( cmd_fwall_del , False , False )

    def svr_exec( self , *cmd ) :
        '''@
        [+] callable
        [+] visible
        @short : sve
        @args : cmd - command to execute in server
        @'''
        if len( cmd ) < 1 :
            self.error_exit( 'sve takes at least one command to execute' )
        if not self.svr_check( False ) :
            self.error_exit( 'remote server down' )
        import socket 
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect( self.svr_addr )
        try:
            sock.sendall( str( " ".join( cmd ) ) )
            response = sock.recv( 65536 )
            print "[+] result from svr_daemon : \n{}".format(response)
        finally:
            sock.close()

    def svr_getpid( self ) :
        return int( self.read_config( "svr" , "pid" ) )

    def svr_setpid( self , pid ) :
        self.write_config( "svr" , "pid" , pid )

    def svr_getaddr( self ) :
        addr = [ a.strip( ) for a in self.read_config( "svr" , "addr" ).replace('(','').replace(')','').replace('\'','').split(',') ]
        addr[1] = int( addr[1] )
        return tuple( addr )
        pass

    def svr_setaddr( self , ( ip , port ) ) :
        self.write_config( "svr" , "addr" , ( ip , port ) )
        pass

class qyh( qyh_svr , qyh_adb ) :

    def log(text):
        def decorator(func):
            def wrapper(*args, **kw):
                print('%s %s():' % (text, func.__name__))
                return func(*args, **kw)
            return wrapper
        return decorator

    @log('aaa')
    def testwrapper( ) :
        print "I'm the real function"

    def test( self ) :
        '''@
        [+] callable
        @'''
        import inspect
        print inspect.getsourcelines( eval( "self.testwrapper" ) )
        
        exit( )
        import socket
        s = socket.socket( socket.AF_INET , socket.SOCK_STREAM )
        host = 'www.guokr.com'
        ip = socket.gethostbyname( host )
        try :
            s.settimeout( 3 )
            s.connect( ( ip , 80 ) )
        except :
            self.error_exit( 'connect exception' )
        s.send( "GET / HTTP/1.0\r\n\r\n" )
        header = ( s.recv( 1000 ) )
        print header
        # header = header.split( "\r\n" )
        # length , = tuple( h for h in header if "content-length" in h.lower() )
        # length = int( length.split(":")[1].strip() )
        # total = 0
        # buf = ""
        # import sys
        # while True :
            # buf = s.recv( 100 )
            # total += len( buf )
            # sys.stdout.write( buf )
            # if len( buf ) == 0 :
                # break
        # print "\n" + str( total )
        # print length
        pass

    def __init__( self ) :
        super( qyh_svr , self ).__init__( ) 
        qyh_svr.__init__( self ) 

    def extract_comments( self , func_name = "" ) :
        import sys
        import inspect
        if func_name == "" :
            func_name = sys._getframe().f_code.co_name
        try :
            return inspect.getsource( eval(func_name) )\
                   [ inspect.getsource( eval(func_name) ).find( "''"+"'@" ) + 4 : \
                     inspect.getsource( eval(func_name) ).find( "@''"+"'" ) ]\
                   if "''"+"'@" in inspect.getsource( eval(func_name) ) else ""
        except :
            return ""

    def check_callable( self , func_name = "" ) :
        import sys
        if func_name == "" :
            func_name = sys._getframe().f_code.co_name
        return True if "[+] callable" in self.extract_comments( func_name ) else False

    def check_visible( self , func_name = "" ) :
        import sys
        if func_name == "" :
            func_name = sys._getframe().f_code.co_name
        return True if "[+] visible" in self.extract_comments( func_name ) else False        

    def get_formated_args( self , func_name = "" ) :
        import sys
        if func_name == "" :
            func_name = sys._getframe().f_code.co_name
        comments = self.extract_comments( func_name ).split( '@' )
        comments = tuple( c.strip( ) for c in comments if c.strip() )
        short = ''.join( tuple( c[ c.find( ":" ) + 1 : ].strip( ) for c in comments if 'short' in c ) )
        args = ' | '.join( tuple( c[ c.find( ":" ) + 1 : c.rfind( "-" ) ].strip( ) for c in comments if 'args' in c ) )
        return short , args

    def main_menu( self , ) :
        funcs = tuple( f for f in dir( self ) if not f.startswith( "_" ) and self.check_visible( "self." + f ) )
        print " qyh.py [ "
        for f in funcs :
            f_args = self.get_formated_args( "self." + f )
            self.print_none_color( "           " + f + " ( " )
            self.print_green( f_args[0] )
            self.print_none_color( " ) " )
            if f_args[1] :
                self.print_none_color( "[ " )
                self.print_yellow( f_args[1] )
                self.print_none_color( " ]" )
            self.print_none_color( "\n" )
        print "        ]"

    def main_loop( self , *args ) :
        if len( args ) > 1 :
            funcs = tuple( f for f in dir( self ) if not f.startswith( "_" ) and self.check_callable( "self." + f ) )
            func_map = { f if self.get_formated_args( "self." + f )[0] == "" else self.get_formated_args( "self." + f )[0] : f for f in funcs }
            self.check_args( args[1:2] , tuple( s for t in func_map.items( ) for s in t ) )
            func_name = "self." + ( func_map[ args[1] ] if args[1] in func_map else args[1] )
            if len( args ) > 2 :
                f = eval( func_name )
                f( *args[2:] )
            else :
                f = eval( func_name )
                f( )
        else :
            self.main_menu( )

if __name__ == "__main__" :
    import sys
    qyh( ).main_loop( *sys.argv )



