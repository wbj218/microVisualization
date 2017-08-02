//
// Created by Yu Gan on 7/10/17.
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
#include <iostream>
#include <random>
#include <string>

#include "../gen-cpp/ProcessUniqueID.h"
#include "../gen-cpp/ComposeTweet.h"
#include "../gen-cpp/ComposeRetweet.h"
#include "../gen-cpp/ComposeDM.h"
#include "../gen-cpp/ComposeReply.h"

#include "json.hpp"
#include <signal.h>
#include <sys/time.h>
#include <fstream>

#define LOG_PATH "../logs/ProcessUniqueID.log"
#define IF_TRACE true

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;
using namespace TwitterMicroservices;
using json = nlohmann::json;



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

class ProcessUniqueIDHandler: public ProcessUniqueIDIf {
public:
    ProcessUniqueIDHandler();
    ~ProcessUniqueIDHandler();
    void ping() {cout << "ping(from server)" << endl;}
    void get_unique_id(const string&, const string&);

private:
    default_random_engine generator;
    boost::shared_ptr<TTransport> compose_socket;
    boost::shared_ptr<TTransport> compose_transport;
    boost::shared_ptr<TProtocol> compose_protocol;
    ComposeTweetClient *compose_client;

    boost::shared_ptr<TTransport> reply_socket;
    boost::shared_ptr<TTransport> reply_transport;
    boost::shared_ptr<TProtocol> reply_protocol;
    ComposeReplyClient *reply_client;

    boost::shared_ptr<TTransport> retweet_socket;
    boost::shared_ptr<TTransport> retweet_transport;
    boost::shared_ptr<TProtocol> retweet_protocol;
    ComposeRetweetClient *retweet_client;

    boost::shared_ptr<TTransport> dm_socket;
    boost::shared_ptr<TTransport> dm_transport;
    boost::shared_ptr<TProtocol> dm_protocol;
    ComposeDMClient *dm_client;


};

ProcessUniqueIDHandler::ProcessUniqueIDHandler() {
    generator.seed((unsigned int) time(NULL));
    try {
        compose_socket = (boost::shared_ptr<TTransport>) new TSocket("localhost", 9090);
        compose_transport = (boost::shared_ptr<TTransport>) new TBufferedTransport(compose_socket);
        compose_protocol = (boost::shared_ptr<TProtocol>) new TBinaryProtocol(compose_transport);
        compose_client = new ComposeTweetClient(compose_protocol);

        reply_socket = (boost::shared_ptr<TTransport>) new TSocket("localhost", 9103);
        reply_transport = (boost::shared_ptr<TTransport>)new TBufferedTransport(reply_socket);
        reply_protocol = (boost::shared_ptr<TProtocol>)new TBinaryProtocol(reply_transport);
        reply_client = new ComposeReplyClient(reply_protocol);

        retweet_socket = (boost::shared_ptr<TTransport>) new TSocket("localhost", 9104);
        retweet_transport = (boost::shared_ptr<TTransport>)new TBufferedTransport(retweet_socket);
        retweet_protocol = (boost::shared_ptr<TProtocol>)new TBinaryProtocol(retweet_transport);
        retweet_client = new ComposeRetweetClient(retweet_protocol);

        dm_socket = (boost::shared_ptr<TTransport>) new TSocket("localhost", 9105);
        dm_transport = (boost::shared_ptr<TTransport>)new TBufferedTransport(dm_socket);
        dm_protocol = (boost::shared_ptr<TProtocol>)new TBinaryProtocol(dm_transport);
        dm_client = new ComposeDMClient(dm_protocol);


    } catch (TException& tx) {
        cout << "ERROR: " << tx.what() << endl;
    }

}

ProcessUniqueIDHandler::~ProcessUniqueIDHandler() {
    delete compose_client;
    delete reply_client;
    delete retweet_client;
    delete dm_client;

}

void ProcessUniqueIDHandler::get_unique_id(const string& req_id, const string& type) {
    logger(req_id, "ProcessUniqueID", "process_unique_id", "begin");
    uniform_int_distribution<unsigned long long > distribution(0, ULLONG_MAX);
    try {
//        mmc_transport->open();
//        mmc_client->set_data(req_id + "unique_id", to_string(distribution(generator)));
//        mmc_transport->close();
//        compose_transport->open();
//        compose_client->notify(req_id, "unique_id");
//        compose_transport->close();
        if (type == "TWEET") {
            compose_transport->open();
            compose_client->upload(req_id, "unique_id", to_string(distribution(generator)));
            compose_transport->close();
        }
        else if (type == "RETWEET") {
            retweet_transport->open();
            retweet_client->upload(req_id, "unique_id", to_string(distribution(generator)));
            retweet_transport->close();
        }
        else if (type == "REPLY") {
            reply_transport->open();
            reply_client->upload(req_id, "unique_id", to_string(distribution(generator)));
            reply_transport->close();
        }
        else if (type == "DM") {
            dm_transport->open();
            dm_client->upload(req_id, "unique_id", to_string(distribution(generator)));
            dm_transport->close();
        }
        

    } catch (TException& tx) {
        cout << "ERROR: " << tx.what() << endl;
    }
    logger(req_id, "ProcessUniqueID", "process_unique_id", "end");

}


int main() {
//    boost::shared_ptr<TProcessor> processor(new ProcessUniqueIDProcessor(boost::make_shared<ProcessUniqueIDHandler>()));
//    boost::shared_ptr<TProtocolFactory> protocol_factory(new TBinaryProtocolFactory());
//    boost::shared_ptr<ThreadManager> thread_manager = ThreadManager::newSimpleThreadManager(4);
//    boost::shared_ptr<PosixThreadFactory> thread_factory = boost::shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
//    thread_manager->threadFactory(thread_factory);
//    thread_manager->start();
//    TNonblockingServer server(processor, protocol_factory, 9096, thread_manager);
    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    TSimpleServer server(
            boost::make_shared<ProcessUniqueIDProcessor>(boost::make_shared<ProcessUniqueIDHandler>()),
            boost::make_shared<TServerSocket>(9096),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}



