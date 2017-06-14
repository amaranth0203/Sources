# -*- coding:gbk -*-
# uncompyle6 version 2.10.0
# Python bytecode 3.4 (3310)
# Decompiled from: Python 3.4.0 (v3.4.0:04f714765c13, Mar 16 2014, 19:24:06) [MSC v.1600 32 bit (Intel)]
# Embedded file name: D:\PlcService1200\DonLib.py
# Compiled at: 2015-07-31 14:58:18
import sys
import os
from os.path import getsize
import time
from ctypes import *

class _dongleinfo(Structure):
    _fields_ = [
     (
      'ver', c_int),
     (
      'type', c_int),
     (
      'birthday', c_byte),
     (
      'pid', c_long),
     (
      'userid', c_long),
     (
      'hid', c_byte),
     (
      'ismother', c_long),
     (
      'devtype', c_long)]

    def __str__(self):
        return 'ver:{0}-pid:{1}-userid:{2}'.format(self.ver, self.pid, self.userid)


try:
    hinst = windll.LoadLibrary('Don_API.dll')
except Exception as e:
    hinst = None

DON_SUCCESS = 0
DON_NOT_FOUND = 4026531841
DON_INVALID_PARAMETER = 4026531842
DON_COMM_ERROR = 4026531843
DON_INSUFFICIENT_BUFFER = 4026531844
DON_NO_LIST = 4026531845
DON_DEVPIN_NOT_CHECK = 4026531846
DON_USERPIN_NOT_CHECK = 4026531847
DON_RSA_FILE_FORMAT_ERROR = 4026531848
DON_DIR_NOT_FOUND = 4026531849
DON_ACCESS_DENIED = 4026531850
DON_ALREADY_INITIALIZED = 4026531851
DON_INCORRECT_PIN = 4026534912
DON_DF_SIZE = 4026531853
DON_FILE_EXIST = 4026531854
DON_UNSUPPORTED = 4026531855
DON_FILE_NOT_FOUND = 4026531856
DON_ALREADY_OPENED = 4026531857
DON_DIRECTORY_EXIST = 4026531858
DON_CODE_RANGE = 4026531859
DON_INVALID_POINTER = 4026531860
DON_GENERAL_FILESYSTEM = 4026531861
DON_OFFSET_BEYOND = 4026531862
DON_FILE_TYPE_MISMATCH = 4026531863
DON_PIN_BLOCKED = 4026531864
DON_INVALID_HANDLE = 4026531865
DON_ERROR_UNKNOWN = 4294967295
DongleHandle = c_int(0)

def hasDongle(productId):
    """检查产品的狗，并打开"""
    if not hinst:
        return False
    nCount = c_int(0)
    ret = hinst.DON_Find(productId.encode('ascii'), byref(nCount))
    if ret or nCount.value < 1:
        return False
    return True


def checkUserPass(userPass):
    """检查用户密码"""
    ret = hinst.DON_Open(byref(DongleHandle), 1)
    if ret:
        return False
    nRemainCount = c_int(0)
    ret = hinst.DON_VerifyPIN(DongleHandle, 0, userPass.encode('ascii'), byref(nRemainCount))
    if ret:
        return False
    return True


def getDeadline():
    """获取过期时间"""
    nTime = c_long(0)
    ret = hinst.DON_GetDeadline(DongleHandle, byref(nTime))
    if ret:
        return False
    if nTime.value == -1:
        return False
    t = time.localtime(nTime.value)
    return t


def closeDongle():
    hinst.DON_Close(DongleHandle)


if __name__ == '__main__':
    nCount = c_int(0)
    ret = hinst.DON_Find('4187F7D1'.encode('ascii'), byref(nCount))
    print('DON_Find return code:{0:x}'.format(ret & 4294967295))
    print('Find {0} dongle'.format(nCount.value))
    if ret:
        print('ERR')
    print('')
    handle = c_int(0)
    ret = hinst.DON_Open(byref(handle), 1)
    print('DON_Open return code:{0:x}'.format(ret & 4294967295))
    if ret:
        print('ERR')
    print('')
    nTime = c_long(0)
    ret = hinst.DON_GetDeadline(handle, byref(nTime))
    print('Dongle_GetDeadline return code:{0:x}'.format(ret & 4294967295))
    print(nTime.value)
    import time
    print('到期时间：')
    t = time.localtime(nTime.value)
    print(time.strftime('%Y-%m-%d %H:%M:%S', t))
    now = time.localtime()
    if now >= t:
        print('已经过期')
    if ret:
        print('ERR')
    print('')
    sharebuf = (c_ubyte * 32)()
    nDataLen = c_int(32)
    for i in range(0, 32):
        sharebuf[i] = i

    ret = hinst.DON_WriteShareMemory(handle, sharebuf, nDataLen)
    print('DON_WriteShareMemory return code:{0:x}'.format(ret & 4294967295))
    if ret:
        print('ERR')
    print('Write data is')
    for i in range(0, 32):
        print(hex(sharebuf[i]))

    print('')
    readbuf = (c_ubyte * 32)()
    ret = hinst.DON_ReadShareMemory(handle, readbuf)
    print('DON_ReadShareMemory return code:{0:x}'.format(ret & 4294967295))
    if ret:
        print('ERR')
    print('Read data is')
    for i in range(0, 32):
        print(hex(readbuf[i]))

    print('')
    nRemainCount = c_int(0)
    ret = hinst.DON_VerifyPIN(handle, 0, '12345678'.encode('ascii'), byref(nRemainCount))
    print('DON_VerifyPIN return code:{0:x}'.format(ret & 4294967295))
    if ret:
        print('ERR')
    print('')
    nRemainCount = c_int(0)
    ret = hinst.DON_VerifyPIN(handle, 1, 'FFFFFFFFFFFFFFFF'.encode('ascii'), byref(nRemainCount))
    print('DON_VerifyPIN return code:{0:x}'.format(ret & 4294967295))
    if ret:
        print('ERR')
    print('')
    ret = hinst.DON_Close(handle)
    print('DON_Close return code:{0:x}'.format(ret & 4294967295))
