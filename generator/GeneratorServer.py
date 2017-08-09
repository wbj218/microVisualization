import sys
sys.path.append('./gen-py/')
from TwitterMicroservices import Generator

from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol
from thrift.server import TNonblockingServer
from thrift.server import TServer

import threading
import requests
import time

import numpy
import random
import json

NUM_USERS = 5
NUM_MOVIES = 5

timelist = []
time_dict = {}

thread_lock = threading.Lock()

def server_shutdown(server):
    server.stop()



class GeneratorHandler:
    def __init__(self):
        self.log = {}

    def ping(self):
        print('ping()')

    def run(self):
        data = {}
        data['user_id'] = 'user_' + str(random.randint(0, NUM_USERS - 1))
        data['movie_id'] = 'movie_' + str(random.randint(0, NUM_MOVIES - 1))
        data["req_id"] = data["user_id"] + " " + str (random.randint(0, 0xffffffff))

        thread_lock.acquire()
        timelist.append(time.time())
        time_dict[data["req_id"], 'start'] = int(time.time() * 1000000)
        thread_lock.release()

        r = requests.get('http://128.253.128.65:32834/GetPage.php', params=data)

        thread_lock.acquire()
        time_dict[data["req_id"], 'end'] = int(time.time() * 1000000)
        thread_lock.release()

    def shutdown(self):
        server_shutdown(self.server)


if __name__ == '__main__':
    handler = GeneratorHandler()
    processor = Generator.Processor(handler)
    transport = TSocket.TServerSocket(port = 10100+int(sys.argv[1]))

    pfactory = TBinaryProtocol.TBinaryProtocolFactory()

    server = TNonblockingServer.TNonblockingServer(processor, transport, pfactory)
    handler.add_server(server)

    print('Starting the server...')

    server.serve()
    print("QPS:", len(timelist)/ (timelist[-1] - timelist[0]))
    with open("/home/yg397/Research/NetflixMicroservices/logs/Client_" + sys.argv[1] + ".log", 'w') as file:
        json.dump(time_dict, file)


