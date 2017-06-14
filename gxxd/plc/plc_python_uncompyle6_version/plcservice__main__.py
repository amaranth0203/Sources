# -*- coding:gbk -*-
# uncompyle6 version 2.10.0
# Python bytecode 3.4 (3310)
# Decompiled from: Python 3.4.0 (v3.4.0:04f714765c13, Mar 16 2014, 19:24:06) [MSC v.1600 32 bit (Intel)]
# Embedded file name: PlcService.py
# Compiled at: 2016-07-12 12:58:20
__author__ = 'kissjava@vip.qq.com'
__version__ = '1.1.7'
import sys
from PyQt4 import QtCore, QtGui
from PyQt4 import QtWebKit
from PlcServiceUI import *
import configparser
import os
import time
import threading
from multiprocessing import Process
import socket
import socketserver
import logging
import S71200
import DonLib
from client import S7Exception

class CheckDonThread(QtCore.QThread):

    def __init__(self, root, *args, **kwargs):
        QtCore.QThread.__init__(self)
        self.root = root
        self.args = args
        self.kwargs = kwargs

    def run(self):
        time.sleep(0.5)
        s = time.mktime(time.localtime())
        while True:
            try:
                try:
                    rs = DonLib.hasDongle('4187F7D1')
#                    if rs:
                    if True :
#                        if DonLib.checkUserPass(self.root.dongleUserPin):
                        if True :
                            rs = DonLib.getDeadline()
#                            if rs:
                            if False :
                                if time.localtime() >= rs:
                                    self.dongleFlag = False
                                    self.root.copyrightinfomessage = '试用版(已经过期，有效期至{}！)'.format(time.strftime('%Y-%m-%d %H:%M:%S', rs))
                                else:
                                    self.dongleFlag = True
                                    self.root.copyrightinfomessage = '全功能试用版，{}过期！'.format(time.strftime('%Y-%m-%d %H:%M:%S', rs))
                            else:
                                self.dongleFlag = True
                                self.root.copyrightinfomessage = '正式版，全部功能可用！'
                        else:
                            self.dongleFlag = False
                            self.root.copyrightinfomessage = '试用版，功能受限！'
                    else:
                        self.dongleFlag = False
                        self.root.copyrightinfomessage = '试用版，功能受限！'
                except Exception as e:
                    self.dongleFlag = False
                    self.root.copyrightinfomessage = '检测授权加密狗失败，无法正常使用！'
                    self.root.logger.error(str(e))

            finally:
                DonLib.closeDongle()

            if self.dongleFlag:
                s = time.mktime(time.localtime())
                self.root.logger.debug('检测到加密狗({})'.format(s))
                self.emit(QtCore.SIGNAL('check_don_result'))
                time.sleep(10)
            else:
                e = time.mktime(time.localtime())
#                if e - s >= 30:
                if False :
                    self.root.logger.debug('无狗超过30秒，自动关闭({})'.format(e))
                    try:
                        try:
                            for plc in self.root.plc_handles.values():
                                plc.disconnect()

                        except Exception as e:
                            self.root.logger.error(e)
                        else:
                            self.root.logger.debug('未检测到加密狗，试用结束自动退出')

                    finally:
                        self.root.logger.debug('exit')

                    self.root.auto_close = True
                    break
                self.emit(QtCore.SIGNAL('check_don_result'))
                time.sleep(3)

        self.emit(QtCore.SIGNAL('check_don_result'))


class PlcConnThread(QtCore.QThread):

    def __init__(self, function, *args, **kwargs):
        QtCore.QThread.__init__(self)
        self.function = function
        self.args = args
        self.kwargs = kwargs

    def __del__(self):
        self.wait()

    def run(self):
        self.function(*self.args, **self.kwargs)
        self.emit(QtCore.SIGNAL('plc_conn_result'))


