import requests

import sys
sys.path.append('../gen-py/')

from NetflixMicroservices import GetWatchNext

from thrift import Thrift
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol

def main():
    # Make socket
    transport = TSocket.TSocket('localhost', 10047)

    # Buffering is critical. Raw sockets are very slow
    transport = TTransport.TBufferedTransport(transport)

    # Wrap in a protocol
    protocol = TBinaryProtocol.TBinaryProtocol(transport)

    # Create a client to use the protocol encoder
    client = GetWatchNext.Client(protocol)

    # Connect!
    transport.open()
    _return = client.get_watch_next("111", "user_5")
    transport.close()

    print(_return)



if __name__ == '__main__':
    try:
        main()
    except Thrift.TException as tx:
        print('%s' % tx.message)





