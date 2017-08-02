//
// Created by Yu Gan on 7/10/17.
//

#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/Mutex.h>
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
#include <regex>
#include <string>
#include <libmongoc-1.0/mongoc.h>
#include "../gen-cpp/ProcessTag.h"
#include "../gen-cpp/ComposeTweet.h"
#include "../gen-cpp/ComposeRetweet.h"
#include "../gen-cpp/ComposeDM.h"
#include "../gen-cpp/ComposeReply.h"

#include "json.hpp"
#include <signal.h>
#include <sys/time.h>
#include <fstream>

#define LOG_PATH "../logs/ProcessTag.log"
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
// Mutex thread_mx;

void logger(const string &log_id, const string &service, const string &stage, const string &state) {
    // thread_mx.lock();
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long time_in_us = tv.tv_sec * 1000000 + tv.tv_usec;
    logs[log_id][service][stage][state] = time_in_us;
    // thread_mx.unlock();
}



void exit_handler(int sig) {
    ofstream log_file;
    log_file.open(LOG_PATH);
    log_file << logs;
    log_file.close();

}


//mutex thread_mx;

class ProcessTagHandler: public ProcessTagIf {
public:
    ProcessTagHandler();
    ~ProcessTagHandler();
    void ping() {cout << "ping(from server)" << endl;}
    void process_tag(const string&, const string&, const string&);

private:
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

    mongoc_gridfs_t *gridfs;
    mongoc_gridfs_file_t *file;
    mongoc_client_t *mongo_client;
    bson_error_t error;
    string buf;
    mongoc_iovec_t iov;
    ssize_t r;

};

ProcessTagHandler::ProcessTagHandler() {
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

    
    iov.iov_base = (void *) buf.c_str();
    iov.iov_len = sizeof buf;
    mongo_client = mongoc_client_new ("mongodb://128.253.128.64:32801");
    assert (mongo_client);
    mongoc_client_set_error_api (mongo_client, 2);
    gridfs = mongoc_client_get_gridfs (mongo_client, "tags", "fs", &error);
    assert (gridfs);
}

ProcessTagHandler::~ProcessTagHandler() {
    delete compose_client;
    delete retweet_client;
    delete reply_client;
    delete dm_client;
}

void ProcessTagHandler::process_tag(const string& req_id, const string& text_data, const string& type) {
    if (IF_TRACE)
        logger(req_id, "ProcessTag", "process_tag", "begin");
    string _return = "";

    string s = text_data;
    smatch m;
    regex e("(@)([a-zA-Z0-9_-]+)?");
    //thread_mx.lock();
    int counter = 0;
    while (regex_search(s, m, e)) {
        if (type == "DM" || type == "REPLY") {
            counter ++;
            if (counter < 2){
                s=m.suffix().str();
                continue;
            }

        }
        file = mongoc_gridfs_find_one_by_filename (gridfs, m.str().c_str(), &error);
        if (file) {
            r = mongoc_gridfs_file_readv(file, &iov, 1, 0, 0);
            mongoc_gridfs_file_destroy (file);
            _return = _return + m.str() + " " + buf + "\n";

        }
        else{
            _return = _return + m.str() + " NULL" + "\n";
        }

        s=m.suffix().str();
    }
  //  thread_mx.unlock();
    // cout<<_return<<endl;
    try {
//        mmc_transport->open();
//        mmc_client->set_data(req_id + "tag", _return);
//        mmc_transport->close();
//        compose_transport->open();
//        compose_client->notify(req_id, "tag");
//        compose_transport->close();

        if (type == "TWEET") {
            compose_transport->open();
            if (_return != "")
                compose_client->upload(req_id, "tag", _return);
            else
                compose_client->upload(req_id, "tag", "NULL");
            compose_transport->close();
        }
        else if (type == "REPLY") {
            reply_transport->open();
            if (_return != "")
                reply_client->upload(req_id, "tag", _return);
            else
                reply_client->upload(req_id, "tag", "NULL");
            reply_transport->close();
        }
        else if (type == "DM") {
            dm_transport->open();
            if (_return != "")
                dm_client->upload(req_id, "tag", _return);
            else
                dm_client->upload(req_id, "tag", "NULL");
            dm_transport->close();
        }
        else if (type == "RETWEET") {
            retweet_transport->open();
            if (_return != "")
                retweet_client->upload(req_id, "tag", _return);
            else
                retweet_client->upload(req_id, "tag", "NULL");
            retweet_transport->close();
        }

    } catch (TException& tx) {
        cout << "ERROR: " << tx.what() << endl;
    }
    if (IF_TRACE)
        logger(req_id, "ProcessTag", "process_tag", "end");

}

class ProcessTagCloneFactory: virtual public  ProcessTagIfFactory {
public:
    virtual ~ProcessTagCloneFactory() {}
    virtual ProcessTagIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo)
    {
        boost::shared_ptr<TSocket> sock = boost::dynamic_pointer_cast<TSocket>(connInfo.transport);
        return new ProcessTagHandler;
    }
    virtual void releaseHandler(ProcessTagIf* handler) {
        delete handler;
    }
};

int main() {
//    boost::shared_ptr<TProcessor> processor(new ProcessTagProcessor(boost::make_shared<ProcessTagHandler>()));
//    boost::shared_ptr<TProtocolFactory> protocol_factory(new TBinaryProtocolFactory());
//    boost::shared_ptr<ThreadManager> thread_manager = ThreadManager::newSimpleThreadManager(4);
//    boost::shared_ptr<PosixThreadFactory> thread_factory = boost::shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
//    thread_manager->threadFactory(thread_factory);
//    thread_manager->start();
//    TNonblockingServer server(processor, protocol_factory, 9095, thread_manager);

    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    TSimpleServer server(
        boost::make_shared<ProcessTagProcessor>(boost::make_shared<ProcessTagHandler>()),
        boost::make_shared<TServerSocket>(9095),
        boost::make_shared<TBufferedTransportFactory>(),
        boost::make_shared<TBinaryProtocolFactory>());

    // const int workerCount = 1;
  
    // boost::shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(workerCount);
    // threadManager->threadFactory(boost::make_shared<PlatformThreadFactory>());
    // threadManager->start();

    // TThreadPoolServer server(
    //         boost::make_shared<ProcessTagProcessorFactory>(boost::make_shared<ProcessTagCloneFactory>()),
    //         boost::make_shared<TServerSocket>(9095),
    //         boost::make_shared<TBufferedTransportFactory>(),
    //         boost::make_shared<TBinaryProtocolFactory>(),
    //         threadManager
    //         );

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}