class GetShowPlcThread(QtCore.QThread):

    def __init__(self, root, *args, **kwargs):
        QtCore.QThread.__init__(self)
        self.root = root
        self.args = args
        self.kwargs = kwargs

    def run(self):
        while True:
            if not hasattr(self.root, 'plc_handles'):
                self.root.show_plc_current_vals = ['' for i in range(18)]
                self.emit(QtCore.SIGNAL('get_show_plc_result'))
                self.root.logger.info('plc disconnect, thread end.')
                break
            plcip = self.root.show_plc_ip.currentText()
            if not plcip:
                self.root.show_plc_current_vals = ['' for i in range(18)]
                self.emit(QtCore.SIGNAL('get_show_plc_result'))
                self.root.logger.info('not plc selected, thread end.')
                break
            handle = self.root.plc_handles.get(plcip)
            if not handle:
                self.root.show_plc_current_vals = ['' for i in range(18)]
                self.emit(QtCore.SIGNAL('get_show_plc_result'))
                self.root.logger.info('plc lost connect, thread end.')
                break
            if self.args[0] != plcip:
                self.root.show_plc_current_vals = ['' for i in range(18)]
                self.emit(QtCore.SIGNAL('get_show_plc_result'))
                self.root.logger.info('plc changed, thread end.')
                break
            try:
                try:
                    self.root.mutex.lock()
                    plcvars = self.root.all_plc_var.get(plcip)
                    if plcvars:
                        plcvars = plcvars.split(',')
                    else:
                        plcvars = []
                    rs = []
                    for varaddr in plcvars:
                        try:
                            rs.append(handle.getMem(varaddr))
                        except S7Exception as e:
                            self.root.logger.error('查看监控变量线程获取单个变量失败：{}'.format(e))
                            rs = []
                            break

                    count = len(rs)
                    if count < 18:
                        rs = rs + ['' for i in range(18 - count)]
                    self.root.show_plc_current_vals = rs
                except Exception as e:
                    self.root.logger.error('查看监控变量线程：{}'.format(e))
                    break

            finally:
                self.root.mutex.unlock()

            self.root.logger.info(rs)
            self.emit(QtCore.SIGNAL('get_show_plc_result'))


class ThreadedTCPRequestHandler(socketserver.BaseRequestHandler):

    def handle(self):
        while True:
            recvstr = ''
            try:
                self.data = self.request.recv(1024).strip()
            except Exception as e:
                self.server.root.logger.error('客户端断线:{}'.format(e))
                break

            data = self.data.decode('utf-8')
            if data.startswith('get'):
                # format - get:ip,DB20.DBB30.0,DB20.DBB32.0 ...
                # return - value:value:value ...
                try:
                    getvars = data.split(',')
                    if not getvars:
                        self.request.sendall(b'err:error command')
                        return
                    length = len(getvars)
                    ip = getvars[0].split(':')[1]
                    plchsocket = self.server.root.plc_handles.get(ip)
                    if not plchsocket:
                        self.request.sendall(b'err:plc offline')
                        return
                    mutex = self.server.root.mutex
                    try:
                        try:
                            mutex.lock()
                            for i in range(1, length):
                                v = plchsocket.getMem(getvars[i])
                                recvstr += str(v) + ':'

                        except Exception as e:
                            self.request.sendall(b'err:getMem error')
                            return

                    finally:
                        mutex.unlock()

                    rs = recvstr[0:-1]
                    self.server.root.logger.debug('socket return:{}'.format(rs))
                    self.request.sendall(rs.encode())
                except Exception as e:
                    self.server.root.logger.error('远程请求失败:{}'.format(e))

            elif data.startswith('set'):
                # format - set:ip,db20.dbx0.0:1,DB20.DBB30:1,DB20.DBB32:2 ...
                # return - "ok" in binary
                try:
                    getvars = data.split(',')
                    if not getvars:
                        self.request.sendall(b'err:error command')
                        return
                    length = len(getvars)
                    ip = getvars[0].split(':')[1]
                    plchsocket = self.server.root.plc_handles.get(ip)
                    if not plchsocket:
                        recvstr = b'err:plc offline'
                        self.request.sendall(recvstr)
                        return
                    mutex = self.server.root.mutex
                    try:
                        try:
                            mutex.lock()
                            for i in range(1, length):
                                try:
                                    k, v = getvars[i].split(':')
                                except Exception as e:
                                    self.request.sendall(b'err:error command')
                                    self.server.root.logger.warning('socket 设置变量参数错误:{}'.format(e))
                                    return

                                with open( 'log.log' , 'w' ) as f :
                                    f.write( 'k v {} {}'.format( k , v ) ) ;
                                plchsocket.writeMem(k, int(v))

                        except Exception as e:
                            self.request.sendall(b'err:writeMem error')
                            self.server.root.logger.warning('socket 设置变量发生错误:{}'.format(e))
                            return

                    finally:
                        mutex.unlock()

                    self.request.sendall(b'ok')
                except Exception as e:
                    self.server.root.logger.error('远程写入失败:{}'.format(e))

            else:
                if data == 'exit':
                    self.request.sendall(b'exit')
                    return
                try:
                    self.request.sendall(b'err:error command')
                    self.server.root.logger.warning('socket 命令错误')
                except Exception as e:
                    self.server.root.logger.error('异常请求失败:{}'.format(e))

                return

    def finish(self):
        self.request.close()


