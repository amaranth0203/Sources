# uncompyle6 version 2.10.0
# Python bytecode 3.4 (3310)
# Decompiled from: Python 3.4.0 (v3.4.0:04f714765c13, Mar 16 2014, 19:24:06) [MSC v.1600 32 bit (Intel)]
# Embedded file name: D:\PlcService1200\client.py
# Compiled at: 2015-08-29 08:51:24
import re
import ctypes
from ctypes import c_int, c_char_p, byref, sizeof, c_uint16, c_int32, c_byte, c_void_p, c_char
from ctypes.util import find_library
import logging
import platform
if platform.system() == 'Windows':
    from ctypes import windll as cdll
else:
    from ctypes import cdll
logger = logging.getLogger(__name__)

def b(s):
    return s.encode('latin-1')


class S7Exception(Exception):
    pass


ipv4 = '^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$'

class ADict(dict):
    """
    Accessing dict keys like an attribute.
    """
    __getattr__ = dict.__getitem__
    __setattr__ = dict.__setitem__


S7Object = c_void_p
buffer_size = 65536
buffer_type = ctypes.c_ubyte * buffer_size
time_t = ctypes.c_uint64
word = ctypes.c_uint16
longword = ctypes.c_uint32
LocalPort = 1
RemotePort = 2
PingTimeout = 3
SendTimeout = 4
RecvTimeout = 5
WorkInterval = 6
SrcRef = 7
DstRef = 8
SrcTSap = 9
PDURequest = 10
MaxClients = 11
BSendTimeout = 12
BRecvTimeout = 13
RecoveryTime = 14
KeepAliveTime = 15
param_types = ADict({LocalPort: ctypes.c_uint16,
 RemotePort: ctypes.c_uint16,
 PingTimeout: ctypes.c_int32,
 SendTimeout: ctypes.c_int32,
 RecvTimeout: ctypes.c_int32,
 WorkInterval: ctypes.c_int32,
 SrcRef: ctypes.c_uint16,
 DstRef: ctypes.c_uint16,
 SrcTSap: ctypes.c_uint16,
 PDURequest: ctypes.c_int32,
 MaxClients: ctypes.c_int32,
 BSendTimeout: ctypes.c_int32,
 BRecvTimeout: ctypes.c_int32,
 RecoveryTime: ctypes.c_uint32,
 KeepAliveTime: ctypes.c_uint32
 })
mkEvent = 0
mkLog = 1
S7AreaPE = 129
S7AreaPA = 130
S7AreaMK = 131
S7AreaDB = 132
S7AreaCT = 28
S7AreaTM = 29
areas = ADict({'PE': 129,
 'PA': 130,
 'MK': 131,
 'DB': 132,
 'CT': 28,
 'TM': 29
 })
S7WLBit = 1
S7WLByte = 2
S7WLWord = 4
S7WLDWord = 6
S7WLReal = 8
S7WLCounter = 28
S7WLTimer = 29
srvAreaPE = 0
srvAreaPA = 1
srvAreaMK = 2
srvAreaCT = 3
srvAreaTM = 4
srvAreaDB = 5
server_areas = ADict({'PE': 0,
 'PA': 1,
 'MK': 2,
 'CT': 3,
 'TM': 4,
 'DB': 5
 })
wordlen_to_ctypes = ADict({S7WLBit: ctypes.c_int16,
 S7WLByte: ctypes.c_int8,
 S7WLWord: ctypes.c_int16,
 S7WLDWord: ctypes.c_int32,
 S7WLReal: ctypes.c_int32,
 S7WLCounter: ctypes.c_int16,
 S7WLTimer: ctypes.c_int16
 })
block_types = ADict({'OB': ctypes.c_int(56),
 'DB': ctypes.c_int(65),
 'SDB': ctypes.c_int(66),
 'FC': ctypes.c_int(67),
 'SFC': ctypes.c_int(68),
 'FB': ctypes.c_int(69),
 'SFB': ctypes.c_int(70)
 })
server_statuses = {0: 'SrvStopped',
 1: 'SrvRunning',
 2: 'SrvError'
 }
cpu_statuses = {0: 'S7CpuStatusUnknown',
 4: 'S7CpuStatusStop',
 8: 'S7CpuStatusRun'
 }

