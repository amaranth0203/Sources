#!/usr/bin/env python 
#-*- coding=utf-8 -*-

class qyh_base( object ) :

    def BASE_caller( self ) :
        '''@
        [+] visible
        [+] callable
        @short : test
        @'''
        pass
        import git , os 
        repo = git.Repo( "/cygdrive/e/Sources" )
        #  print repo.git.status( )
        #  print repo.git.add( '.' )
        #  print repo.git.commit( m = "routine push" )
        o = repo.remotes.origin
        ssh_executable = os.path.join( "/home/Administrator" , 'ssh_executable.sh' )
        repo.git.update_environment( GIT_SSH_COMMAND = ssh_executable )
        o.fetch( )
        #  o.push( repo.active_branch.name )

    def BASE_win32api( self , *args ) :
        '''@
        [+] visible
        [+] callable
        @short : vp
        @args : 32 - keyword to translate
        @args : 64 - keyword to translate
        @args : masm - keyword to translate
        @'''
        pass
        self.check_args( args , ( '32' , '64' , 'masm' ) )
        flag_32 , flag_64 , flag_masm =\
            tuple( self.trans_args( args , ( '32' , '64' , 'masm' ) ) )
        import os
        from sys import platform as _platform
        if _platform == "win32" :
            identity = os.getenv( 'identity' ) 
            if identity == 'Shadow' :
                cmd = ""
                if flag_32 :
                    cmd += '"C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\\vcvarsall.bat"\n'
                elif flag_64 :
                    cmd += '"C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\\vcvarsall.bat" amd64\n'
                if flag_masm :
                    cmd += "set Masm32Dir=C:\Masm32\n"
                    cmd += "set include=%Masm32Dir%\Include;%include%\n"
                    cmd += "set lib=%Masm32Dir%\lib;%lib%\n"
                    cmd += "set path=%Masm32Dir%\Bin;%Masm32Dir%;%PATH%\n"
                    cmd += "set Masm32Dir=\n"
                from Tkinter import Tk
                r = Tk()
                r.withdraw()
                r.clipboard_clear()
                r.clipboard_append( cmd )
                r.destroy()
                print "\n\n[+] execute command in clipboard yourself"
            elif identity == 'vivo_work' :
                cmd = ""
                if flag_32 :
                    cmd += '"D:\Program Files (x86)\\vs2015\VC\\vcvarsall.bat"\n'
                elif flag_64 :
                    cmd += '"D:\Program Files (x86)\\vs2015\VC\\vcvarsall.bat" amd64\n'
                if flag_masm :
                    cmd += "set Masm32Dir=C:\Masm32\n"
                    cmd += "set include=%Masm32Dir%\Include;%include%\n"
                    cmd += "set lib=%Masm32Dir%\lib;%lib%\n"
                    cmd += "set path=%Masm32Dir%\Bin;%Masm32Dir%;%PATH%\n"
                    cmd += "set Masm32Dir=\n"
                from Tkinter import Tk
                r = Tk()
                r.withdraw()
                r.clipboard_clear()
                r.clipboard_append( cmd )
                r.destroy()
                print "\n\n[+] execute command in clipboard yourself"
                # self.generate_env_script( [ 
                    # "Masm32Dir=C:\Masm32" ,
                    # "include=%Masm32Dir%\Include;%include%" ,
                    # "lib=%Masm32Dir%\lib;%lib%" ,
                    # "path=%Masm32Dir%\Bin;%Masm32Dir%;%PATH%" ,
                    # "Masm32Dir=" ,
                # ] )
            else :
                self.error_exit( 'unknown computer' ) ;
        else :
            self.error_exit( 'only in cmd' ) ;

    def generate_env_script( self , *args ) :
        '''@
        [+] callable
        @short : ges
        @'''
        env_d = args[0]
        from sys import platform as _platform
        import os , datetime
        if _platform == "win32" :
            cmd = ""
            for k in env_d : 
                cmd += "set "
                cmd += k
                cmd += "\n"
            # from Tkinter import Tk
            # r = Tk()
            # r.withdraw()
            # r.clipboard_clear()
            # r.clipboard_append( cmd )
            # r.destroy()
            # print "\n\n[+] execute command in clipboard yourself"
            filename = os.getcwd( ) + '/set_env_' + str( datetime.datetime.now() ).split('.')[0].replace( '-' , '' ).replace( ':' , '' ).replace( ' ' , '_' ) + '.bat'
            with open( filename , 'a+' ) as f :
                f.write( cmd )
            print '\n\nrun [ ' + os.path.basename( filename ) + ' ] yourself'
        elif _platform == "cygwin" :
            cmd = ""
            for k in env_d : 
                cmd += "export "
                cmd += k
                cmd += "\n"
            filename = os.getcwd( ) + '/set_env_' + str( datetime.datetime.now() ).split('.')[0].replace( '-' , '' ).replace( ':' , '' ).replace( ' ' , '_' ) + '.sh'
            with open( filename , 'a+' ) as f :
                f.write( cmd )
            print '\n\nrun [ source ' + os.path.basename( filename ) + ' ] yourself'
        elif _platform == "linux" or _platform == "linux2" :
            self.print_green( "[+] Wow, this is Linux\n" ) ;
        elif _platform == "darwin" :
            self.print_green( "[+] Wow, this is Mac OS\n" ) ;
        else :
            self.print_red( "[-]unknow os\n" ) ;
        return True

    def BASE_delete_vim_session( self , ) :
        '''@
        [+] visible
        [+] callable
        @short : dvs
        @'''
        from sys import platform as _platform
        if not _platform == "win32" :
            print '[-] not windows ><'
            exit( )
        import os
        self.lexec( 'del {}\qyh_session_gvim.vim'.format( os.getenv( 'UserProfile' ) ) )
        self.lexec( 'del {}\qyh_session_vim.vim'.format( os.getenv( 'UserProfile' ) ) )

    def BASE_get_news( self , *args ) :
        '''@
        [+] visible
        [+] callable
        @short : news
        @'''
        from sys import platform as _platform
        if _platform == "win32" or _platform == "cygwin" :
            pass
        else :
            print '[-] not windows ><'
            exit( )
        import urllib  
        import urllib2  
        import cookielib  
        opener = urllib2.build_opener()
        url = 'http://news.baidu.com'
        resp = opener.open(url)
        from lxml import etree
        tree = etree.HTML( resp.read() )
        for elem in tree.xpath( "//div[@class='imgtit']" ) :
            print etree.tostring( elem , pretty_print=True )
        #  import lxml.html
        #  html = lxml.html.fromstring( resp.read().strip() )
        #  results = html.xpath( "//div[@class='imgtit']" )
        #  try :
            #  #  print dir( results[0].getchildren()[0] )
            #  #  print results[0].xpath( )
            #  #  print " ".join( results[0].getchildren()[0].getchildren()[0].text_content().split( ) )
        #  except :
            #  print "-"

    def BASE_dictionary_youdao( self , *args ) :
        '''@
        [+] visible
        [+] callable
        @short : dy
        @args : keyword - keyword to translate
        @'''
        from sys import platform as _platform
        if _platform == "win32" or _platform == "cygwin" :
            pass
        else :
            print '[-] not windows ><'
            exit( )
        if len( args ) < 1 :
            self.error_exit( 'takes keyword to translate' )
        import urllib  
        import urllib2  
        import cookielib  
        if _platform == "win32" :
            url = "http://dict.youdao.com/w/" + urllib.quote( ' '.join( args ).decode( 'gbk' ).encode( 'utf-8' ) )
        elif _platform == "cygwin" :
            url = "http://dict.youdao.com/w/" + urllib.quote( ' '.join( args ) )
        opener = urllib2.build_opener()
        resp = opener.open(url)
        #  self.log_rejection( "a" , "a" , "".join( resp.read().strip().split() ) )
        #  raw = "".join( resp.read().strip().split() ) 
        #  idx = raw.find( 'trans-container' )
        #  raw = raw[ idx : ]
        #  idx = raw.find( '<li>' ) + 4
        #  length = raw.find( '</li>' )
        #  print raw[ idx : length ].decode( 'utf-8' )
        #  exit( )
        import lxml.html
        html = lxml.html.fromstring( resp.read().strip() )
        results = html.xpath( "//div[@class='trans-container']" )
        try :
            print " ".join( results[0].getchildren()[0].getchildren()[0].text_content().split( ) )
        except :
            print "-"

    def change_background_color( self , *args ) :
        '''@
        [+] callable
        @short : cbc
        @'''
        if len( args ) != 1 :
            exit( )
        from ctypes import c_ulong , windll
        STD_OUTPUT_HANDLE_ID = c_ulong(0xfffffff5)
        windll.Kernel32.GetStdHandle.restype = c_ulong
        std_output_hdl = windll.Kernel32.GetStdHandle(STD_OUTPUT_HANDLE_ID)
        windll.Kernel32.SetConsoleTextAttribute(std_output_hdl, int( args[0] ) )

    def BASE_tick( self , msg = "[+] ticking..." ) :
        '''@
        [+] callable
        [+] visible
        @short : t
        @args : msg - message to show
        @'''
        import os 
        from sys import platform as _platform
        if _platform == "win32" :
            cmd_tick = 'msg "%username%" '
            cmd_tick += msg
            self.lexec( cmd_tick , False , False )
        elif _platform == "cygwin" :
            cmd_tick = 'msg "$USER" '
            cmd_tick += msg
            self.lexec( cmd_tick , False , False )
        else :
            self.print_red( "[-] only run in windows\n" ) ;

    def run_putty_pub( self ) :
        '''@
        [+] callable
        @short : putty_pub
        @'''
        import os , subprocess
        from sys import platform as _platform
        if _platform == "win32" :
            subprocess.Popen( self.read_config( "putty_pub" , "command" ) )
        elif _platform == "cygwin" :
            os.popen( 'cmd /c ' + self.read_config( "putty_pub" , "command" ) + " &" )
        else :
            self.print_red( "[-] only run in windows\n" ) ;
        return True

    def run_putty_priv( self ) :
        '''@
        [+] callable
        @short : putty_priv
        @'''
        import os , subprocess
        from sys import platform as _platform
        if _platform == "win32" :
            subprocess.Popen( self.read_config( "putty_priv" , "command" ) )
        elif _platform == "cygwin" :
            os.popen( 'cmd /c ' + self.read_config( "putty_priv" , "command" ) + " &" )
        else :
            self.print_red( "[-] only run in windows\n" ) ;
        return True

    def run_vmware_background( self ) :
        '''@
        [+] callable
        @short : vm_run
        @'''
        import os
        from sys import platform as _platform
        if _platform == "win32" :
            self.lexec_( self.read_config( "run_vmware_background" , "command" ) )
        elif _platform == "cygwin" :
            os.popen( 'cmd /c ' + self.read_config( "run_vmware_background" , "command" ) + " &" )
        else :
            self.print_red( "[-] only run in windows\n" ) ;
        return True

    def mount_vmware_drive( self ) :
        '''@
        [+] callable
        @short : vm_mount
        @'''
        import os
        from sys import platform as _platform
        if _platform == "win32" :
            self.lexec( self.read_config( "mount_vmware_drive" , "command" ) )
        elif _platform == "cygwin" :
            self.lexec( self.read_config( "mount_vmware_drive" , "command" ).replace( '\\' , '\\\\' ) )
        elif _platform == "linux" or _platform == "linux2" :
            self.print_green( "[+] Wow, this is Linux, this function is under construction\n" ) ;
        elif _platform == "darwin" :
            self.print_green( "[+] Wow, this is Mac OS\n" ) ;
        else :
            self.print_red( "[-]unknow os\n" ) ;
        return True

    def lexec_hard( self , cmd ) :
        from sys import platform as _platform
        if _platform == "win32" :
            import win32com.client
            shell = win32com.client.Dispatch("WScript.Shell")
            shell.SendKeys( cmd )
        elif _platform == "cygwin" :
            self.print_green( "[+] Wow, this is cygwin\n" ) ;
        elif _platform == "linux" or _platform == "linux2" :
            self.print_green( "[+] Wow, this is Linux\n" ) ;
        elif _platform == "darwin" :
            self.print_green( "[+] Wow, this is Mac OS\n" ) ;
        else :
            self.print_red( "[-]unknow os\n" ) ;
        return True

    def git_routine_entity( self , flag , identity ) :
        pass
        infos = {
            "Shadow" : {
                "passphrase" : "python" ,
                "paths" : [ 
                    'E:\\Source' , 
                    'E:\\Stay\\txt_and_more\\notes' ,
                ]
            } ,
            "vivo_work" : {
                "passphrase" : "11002298" ,
                "paths" : [
                    'E:\\Sources' ,
                    'E:\\notes' ,
                ]
            } ,
        }
        import os
        pwd = str( os.getcwd() )
        info = infos[identity]
        cmd = ""
        if flag == "push" :
            for path in info["paths"] :
                cmd += 'cd /d '
                cmd += path
                cmd += '{ENTER}'
                cmd += 'git status{ENTER}'
                cmd += 'git add .{ENTER}'
                cmd += 'git commit -m "routine push"{ENTER}'
                cmd += 'git push origin master{ENTER}'
                cmd += info["passphrase"]
                cmd += '{ENTER}'
            cmd += 'cd /d ' + pwd + '{ENTER}'
        if flag == "pull" :
            for path in info["paths"] :
                cmd += 'cd /d '
                cmd += path
                cmd += '{ENTER}'
                cmd += 'git status{ENTER}'
                cmd += 'git add .{ENTER}'
                cmd += 'git commit -m "routine push"{ENTER}'
                cmd += 'git pull origin master{ENTER}'
                cmd += info["passphrase"]
                cmd += '{ENTER}'
            cmd += 'cd /d ' + pwd + '{ENTER}'
        self.lexec_hard( cmd )
        # import win32com.client
        # shell = win32com.client.Dispatch("WScript.Shell")
        # shell.SendKeys( cmd )
        return True

    def BASE_git_routine( self , *args ) :
        '''@
        [+] callable
        [+] visible
        @short : gr
        @args : push - push codes to origin
        @args : pull - pull codes from origin
        @'''
        import os
        from sys import platform as _platform
        if _platform == "win32" :
            identity = os.getenv( 'identity' ) 
            if identity == 'Shadow' :
                sources_path = 'E:\\Source'
                notes_path = 'E:\\Stay\\txt_and_more\\notes'
                passphrase = 'python'
            elif identity == 'vivo_work' :
                sources_path = 'E:\\Sources'
                notes_path = 'E:\\notes'
                passphrase = '11002298'
            else :
                self.error_exit( 'unknown computer' ) ;
            self.check_args( args , ( 'push' , 'pull' ) )
            flag_push , flag_pull =\
                tuple( self.trans_args( args , ( 'push' , 'pull' ) ) )
            pwd = str( os.getcwd() )
            if flag_push :
                self.git_routine_entity( "push" , identity )
            if flag_pull :
                self.git_routine_entity( "pull" , identity )
        elif _platform == "cygwin" :
            self.print_green( "[+] Wow, this is cygwin\n" ) ;
        elif _platform == "linux" or _platform == "linux2" :
            self.print_green( "[+] Wow, this is Linux\n" ) ;
        elif _platform == "darwin" :
            self.print_green( "[+] Wow, this is Mac OS\n" ) ;
        else :
            self.print_red( "[-]unknow os\n" ) ;
        return True

    def BASE_change_to_source_dir( self ) :
        '''@
        [+] callable
        [+] visible
        @short : csd
        @'''
        import os
        from sys import platform as _platform
        if _platform == "win32" :
            cmd = "cd /d "
            cmd += os.path.dirname( os.path.realpath( __file__ ) ) 
            import win32com.client
            shell = win32com.client.Dispatch("WScript.Shell")
            shell.SendKeys( cmd )
            shell.SendKeys( "{ENTER}" )
            # shell.SendKeys("^a") # CTRL+A may "select all" depending on which window's focused
            # shell.SendKeys("{DELETE}") # Delete selected text?  Depends on context. :P
            # shell.SendKeys("{TAB}") #Press tab... to change focus or whateverfrom subprocess import Popen, PIPE

        elif _platform == "cygwin" :
            self.print_green( "[+] Wow, this is cygwin, why not use command which\n" ) ;
        elif _platform == "linux" or _platform == "linux2" :
            self.print_green( "[+] Wow, this is Linux, why not use command which\n" ) ;
        elif _platform == "darwin" :
            self.print_green( "[+] Wow, this is Mac OS\n" ) ;
        else :
            self.print_red( "[-]unknow os\n" ) ;
        return True

    def BASE_open_source_dir( self ) :
        '''@
        [+] callable
        [+] visible
        @short : osd
        @'''
        import os
        from sys import platform as _platform
        if _platform == "win32" :
            cmd_open = "explorer "
            cmd_open += os.path.dirname( os.path.realpath( __file__ ) )
            self.lexec( cmd_open )
        elif _platform == "cygwin" :
            cmd_open = "explorer "
            cmd_open += os.path.dirname( __file__ ).replace( '/' , '\\\\' ) 
            self.lexec( cmd_open )
        elif _platform == "linux" or _platform == "linux2" :
            self.print_green( "[+] Wow, this is Linux\n" ) ;
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
        with open( os.path.dirname( __file__ ) + '/qyh.ini' , 'wb' ) as ini_file :
            self.config.write( ini_file )

    def __init__( self , ) :
        try:
            from configparser import ConfigParser
        except ImportError:
            from ConfigParser import ConfigParser  # ver. < 3.0
        import os
        self.config = ConfigParser( ) 
        self.config.read( os.path.dirname( __file__ ) + '/qyh.ini' )
        flag_color = self.read_config( 'colorful' , 'flag' )
        self.print_color = self.print_colorful if flag_color == 'true' else self.print_none_color

    def BASE_set_colorful( self , *args ) :
        '''@
        [+] callable
        [+] visible
        @short : scf
        @args : true - use colorful print
        @args : false - not use colorful print
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
        '''@
        [+] callable
        @'''
        from sys import platform as _platform
        if _platform == "win32" :
            from ctypes import c_ulong , windll
            STD_OUTPUT_HANDLE_ID = c_ulong(0xfffffff5)
            windll.Kernel32.GetStdHandle.restype = c_ulong
            std_output_hdl = windll.Kernel32.GetStdHandle(STD_OUTPUT_HANDLE_ID)
            for color in xrange(16):
                windll.Kernel32.SetConsoleTextAttribute(std_output_hdl, color)
                print( "hello : {}".format( color ) )
            windll.Kernel32.SetConsoleTextAttribute(std_output_hdl, 7)
        elif _platform == "linux" or _platform == "linux2" or _platform == "cygwin" :
            for color in xrange( 16 ) :
                self.print_colorful( "hello : {}\n".format( color ) , color )

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
                0  : "\033[0;30m" ,
                1  : "\033[0;34m" ,
                2  : "\033[0;32m" ,
                3  : "\033[0;36m" ,
                4  : "\033[0;31m" ,
                5  : "\033[0;35m" ,
                6  : "\033[0;33m" ,
                7  : "\033[0;37m" ,
                8  : "\033[1;30m" ,
                9  : "\033[1;34m" ,
                10 : "\033[1;32m" ,
                11 : "\033[1;36m" ,
                12 : "\033[1;31m" ,
                13 : "\033[1;35m" ,
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
            self.print_yellow( "[!] " + str( cmd ) + "\n" )
        rc = os.popen( cmd ).read( )
        if output :
            print( rc )
        return rc

    def lexec_( self , cmd ) :
        import subprocess , shlex
        self.print_yellow( "[!] " + str( cmd ) + "\n" )
        rc = ""
        process = subprocess.Popen( shlex.split( cmd ), stdout=subprocess.PIPE )
        while True:
            output = process.stdout.readline()
            if output == '' and process.poll() is not None:
                break
            if output:
                rc += output
                print( output.strip() )
        # rc = process.poll()
        print( ' ' )
        return rc

class qyh_adb( qyh_base ) :

    def adb_set_adb_no_check( self , *args ) :
        '''@
        [+] callable
        [+] visible
        @short : snc
        @args : true - true
        @args : false - false
        @'''
        self.check_args( args , ( 'true' , 'false' ) )
        flag_true , flag_false =\
            tuple( self.trans_args( args , ( 'true' , 'false' ) ) )
        import os
        print '[+] no check flag : {}'.format( os.getenv( 'qyh_adb_no_check' ) )
        if flag_true :
            #  cmd = "set qyh_adb_no_check=true{ENTER}"
            #  self.lexec_hard( cmd )
            self.generate_env_script( [ "qyh_adb_no_check=true" ] )
        if flag_false :
            #  cmd = "set qyh_adb_no_check={ENTER}"
            #  self.lexec_hard( cmd )
            self.generate_env_script( [ "qyh_adb_no_check=" ] )

    def adb_select_device_serial( self , *args ) :
        '''@
        [+] callable
        [+] visible
        @short : sds
        @'''
        import os
        indexes = []
        current_serial = os.getenv( 'ANDROID_SERIAL' )
        current_index = -1
        devices = self.lexec( 'adb devices' ).strip( ).split( '\n' )[1:]
        self.print_green_light( '\ncurrent -> {}\n\n'.format( current_serial ) )
        print "[+] {} ---> {}".format( 0 , 'None' )
        indexes.append( -1 )
        for index,serial in enumerate( devices ) :
            print "[+] {} ---> {}".format( index + 1 , serial.split( )[0] )
            indexes.append( index )
            if serial.split( )[0] == current_serial :
                current_index = index
        selected = -1
        import sys
        sys.stdout.flush( )
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
        if selected == -1 :
            #  cmd = 'set ANDROID_SERIAL={ENTER}'
            self.generate_env_script( [ "ANDROID_SERIAL=" ] )
        else :
            #  cmd = 'set ANDROID_SERIAL={}'.format( devices[ selected ].split( )[0] ) + '{ENTER}'
            self.generate_env_script( [ "ANDROID_SERIAL=" + devices[ selected ].split( )[0] ] )
        #  self.lexec_hard( cmd )
        

    def adb_log_file_select( self , *args ) :
        '''@
        [+] callable
        [+] visible
        @short : lf_s
        @'''
        import os
        # select lib log file starts
        indexes = []
        # current_file = self.read_config( "push_lib" , "log_file" )
        current_file = os.getenv( 'qyh_llf' )
        current_index = -1
        files = self.read_config( "push_lib" , "log_files" ).split("|")
        self.print_green_light( '\ncurrent -> {}\n\n'.format( current_file ) )
        for index,file in enumerate( files ) :
            print "[+] {} ---> {}".format( index + 1 , file )
            indexes.append( index )
            if file == current_file :
                current_index = index
        selected = -1
        import sys
        sys.stdout.flush( )
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
        #  cmd = 'set qyh_llf={}'.format( files[selected] ) + '{ENTER}'
        env_d = []
        env_d.append( 'qyh_llf={}'.format( files[selected] ) )
        # select lib log file ends

        # select bootimg log file starts
        print( '' )
        indexes = []
        # current_file = self.read_config( "flash_boot" , "log_file" )
        current_file = os.getenv( 'qyh_flf' )
        current_index = -1
        files = self.read_config( "flash_boot" , "log_files" ).split("|")
        self.print_green_light( '\ncurrent -> {}\n\n'.format( current_file ) )
        for index,file in enumerate( files ) :
            print "[+] {} ---> {}".format( index + 1 , file )
            indexes.append( index )
            if file == current_file :
                current_index = index
        selected = -1
        import sys
        sys.stdout.flush( )
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
        # select bootimg log file ends

        #  cmd += 'set qyh_flf={}'.format( files[selected] ) + '{ENTER}'
        #  self.lexec_hard( cmd )
        env_d.append( 'qyh_flf={}'.format( files[selected] ) )
        self.generate_env_script( env_d )
        pass

    def adb_log_file_add( self , ) :
        '''@
        [+] callable
        [+] visible
        @short : lf_a
        @'''
        drive = raw_input( "[+] which drive ? ( default Z:/ ) " ) or "Z"
        proj = raw_input( "[+] project folder name ? " )
        llf = ""
        flf = ""
        if proj == "" :
            self.error_exit( '[-] project folder name needed....' )
        llf = drive + ":/proj/" + proj + "/compile_lib.log"
        flf = drive + ":/proj/" + proj + "/compile_boot.log"
        files = self.read_config( "push_lib" , "log_files" ).split( '|' )
        files.append( llf )
        self.write_config( "push_lib" , "log_files" , '|'.join( files ) )
        files = self.read_config( "flash_boot" , "log_files" ).split( '|' )
        files.append( flf )
        self.write_config( "flash_boot" , "log_files" , '|'.join( files ) )
        return True

    def adb_log_file_remoev( self , ) :
        '''@
        [+] callable
        [+] visible
        @short : lf_r
        @'''
        indexes = []
        files = self.read_config( "push_lib" , "log_files" ).split("|")
        for index,file in enumerate( files ) :
            print "[+] {} ---> {}".format( index + 1 , file )
            indexes.append( index )
        selected = -1
        import sys
        sys.stdout.flush( )
        selected_raw = raw_input( "[+] which one ?\n")
        if selected_raw == "" :
            self.print_red( "[-] aborting...\n" )
            exit( )
        else :
            try :
                selected = int( selected_raw ) - 1
            except :
                self.error_exit( '[-] bad option {}'.format( selected_raw ) )
        if selected not in indexes :
            self.error_exit( '[-] bad option {}'.format( selected + 1 ) )
        files_ini = self.read_config( "push_lib" , "log_files" ).split( '|' )
        del files_ini[selected]
        self.write_config( "push_lib" , "log_files" , '|'.join( files_ini ) )
        files_ini = self.read_config( "flash_boot" , "log_files" ).split( '|' )
        del files_ini[selected]
        self.write_config( "flash_boot" , "log_files" , '|'.join( files_ini ) )

    def adb_check_device( self , ) :
        import os
        check_flag = os.getenv( 'qyh_adb_no_check' )
        if check_flag == 'true' :
            return True
        rc = self.lexec( "adb devices" )
        # print '------------'
        # print len( rc.strip().replace( "devices" , "" ).split( '\n' ) )
        # print '------------'
        # exit( )
        if len( rc.strip().replace( "devices" , "" ).split( '\n' ) ) == 2 :
            current_serial = os.getenv( 'ANDROID_SERIAL' )
            if current_serial == None or current_serial == "" : 
                self.print_green_light( "[+] device found\n" ) ;
                return True
            else :
                devices = rc.strip( ).split( '\n' )[1]
                if current_serial == devices.split( )[0] :
                    self.print_green_light( "[+] device {} found \n".format( current_serial ) ) ;
                    return True
                self.error_exit( "device {} not found".format( current_serial ) ) ;
        elif len( rc.strip().replace( "devices" , "" ).split( '\n' ) ) > 2 :
            current_serial = os.getenv( 'ANDROID_SERIAL' )
            if current_serial == None :
                self.error_exit( "multiple devices , qyh sds first" ) ;
            else :
                devices = rc.strip( ).split( '\n' )[1:]
                for device in devices :
                    if current_serial == device.split( )[0] :
                        self.print_green_light( "[+] device {} found\n".format( current_serial ) ) ;
                        return True
                self.error_exit( "device {} not found".format( current_serial ) ) ;
        else :
            self.error_exit( "device not attached ><" )

    def adb_check_root( self , ) :
        import os
        check_flag = os.getenv( 'qyh_adb_no_check' )
        if check_flag == 'true' :
            return True
        rc = self.lexec( "adb remount" ) 
        if rc.find( "succeeded" ) > 0 :
            self.print_green_light( "[+] already root\n" )
            return True
        else :
            self.error_exit( "not root ><" )

    def adb_check_fastboot_mode( self , ) :
        rc = self.lexec( "fastboot devices" )
        if rc.strip().find( "fastboot" ) > 0 :
            self.print_green_light( "[+] fastboot mode\n" )
            return True
        else :
            return False

    def adb_root_device( self , ) :
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
     
    def adb_reboot_fastboot( self , ) :    
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

    def adb_check_log( self , log_filename , keyword , k_index_start , k_index_end ) :
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

    def adb_read_log( self , log_filename , keyword , k_index_start , k_index_end ) :
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

    def adb_kill_process( self , *args ) :
    #
    #   前提：安卓设备（单一）已经打开debug模式并且root
    #   ps之后查找进程名并结束
    #
        processes = args[0]
        self.print_green_light( "\n[+] kill process : " + ' '.join( processes ) + "\n" )
        rc = self.lexec( 'adb shell ps' , False ).strip( ).split( '\n' )[1:]
        targets = []
        for line in rc :
            if line.split( )[8].split( '/' )[-1] in processes :
                targets.append( line )
        ordered_target = []
        for process in processes :
            for line in targets :
                if process in line :
                    ordered_target.append( line )
        print '\n'.join( ordered_target ) 
        for target in ordered_target : 
            kill_cmd = ""
            kill_cmd += "adb shell kill -9" + " " + target.split( )[1]
            self.lexec( kill_cmd )
            import time
            time.sleep( 0.3 )
        return

    def adb_push_lib( self , *args ) :
        '''@
        [+] callable
        [+] visible
        @short : pl
        @args : backup - backup lib file to current directory before push lib file to phone
        @args : fake - print cmd_push only, not execute it
        @args : multi_thread - using multiple thread to execute push or pull
        @'''
        import datetime , os
        self.check_args( args , ( 'backup' , 'fake' , 'dump_first' , 'multi_thread' ) )
        flag_backup , flag_fake , flag_dump_first , flag_multi_thread =\
            tuple( self.trans_args( args , ( 'backup' , 'fake' , 'dump_first' , 'multi_thread' ) ) )

        log_filename = os.getenv( 'qyh_llf' )
        if log_filename == None :
            self.error_exit( 'qyh lf_s first' )

        self.adb_check_log( log_filename , "Install:" , 0 , 8 ) 
        if not flag_fake : 
            self.adb_check_device( ) 
            self.adb_check_root( )

        if flag_backup :
            folder_name_backup = "backup_lib_" + str( datetime.datetime.now() ).split('.')[0].replace( '-' , '' ).replace( ':' , '' ).replace( ' ' , '_' )
            try :
                os.stat( folder_name_backup )
            except :
                os.makedirs( folder_name_backup )

        logs = self.adb_read_log( log_filename , "Install:" , 0 , 8 )

        import threadpool
        pool = threadpool.ThreadPool( 10 )

        if flag_backup :
            for index , log in enumerate( logs ) :
                dir = folder_name_backup + log[log.find("/system"):log.rfind("/")].strip()
                try :
                    os.stat( dir )
                except :
                    os.makedirs( dir )
                cmd_pull = 'adb pull '
                cmd_pull += log[log.find("/system"):].strip() + ' '
                cmd_pull += dir
                if not flag_multi_thread :
                    self.print_none_color( "[+] backup " )
                    self.print_white( "{:0>4}/{:0>4}".format( str( index + 1 ) , str( len( logs ) ) ) )
                    self.print_none_color( " file(s) :\n" ) ;
                    self.lexec_( cmd_pull )
                else :
                    requests = threadpool.makeRequests( self.lexec , ( cmd_pull , ) )
                    [ pool.putRequest( req ) for req in requests ]
        if flag_multi_thread : pool.wait( )


        for index , log in enumerate( logs ) :
            cmd_push = 'adb push '
            cmd_push += log_filename[:log_filename.rfind('/')] + '/'
            cmd_push += log[log.find("out"):].strip() + ' '
            cmd_push += log[log.find("/system"):log.rfind("/")].strip() + " "
            if flag_fake :
                self.print_yellow( cmd_push + "\n" )
            else :
                if not flag_multi_thread :
                    self.print_none_color( "[+] push " )
                    self.print_white( "{:0>4}/{:0>4}".format( str( index + 1 ) , str( len( logs ) ) ) )
                    self.print_none_color( " file(s) :\n" ) ;
                    self.lexec_( cmd_push )
                else :
                    requests = threadpool.makeRequests( self.lexec , ( cmd_push , ) )
                    [ pool.putRequest( req ) for req in requests ]
        if flag_multi_thread : pool.wait( )
                    
        return True

    def adb_flash_boot( self , ) :
        '''@
        [+] callable
        [+] visible
        @short : fb
        @'''
        import os
        log_filename = os.getenv( 'qyh_flf' )
        if log_filename == None :
            self.error_exit( 'qyh lf_s first' )

        self.adb_check_log( log_filename , "Target boot image:" , 0 , 18 )
        if not self.adb_check_fastboot_mode( ) :
            self.adb_check_device( )
            self.adb_reboot_fastboot( )

        self.lexec_( "fastboot bbk unlock_vivo" )
        for log in self.adb_read_log( log_filename , "Target boot image:" , 0 , 18 ) :
            cmd_flash = "fastboot flash boot "
            cmd_flash += log_filename[:log_filename.rfind('/')] + '/'
            cmd_flash += log[log.find("out"):].strip() + " "
            self.lexec_( cmd_flash )
        self.lexec_( "fastboot reboot" )

        return True

    def adb_kill_camera_svr_and_clt( self , *args ) :
        '''@
        [+] callable
        [+] visible
        @short : kc
        @'''

        self.adb_check_device( )
        self.adb_check_root( )

        processes = []
        for process_name in self.read_config( 'kill_camera_svr_and_clt' , 'camera_process_name' ).split( ' ' ) :
            processes.append( process_name )
        self.adb_kill_process( processes )
        return True

    def adb_kill_camera_service( self , ) :
        '''@
        [+] callable
        [+] visible
        @short : kcs
        @'''
        self.adb_check_device( )
        self.adb_check_root( )

        processes = []
        for process_name in self.read_config( 'kill_camera_service' , 'camera_process_name' ).split( ' ' ) :
            processes.append( process_name )
        self.adb_kill_process( processes )
        return True

    def adb_kill_camera_client( self , ) :
        '''@
        [+] callable
        [+] visible
        @short : kcc
        @'''
        self.adb_check_device( )
        self.adb_check_root( )

        processes = []
        for process_name in self.read_config( 'kill_camera_client' , 'camera_process_name' ).split( ' ' ) :
            processes.append( process_name )
        self.adb_kill_process( processes )
            
        return True

    def adb_start_camera( self , ) :
        '''@
        [+] callable
        [+] visible
        @short : sc
        @'''
        self.adb_check_device( )
        self.lexec( self.read_config( "start_camera" , "command" ) )
        return True

    def adb_take_picture( self , ) :
        '''@
        [+] callable
        [+] visible
        @short : tp
        @'''
        self.adb_check_device( )
        self.lexec( self.read_config( "take_picture" , "command" ) )
        return True

    def adb_power_button( self , ) :
        '''@
        [+] callable
        [+] visible
        @short : pb
        @'''
        self.adb_check_device( )
        self.lexec( self.read_config( "power_button" , "command" ) )
        return True

    def adb_unlock_screen( self , ) :
        '''@
        [+] callable
        @short : us
        @'''
        self.adb_check_device( )
        self.lexec( self.read_config( "unlock_screen" , "command" ) )
        return True

    def adb_screen_on( self , ) :
        '''@
        [+] callable
        [+] visible
        @short : s_on
        @'''
        '''
            adb shell dumpsys window policy :
                        mAwake  mShowingLockscreen
            screen_off  false   true
            screen_lock true    true
            on          true    false
        '''
        self.adb_check_device( )
        rc = self.lexec( "adb shell dumpsys window policy" , False , False )
        flags = [ line.split( '=' )[1] for line in rc.split( ) if line[0:6] == "mAwake" or line[0:18] == "mShowingLockscreen" ]
        # for mtk
        if len( flags ) > 2 :
            flags = flags[0:2]
        # for mtk end
        status = {
            "falsetrue" : "screen_off" ,
            "truetrue"  : "screen_lock" ,
            "truefalse" : "screen_on" ,
        }
        if status[ ''.join( flags ) ] == "screen_off" :
            self.print_green_light( '\n[+] detected screen status : off\n' )
            self.lexec( self.read_config( "power_button" , "command" ) )
            self.lexec( self.read_config( "unlock_screen" , "command" ) )
        elif status[ ''.join( flags ) ] == "screen_lock" :
            self.print_green_light( '\n[+] detected screen status : lock\n' )
            self.lexec( self.read_config( "unlock_screen" , "command" ) )
        elif status[ ''.join( flags ) ] == "screen_on" :
            self.print_green_light( '\n[+] detected screen status : on\n' )
            self.print_green_light( '\n[+] Screen already on\n' )
        else :
            self.error_exit( 'unknow status' )
        return True

    def adb_screen_off( self , ) :
        '''@
        [+] callable
        [+] visible
        @short : s_off
        @'''
        '''
            adb shell dumpsys window policy :
                        mAwake  mShowingLockscreen
            screen_off  false   true
            screen_lock true    true
            on          true    false
        '''
        self.adb_check_device( )
        rc = self.lexec( "adb shell dumpsys window policy" , False , False )
        flags = [ line.split( '=' )[1] for line in rc.split( ) if line[0:6] == "mAwake" or line[0:18] == "mShowingLockscreen" ]
        # for mtk
        if len( flags ) > 2 :
            flags = flags[0:2]
        # for mtk end
        status = {
            "falsetrue" : "screen_off" ,
            "truetrue"  : "screen_lock" ,
            "truefalse" : "screen_on" ,
        }
        if status[ ''.join( flags ) ] == "screen_off" :
            self.print_green_light( '\n[+] detected screen status : off\n' )
            self.print_green_light( '\n[+] Screen already off\n' )
        elif status[ ''.join( flags ) ] == "screen_lock" :
            self.print_green_light( '\n[+] detected screen status : lock\n' )
            self.lexec( self.read_config( "power_button" , "command" ) )
        elif status[ ''.join( flags ) ] == "screen_on" :
            self.print_green_light( '\n[+] detected screen status : on\n' )
            self.lexec( self.read_config( "power_button" , "command" ) )
        else :
            self.error_exit( 'unknow status' )
        return True

    def adb_adb_back( self , ) :
        '''@
        [+] callable
        @short : ab
        @'''
        self.adb_check_device( )
        self.lexec( self.read_config( "adb_back" , "command" ) )
        return True

    def adb_log_file( self , ) :
        '''@
        [+] callable
        [+] visible
        @short : lf
        @'''
        files = self.read_config( "push_lib" , "log_files" ).split("|")
        print( "[+] records in ini file :" )
        for index,file in enumerate( files ) :
            print "[+] {} ---> {}".format( index + 1 , file.replace( "_lib" , "_{lib,boot}" ) )
        import os
        print( "\n[+] current :" )
        log_filename = os.getenv( 'qyh_llf' )
        if log_filename == None :
            self.error_exit( 'qyh lf_s first' )
        self.print_green( "[+] push_lib   : " + os.getenv( 'qyh_llf' ) + "\n" )
        self.print_green( "[+] flash_boot : " + os.getenv( 'qyh_flf' ) + "\n" )
        return True

    def adb_check_flash_log( self , *args ) :
        '''@
        [+] callable
        [+] visible
        @short : lf_cf
        @'''
        import os
        log_filename = os.getenv( 'qyh_flf' )
        if log_filename == None :
            self.error_exit( 'qyh lf_s first' )

        self.adb_check_log( log_filename , "Target boot image:" , 0 , 18 )

        self.print_green_light( "[+] check flash log success" + "\n" ) ;
        logs = self.adb_read_log( log_filename , "Target boot image:" , 0 , 18 )
        for index , log in enumerate( logs ) :
            self.print_none_color( "[+] " )
            #  self.print_white( str( index + 1 ) + "/" + str( len( logs ) ) + " : " )
            self.print_white( "{:0>4}/{:0>4} : ".format( str( index + 1 ) , str( len( logs ) ) ) )
            self.print_none_color( log[log.find('out'):].strip() + '\n' ) ;

        return True

    def adb_log_file_check_lib( self , *args ) :
        '''@
        [+] callable
        [+] visible
        @short : lf_cl
        @args : count - print amount of lib files to be installed
        @args : list - list all lib files to be installed
        @'''
        import os
        log_filename = os.getenv( 'qyh_llf' )
        if log_filename == None :
            self.error_exit( 'qyh lf_s first' )

        self.check_args( args , ( 'count' , 'list' ) )
        self.adb_check_log( log_filename , "Install:" , 0 , 8 )

        flag_count , flag_list = tuple( self.trans_args( args , ( 'count' , 'list' ) ) )
        self.print_green_light( "[+] check lib log success" + "\n" ) ;
        if flag_count :
            logs = self.adb_read_log( log_filename , "Install:" , 0 , 8 )
            self.print_none_color( "[+] Install: " )
            self.print_white( str( len( logs ) ) )
            self.print_none_color( " file(s)\n" )
        if flag_list :
            logs = self.adb_read_log( log_filename , "Install:" , 0 , 8 )
            for index , log in enumerate( logs ) :
                self.print_none_color( "[+] " )
                #  self.print_white( str( index + 1 ) + "/" + str( len( logs ) ) + " : " )
                self.print_white( "{:0>4}/{:0>4} : ".format( str( index + 1 ) , str( len( logs ) ) ) )
                self.print_none_color( log[log.find('/system'):].strip() + '\n' ) ;

        return True

    def adb_logcat_with_dmesg( self , ) :
        '''@
        [+] callable
        [+] visible
        @short : lwd
        @'''
        self.print_none_color( self.read_config( "logcat_with_dmesg" , "command" ) + "\n" )
        return True

    def up_privilege( self , ) :
        '''@
        [+] callable
        @'''
        self.print_none_color( '\n'.join( str( f ) for f in self.read_config( "up_privilege" , "command" ).split( "\"" ) ) + '\n' )
        return True

    def mobicat( self , ) :
        '''@
        [+] callable
        @'''
        self.print_none_color( '\n'.join( str( f ) for f in self.read_config( "mobicat" , "command" ).split( "\"" ) ) + '\n' )
        return True

    def metadata( self , ) :
        '''@
        [+] callable
        @'''
        self.print_none_color( '\n'.join( str( f ) for f in self.read_config( "metadata" , "command" ).split( "\"" ) ) + '\n' )
        return True

    def awb_log( self , ) :
        '''@
        [+] callable
        @'''
        self.print_none_color( '\n'.join( str( f ) for f in self.read_config( "awb_log" , "command" ).split( "\"" ) ) + '\n' )
        return True

    def adb_dump_jpeg( self , *args ) :
        '''@
        [+] callable
        [+] visible
        @short : dj
        @args : all - dump VivoCamera's jpeg & snapdragonCamera's raw & metadata
        @args : meta - dump VivoCamera's jpeg & metadata
        @args : snapraw - dump VivoCamera's jpeg & snapdragonCamera's raw
        @args : del_only - only delete file, not dump
        @'''
        self.check_args( args , ( 'meta' , 'snapraw' , 'all' , 'del_only' ) )

        flag_meta , flag_snapraw , flag_all , flag_del_only =\
            tuple( self.trans_args( args , ( 'meta' , 'snapraw' , 'all' , 'del_only' ) ) )

        self.adb_check_device( )

        file = []

        import datetime , os
        fdst = os.getcwd( ).replace( '\\' , '/' ) + '/'
        folder_name = "dump_qyh_" + str( datetime.datetime.now() ).split('.')[0].replace( '-' , '' ).replace( ':' , '' ).replace( ' ' , '_' )
        fdst += folder_name
        fdst += '/'
        cmd = ""
        cmd += 'mkdir /sdcard/' + folder_name + '\n'

        if not flag_del_only :
            ### dump metadata
            if flag_meta or flag_all :
                #  rc = self.lexec( 'adb shell ls /data' , False )
                #  if 'snapshot' in rc : ### grep condition "no such file or directory" out
                    #  file += [ '/data/' + f for f in rc.split( ) if 'snapshot' in f ]
                #  rc = self.lexec( 'adb shell ls /data/misc/camera' , False )
                #  if '.raw' in rc or '.yuv' in rc or '.bin' in rc : ### grep condition "no such file or directory" out
                    #  file += [ '/data/misc/camera/' + f for f in rc.split( ) if '.raw' in f or '.yuv' in f or '.bin' in f ]
                cmd += 'cp /data/*snapshot* /sdcard/' + folder_name + '\n'
                cmd += 'cp /data/misc/camera/*.raw /sdcard/' + folder_name + '\n'
                cmd += 'cp /data/misc/camera/*.yuv /sdcard/' + folder_name + '\n'
                cmd += 'cp /data/misc/camera/*.bin /sdcard/' + folder_name + '\n'

            ### dump raw from Snapdragon Camera
            if flag_snapraw or flag_all :
                #  rc = self.lexec( 'adb shell ls /sdcard/DCIM/camera/raw' , False )
                #  if '.raw' in rc : ### grep condition "no such file or directory" out
                    #  file += [ '/sdcard/DCIM/camera/raw/' + f for f in rc.split( ) if '.raw' in f ]
                cmd += 'cp /sdcard/DCIM/camera/raw/*.raw /sdcard/' + folder_name + '\n'

            ### dump photo from VivoCamera
            #  rc = self.lexec( 'adb shell ls /sdcard//' + u'\u76f8\u673a'.encode('utf-8') , False )
            #  if '.jpg' in rc : ### grep condition "no such file or directory" out
                #  file += [ '/sdcard/' + u'\u76f8\u673a'.encode('utf-8') + '/' + f for f in rc.split( ) if '.jpg' in f ]
            cmd += 'cp /sdcard/' + u'\u76f8\u673a'.encode( 'utf-8' ) + '/*.jpg /sdcard/' + folder_name + '\n'

        ### tips
        #  print( "------------------ file(s) to process start ------------------" )
        #  print( '\n'.join( str( f ) for f in file ) )
        #  print( "------------------ file(s) to process end --------------------" )
        ### pull command
        #  for index , f in enumerate( file ) :
            #  print "[+] " + str( index + 1 ) + "/" + str( len( file ) ) + " file(s) :"
            #  self.lexec( 'adb pull ' + f + ' .' )
        ### rm files dumped
        #  if not flag_del_only :
        #  for index , f in enumerate( file ) :
            #  print "[+] " + str( index + 1 ) + "/" + str( len( file ) ) + " file(s) :"
            #  self.lexec( 'adb shell rm ' + f )
        if flag_meta or flag_all :
            cmd += 'rm /data/*snapshot*' + '\n'
            cmd += 'rm /data/misc/camera/*.raw' + '\n'
            cmd += 'rm /data/misc/camera/*.yuv' + '\n'
            cmd += 'rm /data/misc/camera/*.bin' + '\n'
        if flag_snapraw or flag_all :
            cmd += 'rm /sdcard/DCIM/camera/raw/*.raw' + '\n'
        cmd += 'rm /sdcard/' + u'\u76f8\u673a'.encode( 'utf-8' ) + '/*.jpg' + '\n'

        cmd += "exit\n"
        with open( folder_name , "w" ) as f :
            f.write( cmd )
        self.lexec( 'adb shell < ' + folder_name , False , True )
        print cmd
        os.remove( folder_name )
        import threadpool
        pool = threadpool.ThreadPool( 10 )
        if not flag_del_only :
            #  self.lexec( 'adb pull /sdcard/' + folder_name + ' .' )
            #  import threading
            #  threads = []
            for line in self.lexec( "adb shell ls /sdcard/" + folder_name , False ).split( "\r\r\n" ) :
                if line == "" : continue
                cmd = "adb pull /sdcard/" + folder_name + "/" + line + " ."
                requests = threadpool.makeRequests( self.lexec , ( cmd , ) )
                [ pool.putRequest( req ) for req in requests ]
                #  t = threading.Thread( target = self.lexec_ , args = ( cmd , ) )
                #  t.daemon = True
                #  threads.append( t )
            pool.wait( )

            #  n = 100
            #  for chunk in [ threads[ i : i+n ] for i in xrange( 0 , len( threads ) , n ) ] :
                #  for t in chunk :
                    #  t.start( )
                #  for t in chunk :
                    #  t.join( )
                #  print '------------ {} job(s) done ------------'.format( len( chunk ) )



        self.lexec( 'adb shell rm -rf /sdcard/' + folder_name )

        return True

    def adb_dump_lib_symbol( self , *args ) :
        '''@
        [+] callable
        [+] visible
        @short : ds
        @args : path - path to save symbol/ folder, auto mkdir if no exist
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

        log_filename = os.getenv( 'qyh_llf' )
        if log_filename == None :
            self.error_exit( 'qyh lf_s first' )
        self.adb_check_log( log_filename , "target Symbolic:" , 0 , 16 )
        logs = self.adb_read_log( log_filename , "target Symbolic:" , 0 , 16 )
        if not spe_dir :
            import datetime
            folder_name = str( datetime.datetime.now() ).split('.')[0].replace( '-' , '' ).replace( ':' , '' ).replace( ' ' , '_' )
        else :
            folder_name = args[0]
            fdst = args[0] + '/'
        for index , log in enumerate( logs ) :
            log = log.replace( '(' , '' )
            log = log.replace( ')' , '' )
            fsrc = log_filename[:log_filename.rfind('/')] + '/'
            fsrc += log[log.find("out"):].strip()
            fdst = folder_name + '/'
            fdst += log[log.find("/symbols")+1:log.rfind("/")].strip() + "/"
            try :
                os.stat( fdst )
            except :
                os.makedirs( fdst )
            self.print_none_color( "[+] copy " )
            #  self.print_white( str( index + 1 ) + "/" + str( len( logs ) ) )
            self.print_white( "{:0>4}/{:0>4}".format( str( index + 1 ) , str( len( logs ) ) ) )
            self.print_none_color( " file(s) : " + fsrc[fsrc.find('/symbols'):] + '\n' ) ;
            shutil.copy( fsrc , fdst )
        print "\n[+] dump to {}/".format( folder_name )

        return True

    def toolkit_copy_with_log( self , args ) :
        import shutil
        fsrc = args.split( '|' )[0]
        fdst = args.split( '|' )[1]
        rc = shutil.copy( fsrc , fdst )
        self.print_none_color( "[+] copyed : {}\n".format( fsrc ) )
        return rc

    def adb_dump_lib( self , *args ) :
        '''@
        [+] callable
        [+] visible
        @short : dl
        @args : path - path to save system/ folder, auto mkdir if no exist
        @'''
        import os 
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

        log_filename = os.getenv( 'qyh_llf' )
        if log_filename == None :
            self.error_exit( 'qyh lf_s first' )
        self.adb_check_log( log_filename , "Install:" , 0 , 8 )
        logs = self.adb_read_log( log_filename , "Install:" , 0 , 8 )
        if not spe_dir :
            import datetime
            folder_name = str( datetime.datetime.now() ).split('.')[0].replace( '-' , '' ).replace( ':' , '' ).replace( ' ' , '_' )
        else :
            folder_name = args[0]
        #  threads = []
        import threadpool
        pool = threadpool.ThreadPool( 10 )
        for index , log in enumerate( logs ) :
            fsrc = log_filename[:log_filename.rfind('/')] + '/'
            fsrc += log[log.find("out"):].strip()
            fdst = folder_name + '/'
            fdst += log[log.find("/system")+1:log.rfind("/")].strip() + "/"
            try :
                os.stat( fdst )
            except :
                os.makedirs( fdst )
            #  self.print_none_color( "[+] copy " )
            #  self.print_white( "{:0>4}/{:0>4}".format( str( index + 1 ) , str( len( logs ) ) ) )
            #  self.print_none_color( " file(s) : " + fsrc[fsrc.find('/system'):] + '\n' ) ;
            #  shutil.copy( fsrc , fdst )
            requests = threadpool.makeRequests( self.toolkit_copy_with_log , ( fsrc + '|' + fdst , ) )
            [ pool.putRequest( req ) for req in requests ]
        pool.wait( )
            #  import threading
            #  t = threading.Thread( target = self.toolkit_copy_with_log , args = ( fsrc , fdst ) )
            #  t.daemon = True
            #  threads.append( t )

        #  n = 100
        #  for chunk in [ threads[ i : i+n ] for i in xrange( 0 , len( threads ) , n ) ] :
            #  for t in chunk :
                #  t.start( )
            #  for t in chunk :
                #  t.join( )
            #  print '------------ {} job(s) done ------------'.format( len( chunk ) )
        print "\n[+] dump to {}/".format( folder_name )

        return True

