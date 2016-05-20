#!/usr/bin/env pythoy
# -*- coding: gbk -*-
__author__ = 'QiYunhu-13111020'
'''
    三个类 Metro SellingRecord dbOperator
        Metro里边是售票机主要能实现的功能
        SellingRecord类用来封装每次成功交易之后的交易详情
        dbOperator用来实现和数据库有关的操作

                            2014-12-14
'''

import sqlite3 , time , platform , os , getpass
from slowPrint import slowPrint

#linux下中文使用utf-8编码输出到命令行
#windows下中文使用gbk编码
Os = platform.system( )
if( Os == 'Linux' ) :
    CODING = 'utf-8'
elif( Os == 'Windows' ) :
    CODING = 'gbk'
else :
    CODING = 'utf-8'


class Metro :

#初始化函数
    def __init__( self ) :
        self.db = dbOperator( )
        self.record = SellingRecord( )

#实现清屏功能
    def cls( self ) :
        Os = platform.system( )
        if( Os == 'Linux' ) :
            os.system( 'clear' )
        if( Os == 'Windows' ) :
            os.system( 'cls' )

#输出欢迎信息
    def tips( self ) :
        self.cls( )
        slowPrint( '\n' ) 
        slowPrint( ' Welcome! \n' ) 
        slowPrint( ' This station is [ %s ] \n' % self.db.GetThisStationName( )  )
        slowPrint( ' This machine has [ %d ] tickets now\n' % self.db.GetTicketLeft( ) )
        slowPrint( ' Today is %s \n' % ( time.strftime( "%Y-%m-%d" , time.localtime( time.time() ) ) ) )
        slowPrint( ' You can go back by inputing \'#\' at any time\n' )
        slowPrint( '\n' )

#管理员模式的欢迎信息
    def adminTips( self ) :
        self.cls( )
        slowPrint( '\n' ) 
        slowPrint( ' [+] Welcome! \n' ) 
        slowPrint( ' [+] This station is [ %s ] \n' % self.db.GetThisStationName( )  )
        slowPrint( ' [+] This machine has [ %d ] tickets now\n' % self.db.GetTicketLeft( ) )
        slowPrint( ' [+] Today is %s \n' % ( time.strftime( "%Y-%m-%d" , time.localtime( time.time() ) ) ) )
        slowPrint( ' [+] You can go back by inputing \'#\' at any time\n' )
        slowPrint( ' [+]  Careful! This is administrate mode\n' )
        slowPrint( '\n' )

#可选的操作
    def services( self ) :
        slowPrint( '\n' ) 
        slowPrint( '   Services : \n' )
        slowPrint( '\n' )
        slowPrint( '    1. Buy metro ticket\n' )
        slowPrint( '    2. Manager this mechine\n' )    

#管理员可选的操作
    def adminServices( self ) :
        slowPrint( '\n' )
        slowPrint( ' [+]  Services : \n' )
        slowPrint( '\n' )
        slowPrint( ' [+]   1. Print Selling Record\n' )
        slowPrint( ' [+]   2. Add tickets to this machine\n' )    
        slowPrint( ' [+]   3. Change this station name\n' )

#找钱或者退钱
    def change( self , money ) :
        slowPrint( '\n' )
        slowPrint( '----------------------------------------------------\n' )
        slowPrint( '  This is your money for change/refund : [ %d ]\n' % money )
        slowPrint( '----------------------------------------------------\n' )
        slowPrint( '\n' )

#出票
    def printTicket( self , begin , end , price ) :
        slowPrint( '\n' )
        slowPrint( '  This is your ticket , have a nice trip \n' )
        slowPrint( '\n' )
        slowPrint( '  +--------------------------------------+ \n' )
        slowPrint( '  |            Metro Ticket              | \n' )
        slowPrint( '  +--------------------------------------+ \n' )
        slowPrint( '  |  %s -> %s \n' % ( begin , end ) )
        slowPrint( '  |  price : %d                           |\n' % price )
        slowPrint( '  +--------------------------------------+ \n' )

#打印可选择的站点
    def PrintStationNameList( self ) :
        nameList = self.db.GetStationNameList( )
        index = 0 ;
        while( index < nameList.__len__( ) ) :
            if( index % 4 == 0 ) :
                if( index + 4 < nameList.__len__( ) ) :
                    slowPrint( '\n    %2d ~ %2d : ' % ( index + 1 , index + 4 ) , 0.00025 )
                else :
                    slowPrint( '\n    %02d ~ %02d : ' % ( index + 1 , nameList.__len__( ) ) , 0.00025 )
            slowPrint( '%s  ' % nameList[index] , 0.00025 )
            index = index + 1 
        slowPrint( '\n' )

#打印历史交易记录
#每打印七条暂停一下防止数据太多看不到上边的信息
    def PrintRecord( self ) :
        recordList = self.db.GetRecord( )
        index = 0 
        while( index < recordList.__len__() ) :
            slowPrint( ' ' , 0.05 )
            print( recordList[index] )
            if( ( index + 1 ) % 7 == 0 ) :
                input_ = raw_input( '\nEnter to continue, input \'#\' to stop' )
                if( input_ == '#' ) : break
            index = index + 1 
        input_ = raw_input( '\n...END Press any key to go back' )

