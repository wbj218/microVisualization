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
#include <vector>
#include <string>

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

#define DOCKER_IP_ADDR "128.253.128.64"
// #define DOCKER_IP_ADDR "192.168.99.100"
#define LOG_DIR_PATH to_string("../logs/")

#define COMPOSE_REVIEW_PORT 19090
#define UNIQUE_ID_PORT 19091
#define TEXT_PORT 19092
#define RATING_PORT 19093
#define MOVIE_ID_PORT 19094
#define REVIEW_STORE_PORT_START 20000 
#define MOVIE_DB_PORT_START 20010 
#define USER_DB_PORT_START 20020 

#define NUM_MOVIES 5
#define NUM_USERS 5
#define MONGO_MOVIE_DB_PORT_START 42000
#define MMC_MOVIE_DB_PORT_START 42005
#define MONGO_USER_DB_PORT_START 42010
#define MMC_USER_DB_PORT_START 42015
#define MONGO_REVIEW_DB_PORT 42020
#define MMC_REVIEW_DB_PORT 42021




#endif //NETFLIXMICROSERVICES_NETFLIX_MICROSERVICES_H