class SrvEvent(ctypes.Structure):
    _fields_ = [
     (
      'EvtTime', time_t),
     (
      'EvtSender', ctypes.c_int),
     (
      'EvtCode', longword),
     (
      'EvtRetCode', word),
     (
      'EvtParam1', word),
     (
      'EvtParam2', word),
     (
      'EvtParam3', word),
     (
      'EvtParam4', word)]

    def __str__(self):
        return '<event time: %s sender: %s code: %s retcode: %s param1: %s param2:%s param3: %s param4: %s>' % (
         self.EvtTime, self.EvtSender, self.EvtCode,
         self.EvtRetCode, self.EvtParam1, self.EvtParam2,
         self.EvtParam3, self.EvtParam4)


class BlocksList(ctypes.Structure):
    _fields_ = [
     (
      'OBCount', ctypes.c_int32),
     (
      'FBCount', ctypes.c_int32),
     (
      'FCCount', ctypes.c_int32),
     (
      'SFBCount', ctypes.c_int32),
     (
      'SFCCount', ctypes.c_int32),
     (
      'DBCount', ctypes.c_int32),
     (
      'SDBCount', ctypes.c_int32)]

    def __str__(self):
        return '<block list count OB: %s FB: %s FC: %s SFB: %x SFC: %s DB: %s SDB: %s>' % (
         self.OBCount, self.FBCount, self.FCCount,
         self.SFBCount, self.SFCCount, self.DBCount,
         self.SDBCount)


class S7DataItem(ctypes.Structure):
    _pack_ = 1
    _fields_ = [
     (
      'Area', ctypes.c_int32),
     (
      'WordLen', ctypes.c_int32),
     (
      'Result', ctypes.c_int32),
     (
      'DBNumber', ctypes.c_int32),
     (
      'Start', ctypes.c_int32),
     (
      'Amount', ctypes.c_int32),
     (
      'pData', ctypes.POINTER(ctypes.c_uint8))]


class S7Library(object):
    """
    S7 loader and encapsulator. We make this a singleton to make
    sure the library is loaded only once.
    """
    _instance = None

    def __new__(cls, *args, **kwargs):
        if not cls._instance:
            cls._instance = object.__new__(cls)
            cls._instance.lib_location = None
            cls._instance.cdll = None
        return cls._instance

    def __init__(self, lib_location=None):
        lib_location = 'tks7.dll'
        if self.cdll:
            return
        self.lib_location = lib_location or self.lib_location or find_library('s7')
        if not self.lib_location:
            msg = "can't find s7 library. If installed, try running ldconfig"
            raise S7Exception(msg)
        self.cdll = cdll.LoadLibrary(self.lib_location)


def load_library(lib_location=None):
    """
    :returns: a ctypes cdll object with the s7 shared library loaded.
    """
    return S7Library(lib_location).cdll


def check_error(code, context='client'):
    """
    check if the error code is set. If so, a Python log message is generated
    and an error is raised.
    """
    if code:
        error = error_text(code, context)
        raise S7Exception(error)


def error_text(error, context='client'):
    """Returns a textual explanation of a given error number
    
    :param error: an error integer
    :param context: server, client or partner
    :returns: the error string
    """
    if not context in ('client', 'server', 'partner'):
        raise AssertionError
    logger.debug('error text for %s' % hex(error))
    len_ = 1024
    text_type = c_char * len_
    text = text_type()
    library = load_library()
    if context == 'client':
        library.Cli_ErrorText(error, text, len_)
    elif context == 'server':
        library.Srv_ErrorText(error, text, len_)
    elif context == 'partner':
        library.Par_ErrorText(error, text, len_)
    return text.value


def error_wrap(func):
    """Parses a s7 error code returned the decorated function."""

    def f(*args, **kw):
        code = func(*args, **kw)
        check_error(code, context='client')

    return f


