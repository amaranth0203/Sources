#!/usr/bin/env python
#-*- coding: utf-8 -*-
__author__ = 'QiYunhu-13111020'
'''
    用于初始化数据库
    price.xls是从百度找到的西安地铁价格表
    简单地从里边读出数据用sqlite3存储到metro.db的METROPRICE表中
    CONTEXT表用于保存余票数量和本站名称
        默认本站为'小寨'，余票2张
    SELLINGRECORD表用于保存交易记录
                            2014-11-25
'''
import xlrd , sqlite3, platform

#linux下中文使用utf-8编码输出到命令行
#windows下中文使用gbk编码
Os = platform.system( )
if( Os == 'Linux' ) :
    CODING = 'utf-8'
elif( Os == 'Windows' ) :
    CODING = 'gbk'


fname = "price.xls"
#获取excel指针
bk = xlrd.open_workbook( fname )
shxrange = range( bk.nsheets )
#打开工作薄
try :
    sh = bk.sheet_by_name( "Sheet1" )
except :
    print "No sheet in %s named Sheet1" % fname
#获取行数和列数
nrows = sh.nrows
ncols = sh.ncols
#构造val用于创建METROPRICE表
val = []
#各个地铁站名字
names = sh.row_values( 1 )
for i in range( 2 , nrows - 1 ) :
    for j in range( 3 , ncols ) :
        start = sh.row_values( i )[ 2 ]
        end = names[ j ]
        price = sh.row_values( i )[ j ] 
        val.append( [ start.encode( CODING ) , end.encode( CODING ) , price if price != '-' else 0 ] )

#打开数据库
conn = sqlite3.connect( 'metro.db' )
#windows下使用gbk编码
conn.text_factory = lambda x: unicode( x , CODING , "ignore" ) 
#获取游标
cursor = conn.cursor( )
#建METROPRICE表
cursor.execute( "DROP TABLE IF EXISTS METROPRICE" )
cursor.execute( "CREATE TABLE METROPRICE (\
                    START VARCHAR( 30 ) ,\
                    END VARCHAR( 30 ) ,\
                    PRICE INT )" )
sql = '''INSERT INTO METROPRICE VALUES( ? , ? , ? )'''
#插入数据
cursor.executemany( sql , tuple( val ) )

#建CONTEXT表
#默认起点站（本站）为小寨
#默认余票有两张
cursor.execute( "DROP TABLE IF EXISTS CONTEXT" )
cursor.execute( "CREATE TABLE CONTEXT( NAME VARCHAR(30), NUM_VALUE INT , STR_VALUE VARCHAR  )" )
cursor.execute( "INSERT INTO CONTEXT( NAME , NUM_VALUE ) \
                 VALUES ( 'TICKETS' , 2 )" )
cursor.execute( "INSERT INTO CONTEXT( NAME , STR_VALUE ) \
                 VALUES ( 'THIS_STATION' , '小寨' )" )

#建SELLINGRECORD表
cursor.execute( "DROP TABLE IF EXISTS SELLINGRECORD" )
cursor.execute( "CREATE TABLE SELLINGRECORD(\
                    ID INTEGER PRIMARY KEY,\
                    RECORD VARCHAR)" )
cursor.close( )
conn.commit( )
conn.close( )
