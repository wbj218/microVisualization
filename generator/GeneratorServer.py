import sys
sys.path.append('../gen-py/')
from NetflixMicroservices import Generator

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


    # #compose page
    # def run(self):
    #     data = {}
    #     data['user_id'] = 'user_' + str(random.randint(0, NUM_USERS - 1))
    #     movie_id = 'movie_' + str(random.randint(0, NUM_MOVIES - 1))
    #     # movie_id = 'movie_0'
    #     data["req_id"] = data["user_id"] + " " + str (random.randint(0, 0xffffffff))
    #     data["url"] = "http://www.imdb.com/title/" + movie_id

    #     thread_lock.acquire()
    #     timelist.append(time.time())
    #     time_dict[data["req_id"]] = {}
    #     time_dict[data["req_id"]]['begin'] = int(time.time() * 1000000)
    #     thread_lock.release()

    #     r = requests.get('http://128.253.128.64:32800/GetPage.php', params=data)

    #     thread_lock.acquire()
    #     time_dict[data["req_id"]]['end'] = int(time.time() * 1000000)
    #     thread_lock.release()



    # compose review
    def run(self):
        data = {}
        data['user_id'] = 'user_' + str(random.randint(0, NUM_USERS - 1))
        movie_id = 'movie_' + str(random.randint(0, NUM_MOVIES - 1))
        # data["req_id"] = data["user_id"] + " " + str (random.randint(0, 0xffffffff))
        data["url"] = "http://www.imdb.com/title/" + movie_id
        data['rating'] = str(random.randint(0, 5))
        data['text'] = 'It is a movie review! It is a movie review! It is a movie review! It is a movie review! It is a movie review! It is a movie review! It is a movie review! It is a movie review! It is a movie review! '

        thread_lock.acquire()
        timelist.append(time.time())
        thread_lock.release()
        start_time = int(time.time() * 1000000) 
        r = requests.post('http://128.253.128.64:32800/ComposeReview.php', data=data)
        end_time = int(time.time() * 1000000) 
        r.content.split('\n')
        php_start_time = r.content[0][2:]
        php_end_time = r.content[0][2:]


        time_dict[data["req_id"]] = {}
        time_dict[data["req_id"]]['begin'] = 

  

    # # movie streaming
    # def run(self):
    #     data = {}
    #     data["req_id"] = str(random.randint(0, 0xffffffff))    
    #     thread_lock.acquire()
    #     timelist.append(time.time())
    #     time_dict[data["req_id"]] = {}
    #     time_dict[data["req_id"]]['begin'] = int(time.time() * 1000000)
    #     thread_lock.release()    

    #     r = requests.get('http://128.253.128.64:32800/hls/user_0/' + data["req_id"] + '/movie_0.mp4')

    #     thread_lock.acquire()
    #     time_dict[data["req_id"]]['end'] = int(time.time() * 1000000)
    #     thread_lock.release()



    def add_server(self, server):
        self.server = server



    def shutdown(self):
        server_shutdown(self.server)


if __name__ == '__main__':
    handler = GeneratorHandler()
    processor = Generator.Processor(handler)
    transport = TSocket.TServerSocket(port = 10100 + int(sys.argv[1]))

    pfactory = TBinaryProtocol.TBinaryProtocolFactory()

    server = TNonblockingServer.TNonblockingServer(processor, transport, pfactory)
    handler.add_server(server)

    print('Starting the server...')

    server.serve()
    with open("/home/yg397/Research/NetflixMicroservices/logs/Client_" + sys.argv[1] + ".log", 'w') as file:
        json.dump(time_dict, file)
    print(len(timelist)/ (timelist[-1] - timelist[0]))
 


