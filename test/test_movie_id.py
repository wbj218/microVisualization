import requests

import sys
sys.path.append('../gen-py/')

from NetflixMicroservices import ProcessMovieID

from thrift import Thrift
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol

def main():
    # Make socket
    transport = TSocket.TSocket('localhost', 9094)

    # Buffering is critical. Raw sockets are very slow
    transport = TTransport.TBufferedTransport(transport)

    # Wrap in a protocol
    protocol = TBinaryProtocol.TBinaryProtocol(transport)

    # Create a client to use the protocol encoder
    client = ProcessMovieID.Client(protocol)

    # Connect!
    transport.open()

    client.get_movie_id("req_0", "http://www.imdb.com/title/tt5555260")


if __name__ == '__main__':
    try:
        main()
    except Thrift.TException as tx:
        print('%s' % tx.message)





