#
# Autogenerated by Thrift Compiler (0.10.0)
#
# DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
#
#  options string: py
#

from thrift.Thrift import TType, TMessageType, TFrozenDict, TException, TApplicationException
from thrift.protocol.TProtocol import TProtocolException
import sys
import logging
from .ttypes import *
from thrift.Thrift import TProcessor
from thrift.transport import TTransport


class Iface(object):
    def ping(self):
        pass

    def write_timeline(self, user_id, unique_id, req_id, type):
        """
        Parameters:
         - user_id
         - unique_id
         - req_id
         - type
        """
        pass


class Client(Iface):
    def __init__(self, iprot, oprot=None):
        self._iprot = self._oprot = iprot
        if oprot is not None:
            self._oprot = oprot
        self._seqid = 0

    def ping(self):
        self.send_ping()
        self.recv_ping()

    def send_ping(self):
        self._oprot.writeMessageBegin('ping', TMessageType.CALL, self._seqid)
        args = ping_args()
        args.write(self._oprot)
        self._oprot.writeMessageEnd()
        self._oprot.trans.flush()

    def recv_ping(self):
        iprot = self._iprot
        (fname, mtype, rseqid) = iprot.readMessageBegin()
        if mtype == TMessageType.EXCEPTION:
            x = TApplicationException()
            x.read(iprot)
            iprot.readMessageEnd()
            raise x
        result = ping_result()
        result.read(iprot)
        iprot.readMessageEnd()
        return

    def write_timeline(self, user_id, unique_id, req_id, type):
        """
        Parameters:
         - user_id
         - unique_id
         - req_id
         - type
        """
        self.send_write_timeline(user_id, unique_id, req_id, type)

    def send_write_timeline(self, user_id, unique_id, req_id, type):
        self._oprot.writeMessageBegin('write_timeline', TMessageType.ONEWAY, self._seqid)
        args = write_timeline_args()
        args.user_id = user_id
        args.unique_id = unique_id
        args.req_id = req_id
        args.type = type
        args.write(self._oprot)
        self._oprot.writeMessageEnd()
        self._oprot.trans.flush()


class Processor(Iface, TProcessor):
    def __init__(self, handler):
        self._handler = handler
        self._processMap = {}
        self._processMap["ping"] = Processor.process_ping
        self._processMap["write_timeline"] = Processor.process_write_timeline

    def process(self, iprot, oprot):
        (name, type, seqid) = iprot.readMessageBegin()
        if name not in self._processMap:
            iprot.skip(TType.STRUCT)
            iprot.readMessageEnd()
            x = TApplicationException(TApplicationException.UNKNOWN_METHOD, 'Unknown function %s' % (name))
            oprot.writeMessageBegin(name, TMessageType.EXCEPTION, seqid)
            x.write(oprot)
            oprot.writeMessageEnd()
            oprot.trans.flush()
            return
        else:
            self._processMap[name](self, seqid, iprot, oprot)
        return True

    def process_ping(self, seqid, iprot, oprot):
        args = ping_args()
        args.read(iprot)
        iprot.readMessageEnd()
        result = ping_result()
        try:
            self._handler.ping()
            msg_type = TMessageType.REPLY
        except (TTransport.TTransportException, KeyboardInterrupt, SystemExit):
            raise
        except Exception as ex:
            msg_type = TMessageType.EXCEPTION
            logging.exception(ex)
            result = TApplicationException(TApplicationException.INTERNAL_ERROR, 'Internal error')
        oprot.writeMessageBegin("ping", msg_type, seqid)
        result.write(oprot)
        oprot.writeMessageEnd()
        oprot.trans.flush()

    def process_write_timeline(self, seqid, iprot, oprot):
        args = write_timeline_args()
        args.read(iprot)
        iprot.readMessageEnd()
        try:
            self._handler.write_timeline(args.user_id, args.unique_id, args.req_id, args.type)
        except (TTransport.TTransportException, KeyboardInterrupt, SystemExit):
            raise
        except:
            pass

# HELPER FUNCTIONS AND STRUCTURES


class ping_args(object):

    thrift_spec = (
    )

    def read(self, iprot):
        if iprot._fast_decode is not None and isinstance(iprot.trans, TTransport.CReadableTransport) and self.thrift_spec is not None:
            iprot._fast_decode(self, iprot, (self.__class__, self.thrift_spec))
            return
        iprot.readStructBegin()
        while True:
            (fname, ftype, fid) = iprot.readFieldBegin()
            if ftype == TType.STOP:
                break
            else:
                iprot.skip(ftype)
            iprot.readFieldEnd()
        iprot.readStructEnd()

    def write(self, oprot):
        if oprot._fast_encode is not None and self.thrift_spec is not None:
            oprot.trans.write(oprot._fast_encode(self, (self.__class__, self.thrift_spec)))
            return
        oprot.writeStructBegin('ping_args')
        oprot.writeFieldStop()
        oprot.writeStructEnd()

    def validate(self):
        return

    def __repr__(self):
        L = ['%s=%r' % (key, value)
             for key, value in self.__dict__.items()]
        return '%s(%s)' % (self.__class__.__name__, ', '.join(L))

    def __eq__(self, other):
        return isinstance(other, self.__class__) and self.__dict__ == other.__dict__

    def __ne__(self, other):
        return not (self == other)


