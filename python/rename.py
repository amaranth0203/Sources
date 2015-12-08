#-*- coding=utf-8 -*-

# 去掉开头的字符串

import os
import sys
for dirname, dirnames, filenames in os.walk('.') :
    for filename in filenames :
        if ".py" not in filename :
            os.rename( filename , filename[28:] )