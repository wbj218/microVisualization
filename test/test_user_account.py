import requests

import sys
sys.path.append('../gen-py/')

from NetflixMicroservices import UserAccount

from thrift import Thrift
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol

def main():
    # Make socket
    transport = TSocket.TSocket('localhost', 10060)

    # Buffering is critical. Raw sockets are very slow
    transport = TTransport.TBufferedTransport(transport)

    # Wrap in a protocol
    protocol = TBinaryProtocol.TBinaryProtocol(transport)

    # Create a client to use the protocol encoder
    client = UserAccount.Client(protocol)

    # Connect!
    transport.open()
    client.add_account("111", "user_0", 100)
    if_success = client.purchase("222", "user_0", "movie_0")
    if_purchase = client.if_purchased("333", "user_0", "movie_0")
    transport.close()
    print (if_success,if_purchase)

if __name__ == '__main__':
    try:
        main()
    except Thrift.TException as tx:
        print('%s' % tx.message)