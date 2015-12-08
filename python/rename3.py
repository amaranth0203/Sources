#-*- coding=utf-8 -*-

#用来去掉从gerrit下来的代码后边带有长长的乱码后缀
#去掉从最后一个横杠（含）到扩展名的点之间的字符

import os
import sys
for dirname, dirnames, filenames in os.walk('.') :
    for filename in filenames :
        if ".py" not in filename :
            # print filename[:filename.rfind('-')] + filename[filename.rfind('.'):]
            os.rename( filename , filename[:filename.rfind('-')] + filename[filename.rfind('.'):] )