#进入管理员模式的时候进行身份验证
#默认密码admin
    def OpenAdminMode( self ) :
        getPass = getpass.getpass( )
        password = getPass( "#" )
        if( password == 'admin' ) :
            return True
        return False
        
#用于存储成功的交易记录
class SellingRecord :
#起点站（本站）的访问器和设置器
    def SetBeginStation( self , begin ) :
        self.__beginStation = begin
    def GetBeginStation( self ) :
        return self.__beginStation
#终点站的访问器和设置器
    def SetEndStation( self , end ) :
        self.__endStation = end
    def GetEndStation( self ) :
        return self.__endStation
#投入钱币的访问器和设置器
    def SetMoneyGained( self , money ) :
        self.__moneyGained = money
    def GetMoneyGained( self ) :
        return self.__moneyGained
#票价的访问器和设置器
    def SetPrice( self , price ) :
        self.__price = price
    def GetPrice( self ) :
        return self.__price
#应找零钱的访问器和设置器
    def SetMoneyForChange( self , money ) :
        self.__moneyForChange = money
    def GetMoneyForChange( self ) :
        return self.__moneyForChange
#完成交易的时间设置器
    def SetCompletedTime( self , time ) :
        self.__completedTime = time 
#相当于java的toString()方法
    def __str__( self ) :
        return ' %s price:%-3d gain:%-3d change:%-3d %s -> %s' % \
            ( self.__completedTime , self.__price , self.__moneyGained , 
              self.__moneyForChange , self.__beginStation , self.__endStation )

class dbOperator :

#构造函数，获得数据库连接和游标
    def __init__( self ) :
        self.__conn = sqlite3.connect( 'metro.db' )
        self.__conn.text_factory = lambda x: unicode( x , CODING , "ignore" ) 
        self.__cursor = self.__conn.cursor( )

#析构函数，关闭游标和数据库连接
    def __del__( self ) :
        self.__cursor.close( )
        self.__conn.close( )
        
#测试用函数
    def test( self ) :
        self.__cursor.execute( "select * from METROPRICE\
                                where start = '小寨' \
                                limit 0 , 5 " )
        str = self.__cursor.fetchall( )
        for i in str :
            for j in i :
                print j 
            print( "----" )

#获得所有站点的名字，返回一个List
    def GetStationNameList( self ) :
        self.__cursor.execute( "select distinct start \
                                from METROPRICE" ) 
        tmp = self.__cursor.fetchall( ) 
        r = []
        for i in tmp :
            r.append( i[0].encode( CODING ) )
        return r

#参数是起点站（本站）和终点站的名字
#返回值是票价
    def CalcPrice( self , begin , end ) :
        self.__cursor.execute( "select * from METROPRICE\
                                where start = ? and end = ?",
                                ( begin , end ) )
        return self.__cursor.fetchall( )[0][2]

#设置本机器剩余票数量
    def SetTicketLeft( self , ti ) :
        self.__cursor.execute( "update CONTEXT \
                                set NUM_VALUE = ?\
                                where name = 'TICKETS'" , (ti,) )
        self.__conn.commit( )

#获得本机器剩余的票数
    def GetTicketLeft( self ) :
        self.__cursor.execute( "select NUM_VALUE from CONTEXT\
                                where NAME = 'TICKETS'" )
        return self.__cursor.fetchall( )[0][0]

#设置本站名称
    def SetThisStationName( self , name ) :
        self.__cursor.execute( "update CONTEXT\
                                set STR_VALUE = ?\
                                where name = 'THIS_STATION'" , (name,) )
        self.__conn.commit( )

#获得本站名字
    def GetThisStationName( self ) :
        self.__cursor.execute( "select STR_VALUE from CONTEXT\
                                where name = 'THIS_STATION'" )
        return self.__cursor.fetchall( )[0][0].encode( CODING )

#保存交易记录
#（简化成保存字符串。。）
    def SaveRecord( self , record ) :
        self.__cursor.execute( "INSERT INTO SELLINGRECORD( RECORD ) VALUES ( ? )" , (record.__str__(),) )
        self.__conn.commit( )

#获得交易记录
#返回值是一个List
    def GetRecord( self ) :
        self.__cursor.execute( "SELECT RECORD FROM SELLINGRECORD" )
        tmp = self.__cursor.fetchall( )
        r = []
        for i in tmp :
            r.append( i[0] )
        return r

#测试用
if __name__ == '__main__' :
    a = dbOperator( )
    # a.test( )
    # a.SetTicketLeft( 2 )
    # print a.GetTicketLeft( )
    # a.SetTicketLeft( 4 )
    # print a.GetTicketLeft( )
    # print a.CalcPrice( '小寨' , '小寨' )
    # a.SetThisStationName( '汉城路' ) 
    # print a.GetThisStationName( )
    # a.SetThisStationName( '小寨' ) 
    # print a.GetThisStationName( )
    # for i in a.GetStationNameList( ) :
        # print i 
    # metro = Metro( )
    # metro.PrintStationNameList( )
