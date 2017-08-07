import requests

import sys
sys.path.append('../gen-py/')

from NetflixMicroservices import MovieReviewDB

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
    client = MovieReviewDB.Client(protocol)

    # Connect!
    transport.open()


    _return = client.get_movie_review("req_3",  "movie_0", 0, 1)
    # client.write_movie_review("user_0 req_0", "movie_0", "user_0", "11111", "5")
    # client.write_movie_review("user_1 req_1", "movie_0", "user_1", "22222", "5")
    # client.write_movie_review("user_2 req_2", "movie_0", "user_2", "33333", "5")
    transport.close()
    print(_return)


if __name__ == '__main__':
    try:
        main()
    except Thrift.TException as tx:
        print('%s' % tx.message)





