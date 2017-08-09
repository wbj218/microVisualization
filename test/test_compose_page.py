import sys
sys.path.append('../gen-py/')

from NetflixMicroservices import ComposePage

from thrift import Thrift
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol

def main():
    socket = TSocket.TSocket('localhost', 10050)
    transport = TTransport.TBufferedTransport(socket)
    protocol = TBinaryProtocol.TBinaryProtocol(transport)
    client = ComposePage.Client(protocol)

    for i in range(1000):
        transport.open()
        page = client.compose_page("req_" + str(int(sys.argv[1]) + i), "movie_0", "user_0")
        transport.close()

if __name__ == '__main__':
    try:
        main()
    except Thrift.TException as tx:
        print('%s' % tx.message)