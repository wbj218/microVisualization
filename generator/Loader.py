import sys
sys.path.append('../gen-py/')
from NetflixMicroservices import Generator

from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol
from thrift.server import TNonblockingServer
from thrift.server import TServer

import time
import random

def main():
# Make socket
    socket = []
    transport = []
    protocol= []
    client = []

    n_servers = int(sys.argv[1])

    for i in range (n_servers):
        socket.append(TSocket.TSocket('localhost', 10100 + i))
        transport.append(TTransport.TFramedTransport(socket[-1]))
        protocol.append(TBinaryProtocol.TBinaryProtocol(transport[-1]))
        client.append(Generator.Client(protocol[-1]))

    for i in range (int(sys.argv[2])):
        server_index = random.randint(0, n_servers - 1)
        transport[server_index].open()
        client[server_index].run()
        transport[server_index].close()
        time.sleep(1 / float(sys.argv[3]))

    time.sleep(5)
    for i in range (n_servers):
        transport[i].open()
        client[i].shutdown()
        transport[i].close()

if __name__ == '__main__':
    try:
        main()
    except Thrift.TException as tx:
        print('%s' % tx.message)