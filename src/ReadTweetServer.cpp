//
// Created by Yu Gan on 7/23/17.
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
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <time.h>
#include "../gen-cpp/ReadTweet.h"
#include "../gen-cpp/MongoTweetStorage.h"
#include <json.hpp>

#include "json.hpp"
#include <signal.h>
#include <sys/time.h>
#include <fstream>


#define LOG_PATH "../logs/ReadTweet.log"
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

class ReadTweetHandler: public ReadTweetIf {
public:
    ReadTweetHandler(const int, const int *);

    ~ReadTweetHandler();

    void ping() { cout << "ping(from server)" << endl; }

    void read_tweet(Tweet &, const string &, const string &, const string &, const string &req_id);


private:
    int n_tweet_store;
    const int *tweet_store_ports;

    boost::shared_ptr<TTransport>* mongo_tweet_storage_socket;
    boost::shared_ptr<TTransport>* mongo_tweet_storage_transport;
    boost::shared_ptr<TProtocol>* mongo_tweet_storage_protocol;
    boost::shared_ptr<MongoTweetStorageClient>* mongo_tweet_storage_client;

};

ReadTweetHandler::ReadTweetHandler(const int n_tweet_store, const int* tweet_store_ports) {

    this->n_tweet_store = n_tweet_store;
    this->tweet_store_ports = tweet_store_ports;

    

    try {  
        mongo_tweet_storage_socket = new boost::shared_ptr<TTransport>[n_tweet_store];
        mongo_tweet_storage_transport = new boost::shared_ptr<TTransport>[n_tweet_store];
        mongo_tweet_storage_protocol = new boost::shared_ptr<TProtocol>[n_tweet_store];
        mongo_tweet_storage_client = new boost::shared_ptr<MongoTweetStorageClient>[n_tweet_store];

        for (int i = 0; i < n_tweet_store; i++) {
            cout<<"store"<<tweet_store_ports[i]<<endl;
            mongo_tweet_storage_socket[i] = (boost::shared_ptr<TTransport>) new TSocket("localhost", tweet_store_ports[i]);
            mongo_tweet_storage_transport[i] =
                (boost::shared_ptr<TTransport>) new TBufferedTransport(mongo_tweet_storage_socket[i]);
            mongo_tweet_storage_protocol[i] =
                (boost::shared_ptr<TProtocol>) new TBinaryProtocol(mongo_tweet_storage_transport[i]);
            mongo_tweet_storage_client[i] =
                (boost::shared_ptr<MongoTweetStorageClient>)new MongoTweetStorageClient(mongo_tweet_storage_protocol[i]);
        }
    } catch (TException& tx) {
        cout << "ERROR: " << tx.what() << endl;
    }
}

ReadTweetHandler::~ReadTweetHandler() {
    delete[] mongo_tweet_storage_socket;
    delete[] mongo_tweet_storage_transport;
    delete[] mongo_tweet_storage_protocol;
    delete[] mongo_tweet_storage_client;
}

void ReadTweetHandler::read_tweet(Tweet& _return, const string& user_id, const string& unique_id, const string& type, const string &req_id){
    if (IF_TRACE)
        logger(req_id, "ReadTweet", "read_tweet", "begin");


    string return_str;
    json return_json;
    cout<<"read_tweet"<<" "<<req_id<<endl;
    
    // cout<<user_id<<" "<<unique_id<<endl;
    try {
        int tweet_store_index = rand() % n_tweet_store;
        cout<<tweet_store_index<<endl;
        mongo_tweet_storage_transport[tweet_store_index]->open();
        mongo_tweet_storage_client[tweet_store_index]->get_tweet(return_str, user_id, unique_id, req_id);
        mongo_tweet_storage_transport[tweet_store_index]->close();
    } catch (TException& tx) {
        cout << "ERROR: " << tx.what() << endl;
    }

    // cout<<"return "<<return_str<<endl;

    return_json = json::parse(return_str);
    _return.tweet_type = return_json["tweet_type"];
    _return.unique_id = return_json["unique_id"];
    _return.text = return_json["text"];

    // cout<<"original_tweet"<<_return.unique_id<<endl;

    _return.user_id = return_json["user_id"];
    assert(_return.unique_id == unique_id);
    assert(_return.user_id == user_id);
    if (return_json.find("video") != return_json.end())
        _return.video = return_json["video"];
    else
        _return.video = "NULL";

    if (return_json.find("image") != return_json.end())
        _return.image = return_json["image"];
    else
        _return.image = "NULL";

    if (return_json.find("urls") != return_json.end())
        _return.urls = return_json["urls"];
    else
        _return.urls = "NULL";

    if (return_json.find("tags") != return_json.end())
        _return.tags = return_json["tags"];
    else
        _return.tags = "NULL";

    if (IF_TRACE)
        logger(req_id, "ReadTweet", "read_tweet", "end");

}



int main(int argc, char *argv[]) {
//    boost::shared_ptr<TProcessor> processor(new MongoTweetStorageProcessor(boost::make_shared<MongoTweetStorageHandler>()));
//    boost::shared_ptr<TProtocolFactory> protocol_factory(new TBinaryProtocolFactory());
//    boost::shared_ptr<ThreadManager> thread_manager = ThreadManager::newSimpleThreadManager(4);
//    boost::shared_ptr<PosixThreadFactory> thread_factory = boost::shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
//    thread_manager->threadFactory(thread_factory);
//    thread_manager->start();
//    TNonblockingServer server(processor, protocol_factory, 9098, thread_manager);
    int n_tweet_store = stoi(argv[1]);
    int *tweet_store_ports = new int[n_tweet_store];
    for (int i = 0; i < n_tweet_store; i++){
        tweet_store_ports[i] = stoi(argv[2 + i]);
        cout<<"store "<<tweet_store_ports[i]<<endl;
    }

    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);
    srand((int) time(0));

    TSimpleServer server(
            boost::make_shared<ReadTweetProcessor>(boost::make_shared<ReadTweetHandler>(n_tweet_store, tweet_store_ports)),
            boost::make_shared<TServerSocket>(9107),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());
    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}



