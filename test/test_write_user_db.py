import requests

import sys
sys.path.append('../gen-py/')

from NetflixMicroservices import WriteUserDB

from thrift import Thrift
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol

def main():
    # Make socket
    transport = TSocket.TSocket('localhost', 10010)

    # Buffering is critical. Raw sockets are very slow
    transport = TTransport.TBufferedTransport(transport)

    # Wrap in a protocol
    protocol = TBinaryProtocol.TBinaryProtocol(transport)

    # Create a client to use the protocol encoder
    client = WriteUserDB.Client(protocol)

    # Connect!
    transport.open()

    client.write_movie_db("user_0 req_0", "movie_0", "user_0", "11111")
    # client.write_movie_db("user_0 req_0", "movie_0", "user_0", "22222")
    # client.write_movie_db("user_0 req_0", "movie_0", "user_0", "33333")


if __name__ == '__main__':
    try:
        main()
    except Thrift.TException as tx:
        print('%s' % tx.message)





