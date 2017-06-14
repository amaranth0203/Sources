# -*- coding:gbk -*-
# uncompyle6 version 2.10.0
# Python bytecode 3.4 (3310)
# Decompiled from: Python 3.4.0 (v3.4.0:04f714765c13, Mar 16 2014, 19:24:06) [MSC v.1600 32 bit (Intel)]
# Embedded file name: D:\PlcService1200\S71200.py
# Compiled at: 2015-07-31 13:17:20
from time import sleep
import client
from util import *

class output(object):
    bool = 1
    byte = 2
    real = 3
    word = 4
    dword = 5


class S71200:

    def __init__(self, ip, localtsap=0, remotetsap=1, debug=False):
        self.debug = debug
        self.plc = client.Client()
        self.plc.connect(ip, localtsap, remotetsap)
        self.ip = ip

    def getMem(self, mem, returnByte=False):
        if mem[0:2].lower() == 'db': # DB20.DBB32.0
            arr = mem.split('.') # [ db20 , dbb32 , 0 ]
            db_number = int(arr[0][2:]) # 20
            start = int(arr[1][3:]) # 32
            if arr[1][2].lower() == 'x':
                size = 1
                out = output().bool # 1
            if arr[1][2].lower() == 'b': # this one
                size = 1
                out = output().byte # 2
            if arr[1][2].lower() == 'w':
                size = 2
                out = output().word # 4
            if arr[1][2].lower() == 'd':
                size = 4
                out = output().dword # 5
            mbyte = self.plc.db_read(db_number, start, size) # ( 20 , 32 , 1 )
            if returnByte:
                return mbyte
            if output().bool == out: # 1
                bit = int(arr[2])
                return get_bool(mbyte, 0, bit)
            if output().byte == out: # 2
                return get_byte(mbyte, 0)
            if output().real == out: # 3 --- cancel
                return get_real(mbyte, 0)
            if output().dword == out: # 5
                return get_dword(mbyte, 0)
            if output().word == out: # 4
                return get_int(mbyte, 0)
        area = 131
        length = 1
        type = 0
        out = None
        bit = 0
        start = 0
        if mem[0].lower() == 'm':
            area = 131
        if mem[0].lower() == 'q':
            area = 130
        if mem[0].lower() == 'i': # --- cancel
            area = 129
        if mem[1].lower() == 'x':
            length = 1
            out = output().bool
            start = int(mem.split('.')[0][2:])
        if mem[1].lower() == 'd':
            out = output().dword
            length = 4
            start = int(mem.split('.')[0][2:])
        if 'freal' in mem.lower(): # --- cancel
            length = 4
            start = int(mem.lower().replace('freal', ''))
            out = output().real
        if output().bool == out:
            bit = int(mem.split('.')[1])
        if self.debug:
            print(mem[0].lower(), bit)
        mbyte = self.plc.read_area(area, 0, start, length)
        if returnByte:
            return mbyte
        if output().bool == out:
            return get_bool(mbyte, 0, bit)
        if output().byte == out: # --- cancel
            return get_int(mbyte, start)
        if output().real == out: # --- cancel
            return get_real(mbyte, 0)
        if output().dword == out:
            return get_dword(mbyte, 0)
        if output().word == out: # --- cancel
            return get_int(mbyte, start)

    def disconnect(self):
        self.plc.disconnect()

    def writeMem(self, mem, value):
        data = self.getMem(mem, True)
        if mem[0:2].lower() == 'db': # DB20.DBW30 写入 0 / 1
            arr = mem.split('.')
            db_number = int(arr[0][2:])
            start = int(arr[1][3:])
            if arr[1][2].lower() == 'x':
                size = 1
                out = output().bool
                bit = int(arr[2])
                set_bool(data, 0, bit, int(value))
            if arr[1][2].lower() == 'b':
                size = 1
                out = output().byte
                set_byte(data, 0, value)
            if arr[1][2].lower() == 'w':
                size = 2
                out = output().word
                set_int(data, 0, value)
            if arr[1][2].lower() == 'd':
                size = 4
                out = output().dword
                set_dword(data, 0, value)
            return self.plc.db_write(db_number, start, data)
        area = 131
        length = 1
        type = 0
        out = None
        bit = 0
        start = 0
        if mem[0].lower() == 'm':
            area = 131
        if mem[0].lower() == 'q':
            area = 130
        if mem[0].lower() == 'i': # --- cancel
            area = 129
        if mem[1].lower() == 'x':
            length = 1
            out = output().bool
            start = int(mem.split('.')[0][2:])
            bit = int(mem.split('.')[1])
            set_bool(data, 0, bit, int(value))
        if mem[1].lower() == 'd':
            out = output().dword
            length = 4
            start = int(mem.split('.')[0][2:])
            set_dword(data, 0, value)
        if 'freal' in mem.lower(): # --- cancel
            length = 4
            start = int(mem.lower().replace('freal', ''))
            out = output().real
            print(data)
            set_real(data, 0, value)
        return self.plc.write_area(area, 0, start, data)


if __name__ == '__main__':
    import threading
    import time
    plcips = [
     '192.168.15.250', '192.168.15.241', '192.168.15.242']
    plcvars = {'192.168.15.250': [
                        'md1', 'md10', 'md100'],
     '192.168.15.241': [
                        'md2', 'md20'],
     '192.168.15.242': [
                        'md3', 'md30', 'md300']
     }
    plchandles = {}
    locks = {}
    threads = []

    def process_write(ip, vals):
        handle = plchandles.get(ip)
        lock = locks.get(ip)
        try:
            for k, v in vals.items():
                lock.acquire()
                try:
                    print("{}'s {} write {}({}) at {}".format(threading.current_thread().name, k, v, handle.writeMem(k, v), time.strftime('%H:%M:%S')))
                finally:
                    lock.release()

        except Exception as e:
            print('写入变量失败！{}'.format(e))


    def process_read(ip, vars):
        handle = plchandles.get(ip)
        lock = locks.get(ip)
        for _ in range(3000000):
            for i in vars:
                lock.acquire()
                try:
                    print("{}'s {} is: {} at {}".format(threading.current_thread().name, i, handle.getMem(i), time.strftime('%H:%M:%S')))
                finally:
                    lock.release()


    for ip in plcips:
        plc = S71200(ip)
        plchandles[ip] = plc
        lock = threading.Lock()
        locks[ip] = lock
        var = plcvars[ip]
        t = threading.Thread(target=process_read, args=(ip, var), name='Thread {}'.format(ip))
        threads.append(t)

    ip = '192.168.15.241'
    vals = {'md3': 1115, 'md30': 2225}
    t = threading.Thread(target=process_write, args=(ip, vals), name='Thread Write @@@@@@@@@@@@@@@@@@@ {}'.format(ip))
    threads.append(t)
    for t in threads:
        t.setDaemon(True)
        t.start()

    ip = '192.168.15.242'
    vals = {'md2': 1915, 'md20': 2925, 'md300': 3003}
    t = threading.Thread(target=process_write, args=(ip, vals), name='Thread Write @@@@@@@@@@@@@@@@@@@ {}'.format(ip))
    t.start()
    time.sleep(3600)
