import sys
sys.path.append('../gen-py/')

from NetflixMicroservices import GetMovieReview

from thrift import Thrift
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol

def main():
    socket = TSocket.TSocket('localhost', 10046)
    transport = TTransport.TBufferedTransport(socket)
    protocol = TBinaryProtocol.TBinaryProtocol(transport)
    client = GetMovieReview.Client(protocol)

    transport.open()
    review = client.get_movie_review("req_6", "movie_0", 1, 3)

    transport.close()

if __name__ == '__main__':
    try:
        main()
    except Thrift.TException as tx:
        print('%s' % tx.message)
