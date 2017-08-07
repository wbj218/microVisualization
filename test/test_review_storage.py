import requests

import sys
sys.path.append('../gen-py/')

from NetflixMicroservices import ReviewStorage
from NetflixMicroservices import ttypes

from thrift import Thrift
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol

def main():
    # Make socket
    transport = TSocket.TSocket('localhost', 10000)

    # Buffering is critical. Raw sockets are very slow
    transport = TTransport.TBufferedTransport(transport)

    # Wrap in a protocol
    protocol = TBinaryProtocol.TBinaryProtocol(transport)

    # Create a client to use the protocol encoder
    client = ReviewStorage.Client(protocol)

    # Connect!
    transport.open()
    review = ttypes.Review("11111",                     # unique_id
                           "user_0",                    # user_id
                           "It is a review.",           # text
                           "5",                         # rating
                           "movie_0",                   # movie_id
                           "req_0"                      # req_id
                           )

    client.review_storage("req_0", review)
    review_2 = client.get_review("req_1", "movie_0", "11111")
    transport.close()

if __name__ == '__main__':
    try:
        main()
    except Thrift.TException as tx:
        print('%s' % tx.message)





