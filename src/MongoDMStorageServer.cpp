//
// Created by Yu Gan on 7/12/17.
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
#include <sstream>
#include <string>
#include <libmongoc-1.0/mongoc.h>
#include "bson.h"
#include "../gen-cpp/MongoDMStorage.h"
#include "libmemcached/memcached.h"
#include "../gen-cpp/Generator.h"

#include "json.hpp"
#include <signal.h>
#include <sys/time.h>
#include <fstream>

#define LOG_PATH "../logs/DMStorage.log"
#define IF_TRACE true
#define IP_ADDR "128.253.128.64"

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

class MongoDMStorageHandler: public MongoDMStorageIf {
public:
    MongoDMStorageHandler();
    ~MongoDMStorageHandler();
    void ping() { cout << "ping(from server)" << endl; }
    void store_dm(const Tweet &tweet, const string &req_id);

private:

//    boost::shared_ptr<TTransport> generator_socket;
//    boost::shared_ptr<TTransport> generator_transport;
//    boost::shared_ptr<TProtocol> generator_protocol;
//    GeneratorClient *generator_client;

    mongoc_client_t *mongo_client;
    mongoc_gridfs_t *gridfs;
    bson_error_t error;
    mongoc_stream_t *stream;
    mongoc_gridfs_file_opt_t opt = {0};
    mongoc_gridfs_file_t *file;
    mongoc_iovec_t iov;

    memcached_st *mmc;
    memcached_return_t mmc_rc;
};

MongoDMStorageHandler::MongoDMStorageHandler() {
//    try {
//        generator_socket = (boost::shared_ptr<TTransport>) new TSocket("localhost", 9102);
//        generator_transport = (boost::shared_ptr<TTransport>) new TBufferedTransport(generator_socket);
//        generator_protocol = (boost::shared_ptr<TProtocol>) new TBinaryProtocol(generator_transport);
//        generator_client = new GeneratorClient(generator_protocol);
//    } catch (TException& tx) {
//        cout << "ERROR: " << tx.what() << endl;
//    }

    mongo_client = mongoc_client_new (("mongodb://" + to_string(IP_ADDR) + ":32820/?appname=store_tweet").c_str());
    string mmc_configs = "--SERVER=" + to_string(IP_ADDR) + ":32821";
    mmc = memcached(mmc_configs.c_str(), mmc_configs.length());
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_NO_BLOCK, 1);
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_TCP_NODELAY, 1);
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_NOREPLY, 0);
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_TCP_KEEPALIVE, 1);

}

MongoDMStorageHandler::~MongoDMStorageHandler() {
    mongoc_client_destroy (mongo_client);
    mongoc_cleanup ();
    memcached_free(mmc);


}

void MongoDMStorageHandler::store_dm(const Tweet &tweet, const string &req_id){
    if (IF_TRACE)
        logger(req_id, "DMStorage", "store_dm",  "begin");


    bson_t *doc = bson_new();
    bson_oid_t oid;
    bson_oid_init (&oid, NULL);


    mongoc_gridfs_t *gridfs =
            mongoc_client_get_gridfs (mongo_client, "dm_storage", tweet.user_id.c_str(), &error);

    // cout<<tweet.image;



//    cout<<tweet.unique_id<<endl;
    BSON_APPEND_OID (doc, "_id", &oid);
//    cout<<"_id complete"<<endl;


//    BSON_APPEND_INT64(doc, "unique_id", stoull(tweet.unique_id));
    BSON_APPEND_UTF8(doc, "unique_id", tweet.unique_id.c_str());
//    cout<<"unique_id complete"<<endl;
    BSON_APPEND_UTF8(doc, "text", tweet.text.c_str());
    BSON_APPEND_UTF8(doc, "user_id", tweet.user_id.c_str());

    if (tweet.image != "NULL")
        BSON_APPEND_BINARY(doc, "image", BSON_SUBTYPE_BINARY,  (const uint8_t*)tweet.image.c_str(), (uint32_t)tweet.image.size());
//    cout<<tweet.image.c_str();
    if (tweet.video != "NULL")
        BSON_APPEND_BINARY(doc, "video", BSON_SUBTYPE_BINARY,  (const uint8_t*)tweet.video.c_str(), (uint32_t)tweet.video.size());
    if (tweet.urls != "NULL")
        BSON_APPEND_UTF8(doc, "urls", tweet.urls.c_str());
    if (tweet.tags != "NULL")
        BSON_APPEND_UTF8(doc, "tags", tweet.tags.c_str());

    char *buf = bson_as_json (doc, NULL);
    iov.iov_base = (void *) buf;
    iov.iov_len = strlen(buf) + 1;

    opt.filename = tweet.unique_id.c_str();
    file = mongoc_gridfs_create_file(gridfs, &opt);
    mongoc_gridfs_file_writev (file, &iov, 1, -1);
    assert (file);


    mongoc_gridfs_file_save (file);
    mongoc_gridfs_file_destroy (file);


    mmc_rc = memcached_set(mmc, tweet.user_id.c_str(), tweet.user_id.length(), buf, strlen(buf),
                           (time_t)0, (uint32_t)0);
    assert(mmc_rc == MEMCACHED_SUCCESS);

//    generator_transport->open();
//    generator_client->response(tweet.req_id, "DMStorage");
//    generator_transport->close();

    bson_destroy (doc);
    mongoc_gridfs_destroy (gridfs);

    if (IF_TRACE)
        logger(req_id, "DMStorage", "store_dm",  "end");

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
            boost::make_shared<MongoDMStorageProcessor>(boost::make_shared<MongoDMStorageHandler>()),
            boost::make_shared<TServerSocket>(9106),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());
    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}