class ping_result(object):

    thrift_spec = (
    )

    def read(self, iprot):
        if iprot._fast_decode is not None and isinstance(iprot.trans, TTransport.CReadableTransport) and self.thrift_spec is not None:
            iprot._fast_decode(self, iprot, (self.__class__, self.thrift_spec))
            return
        iprot.readStructBegin()
        while True:
            (fname, ftype, fid) = iprot.readFieldBegin()
            if ftype == TType.STOP:
                break
            else:
                iprot.skip(ftype)
            iprot.readFieldEnd()
        iprot.readStructEnd()

    def write(self, oprot):
        if oprot._fast_encode is not None and self.thrift_spec is not None:
            oprot.trans.write(oprot._fast_encode(self, (self.__class__, self.thrift_spec)))
            return
        oprot.writeStructBegin('ping_result')
        oprot.writeFieldStop()
        oprot.writeStructEnd()

    def validate(self):
        return

    def __repr__(self):
        L = ['%s=%r' % (key, value)
             for key, value in self.__dict__.items()]
        return '%s(%s)' % (self.__class__.__name__, ', '.join(L))

    def __eq__(self, other):
        return isinstance(other, self.__class__) and self.__dict__ == other.__dict__

    def __ne__(self, other):
        return not (self == other)


class write_timeline_args(object):
    """
    Attributes:
     - user_id
     - unique_id
     - req_id
     - type
    """

    thrift_spec = (
        None,  # 0
        (1, TType.STRING, 'user_id', 'UTF8', None, ),  # 1
        (2, TType.STRING, 'unique_id', 'UTF8', None, ),  # 2
        (3, TType.STRING, 'req_id', 'UTF8', None, ),  # 3
        (4, TType.STRING, 'type', 'UTF8', None, ),  # 4
    )

    def __init__(self, user_id=None, unique_id=None, req_id=None, type=None,):
        self.user_id = user_id
        self.unique_id = unique_id
        self.req_id = req_id
        self.type = type

    def read(self, iprot):
        if iprot._fast_decode is not None and isinstance(iprot.trans, TTransport.CReadableTransport) and self.thrift_spec is not None:
            iprot._fast_decode(self, iprot, (self.__class__, self.thrift_spec))
            return
        iprot.readStructBegin()
        while True:
            (fname, ftype, fid) = iprot.readFieldBegin()
            if ftype == TType.STOP:
                break
            if fid == 1:
                if ftype == TType.STRING:
                    self.user_id = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
                else:
                    iprot.skip(ftype)
            elif fid == 2:
                if ftype == TType.STRING:
                    self.unique_id = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
                else:
                    iprot.skip(ftype)
            elif fid == 3:
                if ftype == TType.STRING:
                    self.req_id = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
                else:
                    iprot.skip(ftype)
            elif fid == 4:
                if ftype == TType.STRING:
                    self.type = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
                else:
                    iprot.skip(ftype)
            else:
                iprot.skip(ftype)
            iprot.readFieldEnd()
        iprot.readStructEnd()

    def write(self, oprot):
        if oprot._fast_encode is not None and self.thrift_spec is not None:
            oprot.trans.write(oprot._fast_encode(self, (self.__class__, self.thrift_spec)))
            return
        oprot.writeStructBegin('write_timeline_args')
        if self.user_id is not None:
            oprot.writeFieldBegin('user_id', TType.STRING, 1)
            oprot.writeString(self.user_id.encode('utf-8') if sys.version_info[0] == 2 else self.user_id)
            oprot.writeFieldEnd()
        if self.unique_id is not None:
            oprot.writeFieldBegin('unique_id', TType.STRING, 2)
            oprot.writeString(self.unique_id.encode('utf-8') if sys.version_info[0] == 2 else self.unique_id)
            oprot.writeFieldEnd()
        if self.req_id is not None:
            oprot.writeFieldBegin('req_id', TType.STRING, 3)
            oprot.writeString(self.req_id.encode('utf-8') if sys.version_info[0] == 2 else self.req_id)
            oprot.writeFieldEnd()
        if self.type is not None:
            oprot.writeFieldBegin('type', TType.STRING, 4)
            oprot.writeString(self.type.encode('utf-8') if sys.version_info[0] == 2 else self.type)
            oprot.writeFieldEnd()
        oprot.writeFieldStop()
        oprot.writeStructEnd()

    def validate(self):
        return

    def __repr__(self):
        L = ['%s=%r' % (key, value)
             for key, value in self.__dict__.items()]
        return '%s(%s)' % (self.__class__.__name__, ', '.join(L))

    def __eq__(self, other):
        return isinstance(other, self.__class__) and self.__dict__ == other.__dict__

    def __ne__(self, other):
        return not (self == other)
