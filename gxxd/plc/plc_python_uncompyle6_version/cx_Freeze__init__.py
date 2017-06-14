# uncompyle6 version 2.10.0
# Python bytecode 3.4 (3310)
# Decompiled from: Python 3.4.0 (v3.4.0:04f714765c13, Mar 16 2014, 19:24:06) [MSC v.1600 32 bit (Intel)]
# Embedded file name: C:\Python34\lib\site-packages\cx_Freeze\initscripts\Console.py
# Compiled at: 2016-06-29 13:18:46
import os
import sys
import zipimport
sys.frozen = True
sys.path = sys.path[:4]
os.environ['TCL_LIBRARY'] = os.path.join(DIR_NAME, 'tcl')
os.environ['TK_LIBRARY'] = os.path.join(DIR_NAME, 'tk')
m = __import__('__main__')
importer = zipimport.zipimporter(INITSCRIPT_ZIP_FILE_NAME)
if INITSCRIPT_ZIP_FILE_NAME != SHARED_ZIP_FILE_NAME:
    moduleName = m.__name__
else:
    name, ext = os.path.splitext(os.path.basename(os.path.normcase(FILE_NAME)))
    moduleName = '%s__main__' % name
code = importer.get_code(moduleName)
exec(code, m.__dict__)
versionInfo = sys.version_info[:3]
if versionInfo >= (2, 5, 0) and versionInfo <= (2, 6, 4):
    module = sys.modules.get('threading')
    if module is not None:
        module._shutdown()