class qyh_svr( qyh_base ) :
    from SocketServer import TCPServer , ThreadingMixIn , BaseRequestHandler

    def __init__( self ) :
        pass
        super( qyh_svr , self ).__init__( ) ;
        self.svr_addr = self.svr_getaddr( )

    def check_white_list( self , host = "127.0.0.1" ) :
        white_list = self.read_config( "white_list" , "hosts" ).split( '|' )
        if host in white_list :
            return True
        else :
            self.log_to_black_list( host )
            return False

    def log_to_black_list( self , host ) :
        black_list = self.read_config( "black_list" , "hosts" ).split( '|' )
        if host not in black_list :
            black_list.append( host )
            self.write_config( "black_list" , "hosts" , '|'.join( black_list ) )

    def log_rejection( self , host , port , data ) :
        import datetime , os
        log = str( datetime.datetime.now() ).split( '.' )[0] + ' '
        log += 'from {} : {}\n'.format( host , port )
        log += 'data : '
        with open( os.path.dirname( __file__ ) + '\qyh_svr_rejection.log' , 'a+' ) as f :
            f.write( log )
            f.write( data )
            f.write( '\n' + '\n' )

    class ThreadedTCPRequestHandler( BaseRequestHandler ) :
        def handle( self ) :
            import threading , os , subprocess , sys
            svr = qyh_svr( )
            data = self.request.recv( 65536 )
            cur_thread = threading.current_thread( )
            if svr.check_white_list( self.client_address[0] ) :


                # import shlex , select
                # process = subprocess.Popen( shlex.split( data ), shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT )
                # while True:
                    # output = process.stdout.readline()
                    # if output == '' and process.poll() is not None:
                        # break
                    # if output:
                        # # print output
                        # sys.stdout.write(output)
                        # sys.stdout.flush()
                        # self.request.sendall( output )

                self.request.sendall( '[!] ' + data + '\r\n' + os.popen( data ).read( ) )
            else :
                svr.log_rejection( self.client_address[0] , self.client_address[1] , data )

    class ThreadedTCPServer( ThreadingMixIn , TCPServer ) :
        pass

    def svr_check_time( self ) :
        '''@
        [+] callable
        @'''
        import datetime , time , subprocess , shlex
        from sys import platform as _platform
        if not _platform == "win32" :
            print '[-] not windows ><'
        hold = [
            '0650' , '0720' , '0750' , '0840' , '0935' , '1025' , '1115' ,
            '1440' , '1535' , '1625' ,
            '1830' , '1930' , '2000' , '2055' , '2150' , 
        ]
        loosen = [
            '0740' , '0830' , '0920' , '1015' , '1105' , '1155' ,
            '1520' , '1615' , '1705' ,
            '1950' , '2045' , '2140' , '2230' 
        ]
        while True :
            print "checking..."
            now = ''.join( str( datetime.datetime.now( )).split( )[1].split( '.' )[0].split( ':' ) )[0:4]
            if now in hold : 
                # os.popen( 'msg %username% "hold"' )
                # self.log_rejection( 'a' , 'hold' , str( datetime.datetime.now( ) ) )
                subprocess.Popen( shlex.split( 'msg Administrator "hold"' ) )
            if now in loosen :
                # os.popen( 'msg %username% "loosen"' )
                # self.log_rejection( 'b' , 'loosen' , str( datetime.datetime.now( ) ) )
                subprocess.Popen( shlex.split( 'msg Administrator "loosen"' ) )
            # self.log_rejection( 'c' , 'check' , str( datetime.datetime.now( ) ) )
            time.sleep( 44 )
            #  self.svr_check_time( )

    def svr_set_timer( self , *args ) :
        '''@
        [+] callable
        [+] visible
        @short : svst
        @'''
        import subprocess
        if self.svr_check( False ) :
            # self.print_green( 'starting timer...\n' )
            self.svr_exec( False , "qyh svr_set_timer_real " + ' '.join( args ) )
        else :
            self.error_exit( "qyh_svr daemon down" )

    def svr_set_timer_real( self , *args ) :
        '''@
        [+] callable
        @short : svstr
        @args : time - timer's time
        @args : msg - message to show
        @'''
        from threading import Timer
        if len( args ) != 2 :
            self.error_exit( 'take two args : time msg' )
        try :
            split = int( args[0] )
        except :
            self.error_exit( 'take integer time as seconds to set' )

        Timer( split , self.tick , ( (args[1],) ) ).start( )

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
            self.print_red( '[-] qyh_svr daemon die\n' )
            self.print_none_color( '[+] addr seted in ini file : '.format( addr ) )
            self.print_green_light( "{}\n".format( addr ) )
            exit()

    def svr_reset( self , output = True , flag_kill = True ) :
        '''@
        [+] callable
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
        import sys
        sys.stdout.flush( )
        ip_ = raw_input( "[+] ip : " + ip + " ?\n" )
        if len( ip_ ) > 0 :
            ip = ip_
        port = 2334
        import sys
        sys.stdout.flush( )
        port_ = raw_input( "[+] port : 2334 ?\n" )
        if len( port_ ) > 0 :
            port = int( port_ )
        addr = ( ip , port )
        # if socket.socket( socket.AF_INET , socket.SOCK_STREAM ).connect_ex( addr ) == 0 :
            # self.error_exit( '[-] addr : ' + ':'.join(str(i) for i in addr) + ' not available\n' )
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

    def depress_surveillance( self ) :
        '''@
        [+] callable
        @short : X
        @'''
        import os
        while True :
            try :
                process_name = "ONacAgent.exe"
                print os.popen( 'taskkill /F /IM ' + process_name + ' /T' ).read( )
                process_name = "winrdlv3.exe"
                print os.popen( 'taskkill /F /IM ' + process_name + ' /T' ).read( )
            except :
                pass

    def svr_start( self ) :
        '''@
        [+] callable
        [+] visible
        @short : svs
        @'''
        import subprocess , os , sys
        cmd = 'pythonw ' + os.path.dirname( __file__ ).replace( '\\' , '/' ) + '/qyh.py depress_surveillance'
        # process = subprocess.Popen( cmd )
        if not self.svr_check( False ) :
            self.svr_reset( False , flag_kill = False)
            port = self.svr_getaddr( )[1]
            cmd_fwall_pass = 'netsh advfirewall firewall add rule name="qyh_svr" dir=out program="' + str( sys.executable ).replace( 'python' , 'pythonw' ) + '" security=authenticate action=allow'.format( sys.executable )
            self.lexec( cmd_fwall_pass , False , False )
            cmd_fwall_pass = 'netsh advfirewall firewall add rule name="qyh_svr" protocol=TCP dir=in localport={} action=allow'.format( port )
            self.lexec( cmd_fwall_pass , False , False )
            from sys import platform as _platform
            if _platform == "win32" :
                cmd = 'pythonw ' + os.path.dirname( __file__ ).replace( '\\' , '/' ) + '/qyh.py svr_check_time'
                process = subprocess.Popen( cmd )
                cmd = 'pythonw ' + os.path.dirname( __file__ ).replace( '\\' , '/' ) + '/qyh.py svd'
                process = subprocess.Popen( cmd )
            elif _platform == "cygwin" :
                cmd = "{} {}".format( sys.executable , os.path.dirname( __file__ ) + '/qyh.py svr_check_time &' )
                os.popen( cmd )
                cmd = "{} {}".format( sys.executable , os.path.dirname( __file__ ) + '/qyh.py svd &' )
                os.popen( cmd )
            else :
                print "didn't implement yet"
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
        # pid = self.svr_getpid( )
        # if pid > 0 :
        try :
            process_name = "pythonw.exe"
            print os.popen( 'taskkill /F /IM ' + process_name + ' /T' ).read( )
            os.kill( pid , signal.SIGTERM )
            self.write_config( "svr" , "pid" , -1 )
        except :
            pass
        try :
            process_name = "python"
            print os.popen( 'taskkill /F /IM ' + process_name + ' /T' ).read( )
            os.kill( pid , signal.SIGTERM )
            self.write_config( "svr" , "pid" , -1 )
        except :
            pass

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

class qyh_php( qyh_base ) :

    def php_tail_error_logs( self , ) :
        '''@
        [+] callable
        [+] visible
        @short : tel
        @'''
        import os
        if os.getenv( 'APACHE_HOME' ) == None :
            self.error_exit( "cannot detect apache home" ) ;
        self.lexec_( "tail -f \"{}/logs/error.log\"".format( str( os.getenv( 'APACHE_HOME' ) ).replace( '\\' , '/' ) ) )

    def php_tail_access_logs( self , ) :
        '''@
        [+] callable
        [+] visible
        @short : tal
        @'''
        import os
        if os.getenv( 'APACHE_HOME' ) == None :
            self.error_exit( "cannot detect apache home" ) ;
        self.lexec_( "tail -f \"{}/logs/access.log\"".format( str( os.getenv( 'APACHE_HOME' ) ).replace( '\\' , '/' ) ) )

class qyh( qyh_svr , qyh_adb , qyh_php ) :

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

    def call_log( self , func_name ) :
        import os
        from sys import platform as _platform
        if _platform == "win32" or _platform == 'cygwin' :
            identity = os.getenv( 'identity' ) or os.getenv( 'IDENTITY' ) 
            if identity in ( 'Shadow' , 'vivo_work' ) :
                try :
                    pre_count = self.read_config( "read_config_" + identity , func_name )
                except :
                    pre_count = 0
                    self.write_config( "read_config_" + identity , func_name , pre_count )
                self.write_config( "read_config_" + identity , func_name , int(pre_count) + 1 )

    def main_loop( self , *args ) :
        if len( args ) > 1 :
            funcs = tuple( f for f in dir( self ) if not f.startswith( "_" ) and self.check_callable( "self." + f ) )
            func_map = { f if self.get_formated_args( "self." + f )[0] == "" else self.get_formated_args( "self." + f )[0] : f for f in funcs }
            self.check_args( args[1:2] , tuple( s for t in func_map.items( ) for s in t ) )
            func_name = "self." + ( func_map[ args[1] ] if args[1] in func_map else args[1] )
            self.call_log( func_name[5:] )
            #  import time
            #  time_start = time.time( )
            if len( args ) > 2 :
                f = eval( func_name )
                f( *args[2:] )
            else :
                f = eval( func_name )
                f( )
            #  self.print_green( "\n\n\n[+] function {} used {} second(s)\n".format( func_name , time.time( ) - time_start ) )
        else :
            self.main_menu( )

if __name__ == "__main__" :
    import sys
    qyh( ).main_loop( *sys.argv )