class ThreadedTCPServer(socketserver.ThreadingMixIn, socketserver.TCPServer):

    def __init__(self, server_address, tcphandle, root):
        super(ThreadedTCPServer, self).__init__(server_address, tcphandle)
        self.root = root


class MyApp(Ui_MainWindow):

    def __init__(self, MainWindow):
        self.setupUi(MainWindow)
        self.MainWindow = MainWindow
        self.show_plc_addrs = []
        self.show_plc_vals = []
        self.write_plc_addrs = []
        self.write_plc_vals = []
        self.create_ui_container()
        self.show_plc_ip.activated[str].connect(self.on_show_plc_change)
        self.write_plc_ip.activated[str].connect(self.on_write_plc_change)
        self.update_show_plc_addr.clicked.connect(self.on_update_show_plc_addr)
        self.submit_write_vals.clicked.connect(self.on_submit_write_vals)
        self.action_conn_PLC.setStatusTip('链接到PLC,只会链接到没有链接的PLC')
        self.action_conn_PLC.triggered.connect(self.on_conn_plc)
        self.action_close_PLC.setStatusTip('断开和所有PLC的链接')
        self.action_close_PLC.triggered.connect(self.on_close_plc)
        self.action_Exit.setStatusTip('退出应用')
        self.action_Exit.triggered.connect(self.on_exit)
        self.action_Help.setStatusTip('打开帮助')
        self.action_Help.triggered.connect(self.on_help)
        self.action_About.setStatusTip('关于')
        self.action_About.triggered.connect(self.on_about)
        self.icon = QtGui.QIcon()
        self.icon.addPixmap(QtGui.QPixmap('image/singleM.ico'), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.mutex = QtCore.QMutex()
        self.checkConfig()
        self.checkDonThread = CheckDonThread(self)
        self.MainWindow.connect(self.checkDonThread, QtCore.SIGNAL('check_don_result'), self.check_don_result)
        self.checkDonThread.start()
        self.run_socket_service()
        if self.autoConn:
            self.on_conn_plc()

    def create_ui_container(self):
        self.show_plc_addrs.append(self.show_plc_addr_1)
        self.show_plc_addrs.append(self.show_plc_addr_2)
        self.show_plc_addrs.append(self.show_plc_addr_3)
        self.show_plc_addrs.append(self.show_plc_addr_4)
        self.show_plc_addrs.append(self.show_plc_addr_5)
        self.show_plc_addrs.append(self.show_plc_addr_6)
        self.show_plc_addrs.append(self.show_plc_addr_7)
        self.show_plc_addrs.append(self.show_plc_addr_8)
        self.show_plc_addrs.append(self.show_plc_addr_9)
        self.show_plc_addrs.append(self.show_plc_addr_10)
        self.show_plc_addrs.append(self.show_plc_addr_11)
        self.show_plc_addrs.append(self.show_plc_addr_12)
        self.show_plc_addrs.append(self.show_plc_addr_13)
        self.show_plc_addrs.append(self.show_plc_addr_14)
        self.show_plc_addrs.append(self.show_plc_addr_15)
        self.show_plc_addrs.append(self.show_plc_addr_16)
        self.show_plc_addrs.append(self.show_plc_addr_17)
        self.show_plc_addrs.append(self.show_plc_addr_18)
        self.show_plc_vals.append(self.show_plc_val_1)
        self.show_plc_vals.append(self.show_plc_val_2)
        self.show_plc_vals.append(self.show_plc_val_3)
        self.show_plc_vals.append(self.show_plc_val_4)
        self.show_plc_vals.append(self.show_plc_val_5)
        self.show_plc_vals.append(self.show_plc_val_6)
        self.show_plc_vals.append(self.show_plc_val_7)
        self.show_plc_vals.append(self.show_plc_val_8)
        self.show_plc_vals.append(self.show_plc_val_9)
        self.show_plc_vals.append(self.show_plc_val_10)
        self.show_plc_vals.append(self.show_plc_val_11)
        self.show_plc_vals.append(self.show_plc_val_12)
        self.show_plc_vals.append(self.show_plc_val_13)
        self.show_plc_vals.append(self.show_plc_val_14)
        self.show_plc_vals.append(self.show_plc_val_15)
        self.show_plc_vals.append(self.show_plc_val_16)
        self.show_plc_vals.append(self.show_plc_val_17)
        self.show_plc_vals.append(self.show_plc_val_18)
        self.write_plc_addrs.append(self.write_plc_addr_1)
        self.write_plc_addrs.append(self.write_plc_addr_2)
        self.write_plc_addrs.append(self.write_plc_addr_3)
        self.write_plc_addrs.append(self.write_plc_addr_4)
        self.write_plc_addrs.append(self.write_plc_addr_5)
        self.write_plc_addrs.append(self.write_plc_addr_6)
        self.write_plc_addrs.append(self.write_plc_addr_7)
        self.write_plc_addrs.append(self.write_plc_addr_8)
        self.write_plc_addrs.append(self.write_plc_addr_9)
        self.write_plc_addrs.append(self.write_plc_addr_10)
        self.write_plc_addrs.append(self.write_plc_addr_11)
        self.write_plc_addrs.append(self.write_plc_addr_12)
        self.write_plc_addrs.append(self.write_plc_addr_13)
        self.write_plc_addrs.append(self.write_plc_addr_14)
        self.write_plc_addrs.append(self.write_plc_addr_15)
        self.write_plc_addrs.append(self.write_plc_addr_16)
        self.write_plc_addrs.append(self.write_plc_addr_17)
        self.write_plc_addrs.append(self.write_plc_addr_18)
        self.write_plc_vals.append(self.write_plc_val_1)
        self.write_plc_vals.append(self.write_plc_val_2)
        self.write_plc_vals.append(self.write_plc_val_3)
        self.write_plc_vals.append(self.write_plc_val_4)
        self.write_plc_vals.append(self.write_plc_val_5)
        self.write_plc_vals.append(self.write_plc_val_6)
        self.write_plc_vals.append(self.write_plc_val_7)
        self.write_plc_vals.append(self.write_plc_val_8)
        self.write_plc_vals.append(self.write_plc_val_9)
        self.write_plc_vals.append(self.write_plc_val_10)
        self.write_plc_vals.append(self.write_plc_val_11)
        self.write_plc_vals.append(self.write_plc_val_12)
        self.write_plc_vals.append(self.write_plc_val_13)
        self.write_plc_vals.append(self.write_plc_val_14)
        self.write_plc_vals.append(self.write_plc_val_15)
        self.write_plc_vals.append(self.write_plc_val_16)
        self.write_plc_vals.append(self.write_plc_val_17)
        self.write_plc_vals.append(self.write_plc_val_18)

    def check_don_result(self):
        if hasattr(self, 'auto_close'):
            if self.auto_close:
                self.on_exit()
        if hasattr(self, 'copyrightinfomessage'):
            self.statusbar.showMessage(self.copyrightinfomessage)

    def get_show_plc_result(self):
        if not self.show_plc_current_vals:
            return
        for valtext, val in zip(self.show_plc_vals, self.show_plc_current_vals):
            valtext.setText(str(val))

    def on_show_plc_change(self):
        """查看PLC变量的plc改变选择"""
        show_current_ip = self.show_plc_ip.currentText()
        if hasattr(self, 'last_show_plc_ip'):
            if self.last_show_plc_ip == show_current_ip:
                return
        if show_current_ip:
            show_current_vars = self.all_plc_var.get(show_current_ip)
            if not show_current_vars:
                show_current_vars = []
            else:
                if ',' in show_current_vars:
                    show_current_vars = show_current_vars.split(',')
                else:
                    show_current_vars = [
                     show_current_vars]
                for containeraddr, containerval in zip(self.show_plc_addrs, self.show_plc_vals):
                    containeraddr.setText('')
                    containerval.setText('')

                for containeraddr, val in zip(self.show_plc_addrs, show_current_vars):
                    containeraddr.setText(val)

                self.showPlcThread = GetShowPlcThread(self, show_current_ip)
                self.MainWindow.connect(self.showPlcThread, QtCore.SIGNAL('get_show_plc_result'), self.get_show_plc_result)
                self.showPlcThread.start()
            self.last_show_plc_ip = show_current_ip
        else:
            if hasattr(self, 'last_show_plc_ip'):
                del self.last_show_plc_ip
            for containeraddr, containerval in zip(self.show_plc_addrs, self.show_plc_vals):
                containeraddr.setText('')
                containerval.setText('')

    def on_write_plc_change(self):
        write_current_ip = self.write_plc_ip.currentText()
        if write_current_ip:
            write_current_vars = self.all_plc_var.get(write_current_ip)
            if not write_current_vars:
                write_current_vars = []
            else:
                if ',' in write_current_vars:
                    write_current_vars = write_current_vars.split(',')
                else:
                    write_current_vars = [
                     write_current_vars]
                for container in self.write_plc_addrs:
                    container.setText('')

                for container, val in zip(self.write_plc_addrs, write_current_vars):
                    container.setText(val)

        else:
            for container, valtext in zip(self.write_plc_addrs, self.write_plc_vals):
                container.setText('')
                valtext.setText('')

    def on_update_show_plc_addr(self):
        show_plc_current_ip = self.show_plc_ip.currentText()
        if not show_plc_current_ip:
            self.statusbar.showMessage('需要先选择PLC!', 3000)
            return
        self.all_plc_var[show_plc_current_ip] = ''
        temp_vars = ''
        for e in self.show_plc_addrs:
            mystr = e.text()
            if mystr:
                temp_vars += mystr + ','

        if temp_vars:
            self.all_plc_var[show_plc_current_ip] = temp_vars[0:-1]

    def on_submit_write_vals(self):
        if not self.write_plc_ip.currentText():
            msg = '请先选择PLC！'
            msgBox = QtGui.QMessageBox(QtGui.QMessageBox.Warning, '错误', msg, QtGui.QMessageBox.NoButton)
            msgBox.setWindowIcon(self.icon)
            msgBox.addButton('确定', QtGui.QMessageBox.AcceptRole)
            msgBox.exec_()
            return
        vals = {}
        for addr, val in zip(self.write_plc_addrs, self.write_plc_vals):
            if addr.text() and val.text():
                try:
                    vals[addr.text()] = abs(int(val.text()))
                except Exception as e:
                    msg = '只能写入整数值！'
                    msgBox = QtGui.QMessageBox(QtGui.QMessageBox.Warning, '错误', msg, QtGui.QMessageBox.NoButton)
                    msgBox.setWindowIcon(self.icon)
                    msgBox.addButton('确定', QtGui.QMessageBox.AcceptRole)
                    msgBox.exec_()
                    self.logger.warning('写入plc({})变量失败：{}'.format(self.write_plc_ip.currentText(), e))
                    return
                else:
                    None
        if not len(vals) > 0:
            msg = '没有符合规范的输入！'
            msgBox = QtGui.QMessageBox(QtGui.QMessageBox.Warning, '错误', msg, QtGui.QMessageBox.NoButton)
            msgBox.setWindowIcon(self.icon)
            msgBox.addButton('确定', QtGui.QMessageBox.AcceptRole)
            msgBox.exec_()
            return
        handle = self.plc_handles.get(self.write_plc_ip.currentText())
        if not handle:
            msg = '该PLC链接已断开！'
            msgBox = QtGui.QMessageBox(QtGui.QMessageBox.Warning, '错误', msg, QtGui.QMessageBox.NoButton)
            msgBox.setWindowIcon(self.icon)
            msgBox.addButton('确定', QtGui.QMessageBox.AcceptRole)
            msgBox.exec_()
            return
        self.mutex.lock()
        try:
            try:
                for k, v in vals.items():
                    try:
                        try:
                            with open( 'log.log' , 'w' ) as f :
                                f.write( 'k v {} {}'.format( k , v ) ) ;
                            handle.writeMem(k, v)
                        except Exception as e:
                            raise e

                    finally:
                        pass

            except Exception as e:
                self.statusbar.showMessage('写入变量失败！{}'.format(e))
                self.logger.error('写入变量失败！{}'.format(e))
                msg = '写入变量失败！{}'.format(e)
                msgBox = QtGui.QMessageBox(QtGui.QMessageBox.Warning, '写入失败!', msg, QtGui.QMessageBox.NoButton)
                msgBox.setWindowIcon(self.icon)
                msgBox.addButton('确定', QtGui.QMessageBox.AcceptRole)
                msgBox.exec_()
            else:
                self.statusbar.showMessage('写入变量成功！({})'.format(time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(time.time()))))

        finally:
            self.mutex.unlock()

    def plc_conn_result(self):
        """Plc链接结果显示更新"""
        msg = ''
        for k in self.plc_handles:
            self.show_plc_ip.addItem(k)
            self.write_plc_ip.addItem(k)

        failed = ''
        for k in self.conn_failed:
            failed = failed + ' ' + k

        if failed:
            msg = failed + ' 链接失败！'
        else:
            msg = ' 链接成功！'
        self.statusbar.showMessage(msg)

    def process_conn_plc(self):# wassup 调用 lib, 重连机制在这里添加试试看
        for ip, info in self.all_plc_info.items():
            try:
                info = info.split(',')
                plc = S71200.S71200(ip, int(info[0]), int(info[1]))
                self.plc_handles[ip] = plc
            except Exception as e:
                self.conn_failed.add(ip)
                self.logger.error(e)

    def on_conn_plc(self):
        self.statusbar.showMessage(' 正在链接到PLC......')
        self.checkConfig()
        self.plcConnThread = PlcConnThread(self.process_conn_plc)
        self.MainWindow.connect(self.plcConnThread, QtCore.SIGNAL('plc_conn_result'), self.plc_conn_result)
        self.plcConnThread.start()

    def on_close_plc(self):
        self.statusbar.showMessage('断开PLC')
        self.show_plc_ip.setCurrentIndex(0)
        if hasattr(self, 'plc_handles'):
            for ip in self.plc_handles:
                try:
                    self.plc_handles.get(ip).disconnect()
                except Exception as e:
                    pass

            self.plc_handles = {}
            self.statusbar.showMessage('PLC已断开')
        else:
            self.statusbar.showMessage('没有已链接的PLC')
        self.show_plc_ip.clear()
        self.write_plc_ip.clear()
        for container, valtext in zip(self.show_plc_addrs, self.show_plc_vals):
            container.setText('')
            valtext.setText('')

        for container, valtext in zip(self.write_plc_addrs, self.write_plc_vals):
            container.setText('')
            valtext.setText('')

    def on_exit(self):
        sys.exit()

    def on_help(self):
        self.statusbar.showMessage('打开帮助', 3000)
        dialog = QtGui.QDialog()
        dialog.setWindowIcon(self.icon)
        dialog.setWindowTitle('帮助')
        dialog.resize(800, 600)
        webView = QtWebKit.QWebView(dialog)
        webView.setGeometry(QtCore.QRect(0, 0, 800, 600))
        webView.setUrl(QtCore.QUrl('image/help.html'))
        dialog.exec_()

    def on_about(self):
        self.statusbar.showMessage('关于', 3000)
        msg = '多路PLC通讯服务端 v{}'.format(__version__)
        if hasattr(self, 'copyrightinfomessage'):
            msg = msg + '\n【' + self.copyrightinfomessage + '】'
        msg += '\n{}'.format(__author__)
        msgBox = QtGui.QMessageBox(QtGui.QMessageBox.Information, '关于', msg, QtGui.QMessageBox.NoButton)
        msgBox.setWindowIcon(self.icon)
        msgBox.addButton('确定', QtGui.QMessageBox.AcceptRole)
        msgBox.exec_()

    def checkConfig(self):
        self.plc_handles = {}
        self.dongleFlag = False
        self.conn_failed = set([])
        self.show_plc_ip.clear()
        self.write_plc_ip.clear()
        self.show_plc_ip.addItem('')
        self.write_plc_ip.addItem('')
        config = configparser.ConfigParser()
        try:
            config.read(os.getcwd() + '\\config.ini')
            debug_info = config.get('base', 'debug')
            if not hasattr(self, 'logger'):
                self.logger = logging.getLogger('my_logger')
                if int(debug_info) == 0:
                    self.logger.setLevel(logging.ERROR)
                else:
                    if int(debug_info) == 1:
                        self.logger.setLevel(logging.DEBUG)
                    formatter = logging.Formatter('%(asctime)s - %(levelname)s - line:%(lineno)d - %(message)s')
                    fh = logging.FileHandler('app.log', 'w')
                    fh.setFormatter(formatter)
                    self.logger.addHandler(fh)
            self.all_plc_info = {}
            plc_params = config.items('plc')
            for k, v in plc_params:
                self.all_plc_info[k] = v

            self.logger.debug('所有被监控plc信息（all_plc_info）:{}'.format(self.all_plc_info))
            self.all_plc_var = {}
            var_params = config.items('var')
            for k, v in var_params:
                self.all_plc_var[k] = v

            self.logger.debug('被监控变量字典（all_plc_var）:{}'.format(self.all_plc_var))
            self.socketPort = int(config.get('socket', 'port'))
            self.dongleUserPin = config.get('base', 'userPin')
            self.autoConn = int(config.get('base', 'autoConn'))
        except Exception as e:
            msg = '配置文件无法解析！请检查配置文件是否正确！'
            msgBox = QtGui.QMessageBox(QtGui.QMessageBox.Warning, '错误', msg, QtGui.QMessageBox.NoButton)
            msgBox.setWindowIcon(self.icon)
            msgBox.addButton('确定', QtGui.QMessageBox.AcceptRole)
            msgBox.exec_()
            self.root.logger.error(msg)
            self.root.logger.error(e)
            os._exit(-1)

    def run_socket_service(self):
        HOST, PORT = socket.gethostbyname(socket.gethostname()), self.socketPort
        try:
            server = ThreadedTCPServer((HOST, PORT), ThreadedTCPRequestHandler, self)
            server_thread = threading.Thread(target=server.serve_forever)
            server_thread.daemon = True
            server_thread.start()
        except Exception as e:
            msg = 'Socket服务启动失败,端口可能被占用,无法对外提供服务！{}'.format(e)
            self.logger.error(msg)
            msgBox = QtGui.QMessageBox(QtGui.QMessageBox.Warning, '错误', msg, QtGui.QMessageBox.NoButton)
            msgBox.setWindowIcon(self.icon)
            msgBox.addButton('确定', QtGui.QMessageBox.AcceptRole)
            msgBox.exec_()
            self.statusbar.showMessage(msg)


if __name__ == '__main__':
    app = QtGui.QApplication(sys.argv)
    MainWindow = QtGui.QMainWindow()
    ui = MyApp(MainWindow)
    MainWindow.setWindowTitle('多路PLC通讯服务端  v{}'.format(__version__))
    MainWindow.setWindowIcon(QtGui.QIcon('image/singleM.ico'))
    MainWindow.show()
    sys.exit(app.exec_())
