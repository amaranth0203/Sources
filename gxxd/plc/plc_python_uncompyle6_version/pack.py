
# 打包方法 :
# python pack.py py2exe

from distutils.core import setup  
import py2exe  

import os
data_files = []
image = [] 
imageformats = []
for files in os.listdir( "image\\" ) :
    f1 = "image\\" + files
    if os.path.isfile( f1 ) :
        f2 = 'image' , [f1]
        data_files.append( f2 )
for files in os.listdir( "imageformats\\" ) :
    f1 = "imageformats\\" + files
    if os.path.isfile( f1 ) :
        f2 = 'imageformats' , [f1]
        data_files.append( f2 )
data_files.append( ( '' , [ 'config.ini' ] ) )
data_files.append( ( '' , [ 'Don_API.dll' ] ) )
data_files.append( ( '' , [ 'tks7.dll' ] ) )

setup(
    options={"py2exe" : {
        "includes" : ["sip","PyQt4.QtNetwork"] ,
    }} ,
    windows=[{
        "script":"plcservice__main__.py",
        "icon_resources":[(1,"image\\singleM.ico")],
        "dest_base":"PlcService",
    }] ,
    data_files = data_files ,
)
