#-*- coding=utf-8 -*-

'''
    提取 excel 中的联系方式 制作易企秀（莫站让娟帮做）
    再对比 excel 和照片中的名字，寻找差异和没照片/联系方式的人

									2018.02.06
'''

import xlrd
book = xlrd.open_workbook('1213班毕业40周年同学聚会通讯录.xls')

'''
max_nb_row = 0
for sheet in book.sheets() :
    max_nb_row = max( max_nb_row , sheet.nrows )

#for row in range( max_nb_row ) :
#    for sheet in book.sheets( ) :
#        if row < sheet.nrows :
#            print sheet.row( row )[1].value

with open( 'output.txt' , 'wb' ) as f :
    for i in range( 2 , 34 ) :
        f.write(( u'姓名：'+book.sheets( )[1].row( i )[1].value + u'\r\n班级：高13班\r\n电话：' + book.sheets( )[1].row( i )[3].value + u'\r\n住址：' + book.sheets( )[1].row( i )[4].value+'\r\n\r\n' ).encode( 'utf8' ))

with open( 'output2.txt' , 'wb' ) as f :
    for i in range( 2 , 30 ) :
        f.write( ( book.sheets()[0].row(i)[1].value + '\r\n' ).encode( 'utf8' ) )
with open( 'output2.txt' , 'awb' ) as f :
    for i in range( 2 , 34 ) :
        f.write( ( book.sheets()[1].row(i)[1].value + '\r\n' ).encode( 'utf8' ) )



import os
import sys
with open( 'output2.txt' , 'wb' ) as f :
    for dirname , dirnames , filenames in os.walk( u'单人一寸照' ) :
        for filename in filenames :
            if "jpg" in filename.lower( ) :
                f.write( ( filename.split('.')[0] + '\r\n' ).encode( 'utf-8' ) )
'''

name_in_excel = []
name_in_photo = []
with open( 'name_in_excel.txt' , 'r' ) as f :
    for line in f.readlines( ) :
        name_in_excel.append( line.strip() )


with open( 'name_in_photo.txt' , 'r' ) as f :
    for line in f.readlines( ) :
        name_in_photo.append( line.strip() )

for name in name_in_photo :
    if name not in name_in_excel :
        print name
