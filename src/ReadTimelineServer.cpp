//
// Created by Yu Gan on 7/24/17.
//

#include <thrift/concurrency/ThreadManager.h>
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
#include <json.hpp>
#include <regex>
#include <vector>


#include <iostream>
#include <string>

#include <libmongoc-1.0/mongoc.h>
#include "../gen-cpp/ReadTimeline.h"
#include "libmemcached/memcached.h"

#include "../gen-cpp/Generator.h"

#include "json.hpp"
#include <signal.h>
#include <sys/time.h>
#include <fstream>

#define LOG_PATH "../logs/ReadTimeline.log"
#define IF_TRACE true

#define NUM_USERS 5
#define MONGO_PORT_START 32777
#define MMC_PORT_START 32782
#define IP_ADDR "128.253.128.64"
#define IF_TRACE true

using namespace std;
using json = nlohmann::json;
using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;
using namespace TwitterMicroservices;

json logs;

void logger(const string &log_id, const string &service, const string &stage, const string &state) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long time_in_us = tv.tv_sec * 1000000 + tv.tv_usec;
    logs[log_id][service][stage][state] = time_in_us;
}

void exit_handler(int sig) {
    ofstream log_file;
    log_file.open(LOG_PATH);
    log_file << logs;
    log_file.close();

}

class ReadTimelineHandler: public ReadTimelineIf {
public:
    ReadTimelineHandler();
    ~ReadTimelineHandler();
    void ping() { cout << "ping(from server)" << endl; }
    void read_timeline(vector<string> & _return, const string& user_id, const int32_t start_no, const int32_t n_tweets,
                        const string &, const string &);

private:
//    boost::shared_ptr<TTransport> generator_socket;
//    boost::shared_ptr<TTransport> generator_transport;
//    boost::shared_ptr<TProtocol> generator_protocol;
//    GeneratorClient *generator_client;

    mongoc_client_t *mongo_client[NUM_USERS];
    mongoc_collection_t *collection[NUM_USERS];
    memcached_st *mmc[NUM_USERS];
    memcached_return_t mmc_rc;
};

ReadTimelineHandler::ReadTimelineHandler() {
//    try {
//        generator_socket = (boost::shared_ptr<TTransport>) new TSocket("localhost", 9102);
//        generator_transport = (boost::shared_ptr<TTransport>) new TBufferedTransport(generator_socket);
//        generator_protocol = (boost::shared_ptr<TProtocol>) new TBinaryProtocol(generator_transport);
//        generator_client = new GeneratorClient(generator_protocol);
//    } catch (TException& tx) {
//        cout << "ERROR: " << tx.what() << endl;
//    }

    string mmc_configs;
    for (int i = 0; i< NUM_USERS; i++) {
        mongo_client[i] = mongoc_client_new (("mongodb://" + to_string(IP_ADDR) + ":" + to_string(MONGO_PORT_START + i) +
                                              "/?appname=read_timeline").c_str());
        collection[i] =
                mongoc_client_get_collection (mongo_client[i], ("@user_" + to_string(i)).c_str(), "timeline");
         mmc_configs = "--SERVER=" + to_string(IP_ADDR) + ":" + to_string(MMC_PORT_START + i);
        mmc[i] = memcached(mmc_configs.c_str(), mmc_configs.length());
        memcached_behavior_set(mmc[i], MEMCACHED_BEHAVIOR_NO_BLOCK, 1);
        memcached_behavior_set(mmc[i], MEMCACHED_BEHAVIOR_TCP_NODELAY, 1);
        memcached_behavior_set(mmc[i], MEMCACHED_BEHAVIOR_NOREPLY, 1);
        memcached_behavior_set(mmc[i], MEMCACHED_BEHAVIOR_TCP_KEEPALIVE, 1);
    }
}

ReadTimelineHandler::~ReadTimelineHandler() {
    for (int i = 0; i< NUM_USERS; i++) {
        mongoc_client_destroy (mongo_client[i]);
        mongoc_collection_destroy (collection[i]);
        memcached_free(mmc[i]);
    }
}

void ReadTimelineHandler::read_timeline(vector<string> & _return, const string& user_id, const int32_t start_no,
                                        const int32_t n_tweets, const string & type, const string &req_id) {
    if (IF_TRACE)
        logger(req_id, "ReadTimeline", "read_timeline",  "begin");

    char* value_char;
    json value_json;
    string value_str;
    smatch m;
    regex e("([0-9]+)");
    regex_search(user_id, m, e);
    int index = stoi(m.str());

    memcached_return_t mmc_rc;
    uint32_t mmc_flags;
    size_t mmc_value_length;
    value_char = memcached_get(mmc[index], type.c_str(), type.length(), &mmc_value_length, &mmc_flags, &mmc_rc);
    if (value_char){
        value_json = json::parse(value_char);
        for (int i = start_no; i < n_tweets + start_no; i++) {
            _return.push_back(value_json[type][i]);
        }
    }
    else {
        bson_t *query;
        query = bson_new();
        mongoc_cursor_t *cursor;
        const bson_t *doc;
        BSON_APPEND_UTF8(query, "type", type.c_str());
        cursor = mongoc_collection_find_with_opts (collection[index], query, NULL, NULL);
        if (mongoc_cursor_next (cursor, &doc)) {
            value_json = json::parse(bson_as_json (doc, NULL));
            for (int i = start_no; i < n_tweets + start_no; i++) {
                _return.push_back(value_json[type][i]);
            }
            value_str = value_json.dump(4);
            memcached_set(mmc[index], type.c_str(), type.length(), value_str.c_str(), value_str.length(),
                          (time_t) 0, (uint32_t) 0);
        }
        mongoc_cursor_destroy (cursor);
        bson_destroy(query);
    }
    if (IF_TRACE)
        logger(req_id, "ReadTimeline", "read_timeline",  "end");
}

int main() {
//    boost::shared_ptr<TProcessor> processor(new MongoTweetStorageProcessor(boost::make_shared<MongoTweetStorageHandler>()));
//    boost::shared_ptr<TProtocolFactory> protocol_factory(new TBinaryProtocolFactory());
//    boost::shared_ptr<ThreadManager> thread_manager = ThreadManager::newSimpleThreadManager(4);
//    boost::shared_ptr<PosixThreadFactory> thread_factory = boost::shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
//    thread_manager->threadFactory(thread_factory);
//    thread_manager->start();
//    TNonblockingServer server(processor, protocol_factory, 9098, thread_manager);

    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    TSimpleServer server(
            boost::make_shared<ReadTimelineProcessor>(boost::make_shared<ReadTimelineHandler>()),
            boost::make_shared<TServerSocket>(9108),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());
    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}