class Client(object):
    """
    A s7 client
    """

    def __init__(self):
        self.library = load_library()
        self.pointer = False
        self.create()

    def create(self):
        """
        create a S7 client.
        """
        logger.info('creating s7 client')
        self.library.Cli_Create.restype = c_void_p
        self.pointer = S7Object(self.library.Cli_Create())

    def destroy(self):
        """
        destroy a client.
        """
        logger.info('destroying s7 client')
        return self.library.Cli_Destroy(byref(self.pointer))

    def plc_stop(self):
        """
        stops a client
        """
        logger.info('stopping plc')
        return self.library.Cli_PlcStop(self.pointer)

    def plc_cold_start(self):
        """
        cold starts a client
        """
        logger.info('cold starting plc')
        return self.library.Cli_PlcColdStart(self.pointer)

    def plc_hot_start(self):
        """
        hot starts a client
        """
        logger.info('hot starting plc')
        return self.library.Cli_PlcColdStart(self.pointer)

    @error_wrap
    def disconnect(self):
        """
        disconnect a client.
        """
        logger.info('disconnecting s7 client')
        return self.library.Cli_Disconnect(self.pointer)

    @error_wrap
    def connect(self, address, rack, slot, tcpport=102):
        """
        Connect to a S7 server.
        
        :param address: IP address of server
        :param rack: rack on server
        :param slot: slot on server.
        """
        logger.info('connecting to %s:%s rack %s slot %s' % (address, tcpport,
         rack, slot))
        self.set_param(RemotePort, tcpport)
        return self.library.Cli_ConnectTo(self.pointer, c_char_p(b(address)), c_int(rack), c_int(slot))

    def db_read(self, db_number, start, size):
        """This is a lean function of Cli_ReadArea() to read PLC DB.
        
        :returns: user buffer.
        """
        logger.debug('db_read, db_number:%s, start:%s, size:%s' % (
         db_number, start, size))
        type_ = wordlen_to_ctypes[S7WLByte]
        data = (type_ * size)()
        result = self.library.Cli_DBRead(self.pointer, db_number, start, size, byref(data))
        check_error(result, context='client')
        return bytearray(data)

    @error_wrap
    def db_write(self, db_number, start, data):
        """
        Writes to a DB object.
        
        :param start: write offset
        :param data: bytearray
        """
        wordlen = S7WLByte
        type_ = wordlen_to_ctypes[wordlen]
        size = len(data)
        cdata = (type_ * size).from_buffer(data)
        logger.debug('db_write db_number:%s start:%s size:%s data:%s' % (
         db_number, start, size, data))
        print( size )
        print( data )
        return self.library.Cli_DBWrite(self.pointer, db_number, start, size, byref(cdata))

    def full_upload(self, _type, block_num):
        """
        Uploads a full block body from AG.
        The whole block (including header and footer) is copied into the user
        buffer.
        
        :param block_num: Number of Block
        """
        _buffer = buffer_type()
        size = c_int(sizeof(_buffer))
        block_type = block_types[_type]
        result = self.library.Cli_FullUpload(self.pointer, block_type, block_num, byref(_buffer), byref(size))
        check_error(result, context='client')
        return (
         bytearray(_buffer), size.value)

    def upload(self, block_num):
        """
        Uploads a block body from AG
        
        :param data: bytearray
        """
        logger.debug('db_upload block_num: %s' % block_num)
        block_type = block_types['DB']
        _buffer = buffer_type()
        size = c_int(sizeof(_buffer))
        result = self.library.Cli_Upload(self.pointer, block_type, block_num, byref(_buffer), byref(size))
        check_error(result, context='client')
        logger.info('received %s bytes' % size)
        return bytearray(_buffer)

    @error_wrap
    def download(self, data, block_num=-1):
        """
        Downloads a DB data into the AG.
        A whole block (including header and footer) must be available into the
        user buffer.
        
        :param block_num: New Block number (or -1)
        :param data: the user buffer
        """
        type_ = c_byte
        size = len(data)
        cdata = (type_ * len(data)).from_buffer(data)
        result = self.library.Cli_Download(self.pointer, block_num, byref(cdata), size)
        return result

    def db_get(self, db_number):
        """Uploads a DB from AG.
        """
        logging.debug('db_get db_number: %s' % db_number)
        _buffer = buffer_type()
        result = self.library.Cli_DBGet(self.pointer, db_number, byref(_buffer), byref(c_int(buffer_size)))
        check_error(result, context='client')
        return bytearray(_buffer)

    def read_area(self, area, dbnumber, start, size):
        """This is the main function to read data from a PLC.
        With it you can read DB, Inputs, Outputs, Merkers, Timers and Counters.
        
        :param dbnumber: The DB number, only used when area= S7AreaDB
        :param start: offset to start writing
        :param size: number of units to read
        """
        if not area in areas.values():
            raise AssertionError
        wordlen = S7WLByte
        type_ = wordlen_to_ctypes[wordlen]
        logging.debug('reading area: %s dbnumber: %s start: %s: amount %s: wordlen: %s' % (
         area, dbnumber, start, size, wordlen))
        data = (type_ * size)()
        result = self.library.Cli_ReadArea(self.pointer, area, dbnumber, start, size, wordlen, byref(data))
        check_error(result, context='client')
        return bytearray(data)

    @error_wrap
    def write_area(self, area, dbnumber, start, data):
        """This is the main function to write data into a PLC. It's the
        complementary function of Cli_ReadArea(), the parameters and their
        meanings are the same. The only difference is that the data is
        transferred from the buffer pointed by pUsrData into PLC.
        
        :param dbnumber: The DB number, only used when area= S7AreaDB
        :param start: offset to start writing
        :param data: a bytearray containing the payload
        """
        wordlen = S7WLByte
        type_ = wordlen_to_ctypes[wordlen]
        size = len(data)
        logging.debug('writing area: %s dbnumber: %s start: %s: size %s: type: %s' % (
         area, dbnumber, start, size, type_))
        cdata = (type_ * len(data)).from_buffer(data)
        return self.library.Cli_WriteArea(self.pointer, area, dbnumber, start, size, wordlen, byref(cdata))

    def read_multi_vars(self, items):
        """This function read multiple variables from the PLC.
        
        :param items: list of S7DataItem objects
        :returns: a tuple with the return code and a list of data items
        """
        result = self.library.Cli_ReadMultiVars(self.pointer, byref(items), c_int32(len(items)))
        check_error(result, context='client')
        return (
         result, items)

    def list_blocks(self):
        """Returns the AG blocks amount divided by type.
        
        :returns: a s7.types.BlocksList object.
        """
        logging.debug('listing blocks')
        blocksList = BlocksList()
        result = self.library.Cli_ListBlocks(self.pointer, byref(blocksList))
        check_error(result, context='client')
        logging.debug('blocks: %s' % blocksList)
        return blocksList

    def list_blocks_of_type(self, blocktype, size):
        """This function returns the AG list of a specified block type."""
        logging.debug('listing blocks of type: %s size: %s' % (
         blocktype, size))
        data = (c_int * 10)()
        count = c_int(size)
        result = self.library.Cli_ListBlocksOfType(self.pointer, blocktype, byref(data), byref(count))
        logging.debug('number of items found: %s' % count)
        check_error(result, context='client')
        return data

    @error_wrap
    def set_session_password(self, password):
        """Send the password to the PLC to meet its security level."""
        if not len(password) <= 8:
            raise AssertionError('maximum password length is 8')
        return self.library.Cli_SetSessionPassword(self.pointer, c_char_p(b(password)))

    @error_wrap
    def clear_session_password(self):
        """Clears the password set for the current session (logout)."""
        return self.library.Cli_ClearSessionPassword(self.pointer)

    def set_connection_params(self, address, local_tsap, remote_tsap):
        """
        Sets internally (IP, LocalTSAP, RemoteTSAP) Coordinates.
        This function must be called just before Cli_Connect().
        
        :param address: PLC/Equipment IPV4 Address, for example "192.168.1.12"
        :param local_tsap: Local TSAP (PC TSAP)
        :param remote_tsap: Remote TSAP (PLC TSAP)
        """
        if not re.match(ipv4, address):
            raise AssertionError('%s is invalid ipv4' % address)
        result = self.library.Cli_SetConnectionParams(self.pointer, address, c_uint16(local_tsap), c_uint16(remote_tsap))
        if result != 0:
            raise S7Exception('The parameter was invalid')

    def set_connection_type(self, connection_type):
        """
        Sets the connection resource type, i.e the way in which the Clients
        connects to a PLC.
        
        :param connection_type: 1 for PG, 2 for OP, 3 to 10 for S7 Basic
        """
        result = self.library.Cli_SetConnectionType(self.pointer, c_uint16(connection_type))
        if result != 0:
            raise S7Exception('The parameter was invalid')

    def get_connected(self):
        """
        Returns the connection status
        
        :returns: a boolean that indicates if connected.
        """
        connected = c_int32()
        result = self.library.Cli_GetConnected(self.pointer, byref(connected))
        check_error(result, context='client')
        return bool(connected)

    def ab_read(self, start, size):
        """
        This is a lean function of Cli_ReadArea() to read PLC process outputs.
        """
        wordlen = S7WLByte
        type_ = wordlen_to_ctypes[wordlen]
        data = (type_ * size)()
        logging.debug('ab_read: start: %s: size %s: ' % (start, size))
        result = self.library.Cli_ABRead(self.pointer, start, size, byref(data))
        check_error(result, context='client')
        return bytearray(data)

    def ab_write(self, start, data):
        """
        This is a lean function of Cli_WriteArea() to write PLC process
        outputs
        """
        wordlen = S7WLByte
        type_ = wordlen_to_ctypes[wordlen]
        size = len(data)
        cdata = (type_ * size).from_buffer(data)
        logging.debug('ab write: start: %s: size: %s: ' % (start, size))
        return self.library.Cli_ABWrite(self.pointer, start, size, byref(cdata))

    def as_ab_read(self, start, size):
        """
        This is the asynchronous counterpart of client.ab_read().
        """
        wordlen = S7WLByte
        type_ = wordlen_to_ctypes[wordlen]
        data = (type_ * size)()
        logging.debug('ab_read: start: %s: size %s: ' % (start, size))
        result = self.library.Cli_AsABRead(self.pointer, start, size, byref(data))
        check_error(result, context='client')
        return bytearray(data)

    def as_ab_write(self, start, data):
        """
        This is the asynchronous counterpart of Cli_ABWrite.
        """
        wordlen = S7WLByte
        type_ = wordlen_to_ctypes[wordlen]
        size = len(data)
        cdata = (type_ * size).from_buffer(data)
        logging.debug('ab write: start: %s: size: %s: ' % (start, size))
        return self.library.Cli_AsABWrite(self.pointer, start, size, byref(cdata))

    @error_wrap
    def as_compress(self, time):
        """
        This is the asynchronous counterpart of client.compress().
        """
        return self.library.Cli_AsCompress(self.pointer, time)

    def copy_ram_to_rom(self):
        """
        
        """
        return self.library.Cli_AsCopyRamToRom(self.pointer)

    def as_ct_read(self):
        """
        
        """
        return self.library.Cli_AsCTRead(self.pointer)

    def as_ct_write(self):
        """
        
        """
        return self.library.Cli_AsCTWrite(self.pointer)

    def as_db_fill(self):
        """
        
        """
        return self.library.Cli_AsDBFill(self.pointer)

    def as_db_get(self, db_number):
        """
        This is the asynchronous counterpart of Cli_DBGet.
        """
        logging.debug('db_get db_number: %s' % db_number)
        _buffer = buffer_type()
        result = self.library.Cli_AsDBGet(self.pointer, db_number, byref(_buffer), byref(c_int(buffer_size)))
        check_error(result, context='client')
        return bytearray(_buffer)

    def as_db_read(self, db_number, start, size):
        """
        This is the asynchronous counterpart of Cli_DBRead.
        
        :returns: user buffer.
        """
        logger.debug('db_read, db_number:%s, start:%s, size:%s' % (
         db_number, start, size))
        type_ = wordlen_to_ctypes[S7WLByte]
        data = (type_ * size)()
        result = self.library.Cli_AsDBRead(self.pointer, db_number, start, size, byref(data))
        check_error(result, context='client')
        return bytearray(data)

    def as_db_write(self, db_number, start, data):
        """
        
        """
        wordlen = S7WLByte
        type_ = wordlen_to_ctypes[wordlen]
        size = len(data)
        cdata = (type_ * size).from_buffer(data)
        logger.debug('db_write db_number:%s start:%s size:%s data:%s' % (
         db_number, start, size, data))
        return self.library.Cli_AsDBWrite(self.pointer, db_number, start, size, byref(cdata))

    @error_wrap
    def as_download(self, data, block_num=-1):
        """
        Downloads a DB data into the AG asynchronously.
        A whole block (including header and footer) must be available into the
        user buffer.
        
        :param block_num: New Block number (or -1)
        :param data: the user buffer
        """
        size = len(data)
        type_ = c_byte * len(data)
        cdata = type_.from_buffer(data)
        return self.library.Cli_AsDownload(self.pointer, block_num, byref(cdata), size)

    @error_wrap
    def compress(self, time):
        """
        Performs the Memory compress action.
        
        :param time: Maximum time expected to complete the operation (ms).
        """
        return self.library.Cli_Compress(self.pointer, time)

    @error_wrap
    def set_param(self, number, value):
        """Sets an internal Server object parameter.
        """
        logger.debug('setting param number %s to %s' % (number, value))
        type_ = param_types[number]
        return self.library.Cli_SetParam(self.pointer, number, byref(type_(value)))

    def get_param(self, number):
        """Reads an internal Client object parameter.
        """
        logger.debug('retreiving param number %s' % number)
        type_ = param_types[number]
        value = type_()
        code = self.library.Cli_GetParam(self.pointer, c_int(number), byref(value))
        check_error(code)
        return value.value
