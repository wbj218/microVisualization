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

from thrift.transport import TTransport


class Tweet(object):
    """
    Attributes:
     - unique_id
     - user_id
     - text
     - image
     - video
     - tags
     - urls
     - tweet_type
     - recipient_id
     - orig_tweet_id
     - retweet_text
     - retweet_tags
     - retweet_urls
     - orig_user_id
     - req_id
    """

    thrift_spec = (
        None,  # 0
        (1, TType.STRING, 'unique_id', 'UTF8', None, ),  # 1
        (2, TType.STRING, 'user_id', 'UTF8', None, ),  # 2
        (3, TType.STRING, 'text', 'UTF8', None, ),  # 3
        (4, TType.STRING, 'image', 'UTF8', None, ),  # 4
        (5, TType.STRING, 'video', 'UTF8', None, ),  # 5
        (6, TType.STRING, 'tags', 'UTF8', None, ),  # 6
        (7, TType.STRING, 'urls', 'UTF8', None, ),  # 7
        (8, TType.STRING, 'tweet_type', 'UTF8', None, ),  # 8
        (9, TType.STRING, 'recipient_id', 'UTF8', None, ),  # 9
        (10, TType.STRING, 'orig_tweet_id', 'UTF8', None, ),  # 10
        (11, TType.STRING, 'retweet_text', 'UTF8', None, ),  # 11
        (12, TType.STRING, 'retweet_tags', 'UTF8', None, ),  # 12
        (13, TType.STRING, 'retweet_urls', 'UTF8', None, ),  # 13
        (14, TType.STRING, 'orig_user_id', 'UTF8', None, ),  # 14
        (15, TType.STRING, 'req_id', 'UTF8', None, ),  # 15
    )

    def __init__(self, unique_id=None, user_id=None, text=None, image=None, video=None, tags=None, urls=None, tweet_type=None, recipient_id=None, orig_tweet_id=None, retweet_text=None, retweet_tags=None, retweet_urls=None, orig_user_id=None, req_id=None,):
        self.unique_id = unique_id
        self.user_id = user_id
        self.text = text
        self.image = image
        self.video = video
        self.tags = tags
        self.urls = urls
        self.tweet_type = tweet_type
        self.recipient_id = recipient_id
        self.orig_tweet_id = orig_tweet_id
        self.retweet_text = retweet_text
        self.retweet_tags = retweet_tags
        self.retweet_urls = retweet_urls
        self.orig_user_id = orig_user_id
        self.req_id = req_id

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
                    self.unique_id = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
                else:
                    iprot.skip(ftype)
            elif fid == 2:
                if ftype == TType.STRING:
                    self.user_id = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
                else:
                    iprot.skip(ftype)
            elif fid == 3:
                if ftype == TType.STRING:
                    self.text = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
                else:
                    iprot.skip(ftype)
            elif fid == 4:
                if ftype == TType.STRING:
                    self.image = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
                else:
                    iprot.skip(ftype)
            elif fid == 5:
                if ftype == TType.STRING:
                    self.video = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
                else:
                    iprot.skip(ftype)
            elif fid == 6:
                if ftype == TType.STRING:
                    self.tags = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
                else:
                    iprot.skip(ftype)
            elif fid == 7:
                if ftype == TType.STRING:
                    self.urls = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
                else:
                    iprot.skip(ftype)
            elif fid == 8:
                if ftype == TType.STRING:
                    self.tweet_type = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
                else:
                    iprot.skip(ftype)
            elif fid == 9:
                if ftype == TType.STRING:
                    self.recipient_id = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
                else:
                    iprot.skip(ftype)
            elif fid == 10:
                if ftype == TType.STRING:
                    self.orig_tweet_id = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
                else:
                    iprot.skip(ftype)
            elif fid == 11:
                if ftype == TType.STRING:
                    self.retweet_text = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
                else:
                    iprot.skip(ftype)
            elif fid == 12:
                if ftype == TType.STRING:
                    self.retweet_tags = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
                else:
                    iprot.skip(ftype)
            elif fid == 13:
                if ftype == TType.STRING:
                    self.retweet_urls = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
                else:
                    iprot.skip(ftype)
            elif fid == 14:
                if ftype == TType.STRING:
                    self.orig_user_id = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
                else:
                    iprot.skip(ftype)
            elif fid == 15:
                if ftype == TType.STRING:
                    self.req_id = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
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
        oprot.writeStructBegin('Tweet')
        if self.unique_id is not None:
            oprot.writeFieldBegin('unique_id', TType.STRING, 1)
            oprot.writeString(self.unique_id.encode('utf-8') if sys.version_info[0] == 2 else self.unique_id)
            oprot.writeFieldEnd()
        if self.user_id is not None:
            oprot.writeFieldBegin('user_id', TType.STRING, 2)
            oprot.writeString(self.user_id.encode('utf-8') if sys.version_info[0] == 2 else self.user_id)
            oprot.writeFieldEnd()
        if self.text is not None:
            oprot.writeFieldBegin('text', TType.STRING, 3)
            oprot.writeString(self.text.encode('utf-8') if sys.version_info[0] == 2 else self.text)
            oprot.writeFieldEnd()
        if self.image is not None:
            oprot.writeFieldBegin('image', TType.STRING, 4)
            oprot.writeString(self.image.encode('utf-8') if sys.version_info[0] == 2 else self.image)
            oprot.writeFieldEnd()
        if self.video is not None:
            oprot.writeFieldBegin('video', TType.STRING, 5)
            oprot.writeString(self.video.encode('utf-8') if sys.version_info[0] == 2 else self.video)
            oprot.writeFieldEnd()
        if self.tags is not None:
            oprot.writeFieldBegin('tags', TType.STRING, 6)
            oprot.writeString(self.tags.encode('utf-8') if sys.version_info[0] == 2 else self.tags)
            oprot.writeFieldEnd()
        if self.urls is not None:
            oprot.writeFieldBegin('urls', TType.STRING, 7)
            oprot.writeString(self.urls.encode('utf-8') if sys.version_info[0] == 2 else self.urls)
            oprot.writeFieldEnd()
        if self.tweet_type is not None:
            oprot.writeFieldBegin('tweet_type', TType.STRING, 8)
            oprot.writeString(self.tweet_type.encode('utf-8') if sys.version_info[0] == 2 else self.tweet_type)
            oprot.writeFieldEnd()
        if self.recipient_id is not None:
            oprot.writeFieldBegin('recipient_id', TType.STRING, 9)
            oprot.writeString(self.recipient_id.encode('utf-8') if sys.version_info[0] == 2 else self.recipient_id)
            oprot.writeFieldEnd()
        if self.orig_tweet_id is not None:
            oprot.writeFieldBegin('orig_tweet_id', TType.STRING, 10)
            oprot.writeString(self.orig_tweet_id.encode('utf-8') if sys.version_info[0] == 2 else self.orig_tweet_id)
            oprot.writeFieldEnd()
        if self.retweet_text is not None:
            oprot.writeFieldBegin('retweet_text', TType.STRING, 11)
            oprot.writeString(self.retweet_text.encode('utf-8') if sys.version_info[0] == 2 else self.retweet_text)
            oprot.writeFieldEnd()
        if self.retweet_tags is not None:
            oprot.writeFieldBegin('retweet_tags', TType.STRING, 12)
            oprot.writeString(self.retweet_tags.encode('utf-8') if sys.version_info[0] == 2 else self.retweet_tags)
            oprot.writeFieldEnd()
        if self.retweet_urls is not None:
            oprot.writeFieldBegin('retweet_urls', TType.STRING, 13)
            oprot.writeString(self.retweet_urls.encode('utf-8') if sys.version_info[0] == 2 else self.retweet_urls)
            oprot.writeFieldEnd()
        if self.orig_user_id is not None:
            oprot.writeFieldBegin('orig_user_id', TType.STRING, 14)
            oprot.writeString(self.orig_user_id.encode('utf-8') if sys.version_info[0] == 2 else self.orig_user_id)
            oprot.writeFieldEnd()
        if self.req_id is not None:
            oprot.writeFieldBegin('req_id', TType.STRING, 15)
            oprot.writeString(self.req_id.encode('utf-8') if sys.version_info[0] == 2 else self.req_id)
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
