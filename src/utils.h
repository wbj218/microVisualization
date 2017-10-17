//
// Created by yugan on 10/17/17.
//

#ifndef NETFLIXMICROSERVICES_UTILS_H
#define NETFLIXMICROSERVICES_UTILS_H

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
#include <csignal>
#include <fstream>
#include <string>
#include <mutex>
#include <sys/time.h>
#include <map>
#include <random>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include "nlohmann/json.hpp"
#include <condition_variable>
#include <thread>
#include "libmemcached/memcached.h"
#include <libmongoc-1.0/mongoc.h>
#include <mutex>
#include <sstream>





#include "../gen-cpp/GetMovieReview.h"
#include "../gen-cpp/ComposePage.h"
#include "../gen-cpp/GetCastInfo.h"
#include "../gen-cpp/GetPhoto.h"
#include "../gen-cpp/GetThumbnail.h"
#include "../gen-cpp/GetPlot.h"
#include "../gen-cpp/GetRating.h"
#include "../gen-cpp/GetVideo.h"
#include "../gen-cpp/GetWatchNext.h"
#include "../gen-cpp/NetflixMicroservices_types.h"
#include "../gen-cpp/ComposeReview.h"
#include "../gen-cpp/ReviewStorage.h"
#include "../gen-cpp/MovieReviewDB.h"
#include "../gen-cpp/UserReviewDB.h"
#include "../gen-cpp/MovieInfoStorage.h"
#include "../gen-cpp/AssignRating.h"
#include "../gen-cpp/ProcessText.h"
#include "../gen-cpp/ProcessUniqueID.h"
#include "../gen-cpp/ProcessMovieID.h"
#include "../gen-cpp/UserAccount.h"








using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;
using json = nlohmann::json;

void logger(const string &log_id, const string &service, const string &stage, const string &state,
            mutex &mutex, json &logs);

#endif //NETFLIXMICROSERVICES_UTILS_H










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