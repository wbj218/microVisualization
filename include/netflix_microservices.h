//
// Created by Yu Gan on 8/2/17.
//

#ifndef NETFLIXMICROSERVICES_NETFLIX_MICROSERVICES_H
#define NETFLIXMICROSERVICES_NETFLIX_MICROSERVICES_H

#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/Mutex.h>
#include <thrift/concurrency/Monitor.h>
#include <thrift/concurrency/PlatformThreadFactory.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/server/TNonblockingServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/TToString.h>

#include <boost/make_shared.hpp>
#include <iostream>

#include "json.hpp"
#include <csignal>
#include <sys/time.h>
#include <fstream>

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;
using json = nlohmann::json;




#endif //NETFLIXMICROSERVICES_NETFLIX_MICROSERVICES_H



