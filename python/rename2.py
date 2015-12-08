#-*- coding=utf-8 -*-

#把文件名中的 “几十几” 替换成数字
#只有两个字，比如 “十三”之类的要更改下标

import os
import sys
index = { 
    "一" : "1" ,
    "二" : "2" ,
    "三" : "3" ,
    "四" : "4" ,
    "五" : "5" ,
    "六" : "6" ,
    "七" : "7" ,
    "八" : "8" ,
    "九" : "9" , }
for dirname, dirnames, filenames in os.walk('.') :
    for filename in filenames :
        if ".py" not in filename :
            os.rename( filename , filename[:2] + index[filename[2:4]] + index[filename[6:8]] + filename[8:] )
#            print filename[2:8] + " -> " + index[filename[2:4]] + index[filename[6:8]]
#            print filename[:2] + index[filename[2:4]] + index[filename[6:8]] + filename[8:]