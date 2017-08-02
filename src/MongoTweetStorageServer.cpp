//
// Created by Yu Gan on 7/12/17.
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
#include <sstream>
#include <string>
#include <libmongoc-1.0/mongoc.h>
#include "bson.h"
#include "../gen-cpp/MongoTweetStorage.h"
#include "libmemcached/memcached.h"
#include "../gen-cpp/Generator.h"

#include "json.hpp"
#include <signal.h>
#include <sys/time.h>
#include <fstream>


#define IP_ADDR "128.253.128.64"
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
string LOG_PATH = "../logs/";
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



class MongoTweetStorageHandler: public MongoTweetStorageIf {
public:
    MongoTweetStorageHandler();
    ~MongoTweetStorageHandler();
    void ping() { cout << "ping(from server)" << endl; }
    void store_tweet(const Tweet &tweet, const string &req_id);
    void get_tweet(string & _return, const string &user_id, const string &unique_id, const string &req_id);

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

    ssize_t r;

    memcached_st *mmc;
    memcached_return_t mmc_rc;
};

MongoTweetStorageHandler::MongoTweetStorageHandler() {
//    try {
//        generator_socket = (boost::shared_ptr<TTransport>) new TSocket("localhost", 9102);
//        generator_transport = (boost::shared_ptr<TTransport>) new TBufferedTransport(generator_socket);
//        generator_protocol = (boost::shared_ptr<TProtocol>) new TBinaryProtocol(generator_transport);
//        generator_client = new GeneratorClient(generator_protocol);
//    } catch (TException& tx) {
//        cout << "ERROR: " << tx.what() << endl;
//    }
    // mongoc_init ();
    mongo_client = mongoc_client_new (("mongodb://" + to_string(IP_ADDR) + ":32768/?appname=store_tweet").c_str());
    string mmc_configs = "--SERVER=" + to_string(IP_ADDR) + ":32792";
    mmc = memcached(mmc_configs.c_str(), mmc_configs.length());
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_NO_BLOCK, 1);
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_TCP_NODELAY, 1);
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_NOREPLY, 0);
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_TCP_KEEPALIVE, 1);

}

MongoTweetStorageHandler::~MongoTweetStorageHandler() {
    mongoc_client_destroy (mongo_client);
    // mongoc_cleanup ();
    memcached_free(mmc);


}

void MongoTweetStorageHandler::store_tweet(const Tweet &tweet, const string &req_id){
    if (IF_TRACE) 
        logger(req_id, "TweetStorage", "tweet_storage", "begin");

    bson_t *doc = bson_new();
    bson_oid_t oid;
    bson_oid_init (&oid, NULL);


    mongoc_gridfs_t *gridfs =
            mongoc_client_get_gridfs (mongo_client, "tweet_storage", tweet.user_id.c_str(), &error);

    // cout<<tweet.image;



//    cout<<tweet.unique_id<<endl;
    BSON_APPEND_OID (doc, "_id", &oid);
//    cout<<"_id complete"<<endl;


//    BSON_APPEND_INT64(doc, "unique_id", stoull(tweet.unique_id));
    BSON_APPEND_UTF8(doc, "unique_id", tweet.unique_id.c_str());
//    cout<<"unique_id complete"<<endl;
    BSON_APPEND_UTF8(doc, "text", tweet.text.c_str());
    BSON_APPEND_UTF8(doc, "user_id", tweet.user_id.c_str());
    BSON_APPEND_UTF8(doc, "tweet_type", tweet.tweet_type.c_str());

    
    if (tweet.image != "NULL")
        BSON_APPEND_BINARY(doc, "image", BSON_SUBTYPE_BINARY,  (const uint8_t*)tweet.image.c_str(), (uint32_t)tweet.image.size());
//    cout<<tweet.image.c_str();
    if (tweet.video != "NULL")
        BSON_APPEND_BINARY(doc, "video", BSON_SUBTYPE_BINARY,  (const uint8_t*)tweet.video.c_str(), (uint32_t)tweet.video.size());
    if (tweet.urls != "NULL")
        BSON_APPEND_UTF8(doc, "urls", tweet.urls.c_str());
    if (tweet.tags != "NULL")
        BSON_APPEND_UTF8(doc, "tags", tweet.tags.c_str());

    mongoc_iovec_t iov;
    char *buf = bson_as_json (doc, NULL);
    iov.iov_base = (void *) buf;
    iov.iov_len = strlen(buf)+1;

    opt.filename = tweet.unique_id.c_str();
    file = mongoc_gridfs_create_file(gridfs, &opt);
    mongoc_gridfs_file_writev (file, &iov, 1, -1);
    assert (file);

//    generator_transport->open();
//    generator_client->response(tweet.req_id, "TweetStorage");
//    generator_transport->close();


    mongoc_gridfs_file_save (file);
    mongoc_gridfs_file_destroy (file);
    bson_free (buf);





//    BSON_APPEND_BINARY(doc, "text", BSON_SUBTYPE_BINARY, (const uint8_t*)(tweet.text.c_str()),
//                       (uint32_t)(tweet.text.size()));
//    cout<<"text complete"<<endl;
//    BSON_APPEND_BINARY(doc, "user_id", BSON_SUBTYPE_BINARY, (const uint8_t*)(tweet.user_id.c_str()),
//                       (uint32_t)(tweet.user_id.size()));
//    cout<<"user_id complete"<<endl;
//    if (tweet.image != "NULL")
//        BSON_APPEND_BINARY(doc, "image", BSON_SUBTYPE_BINARY, (const uint8_t*)(tweet.image.c_str()),
//                       (uint32_t)(tweet.image.size()));
//    cout<<"image complete"<<endl;
//    if (tweet.video != "NULL")
//        BSON_APPEND_BINARY(doc, "video", BSON_SUBTYPE_BINARY, (const uint8_t*)(tweet.video.c_str()),
//                           (uint32_t)(tweet.video.size()));
//    cout<<"video complete"<<endl;
//    if (tweet.tags != "NULL")
//        BSON_APPEND_BINARY(doc, "tags", BSON_SUBTYPE_BINARY, (const uint8_t*)(tweet.tags.c_str()),
//                           (uint32_t)(tweet.tags.size()));
//    cout<<"tags complete"<<endl;
//    if (tweet.urls != "NULL")
//        BSON_APPEND_BINARY(doc, "urls", BSON_SUBTYPE_BINARY, (const uint8_t*)(tweet.urls.c_str()),
//                           (uint32_t)(tweet.urls.size()));
//    cout<<"urls complete"<<endl;

//    bool rc = mongoc_collection_insert(collection, MONGOC_INSERT_NONE, doc, NULL, &error);
//
//    assert(rc);



    mmc_rc = memcached_set(mmc, tweet.user_id.c_str(), tweet.user_id.length(), buf, strlen(buf),
                       (time_t)0, (uint32_t)0);
    assert(mmc_rc == MEMCACHED_SUCCESS);

    
    bson_destroy (doc);
    mongoc_gridfs_destroy (gridfs);
    if (IF_TRACE)
        logger(req_id, "TweetStorage", "tweet_storage", "end");

}


void MongoTweetStorageHandler::get_tweet(string & _return, const string &user_id, const string &unique_id, const string &req_id){
    uint32_t mmc_flags;
    size_t mmc_value_length;
    char* mmc_value_char;
    memcached_return_t mmc_rc;
    mmc_value_char = memcached_get(mmc, unique_id.c_str(), unique_id.length(), &mmc_value_length, &mmc_flags, &mmc_rc);
    if (mmc_value_char) {
        cout<<"cached"<<endl;
        _return = to_string(mmc_value_char);
        free(mmc_value_char);    
    }
        
    else {
        bson_t *doc = bson_new();
        mongoc_gridfs_t *gridfs =
            mongoc_client_get_gridfs (mongo_client, "tweet_storage", user_id.c_str(), &error);
        file = mongoc_gridfs_find_one_by_filename (gridfs, unique_id.c_str(), &error);
        assert (file);

        stream = mongoc_stream_gridfs_new (file);
        assert (stream);

        mongoc_iovec_t iov;
        char buf[8192];
        iov.iov_base = (void *) buf;
        iov.iov_len = sizeof buf;

        for (;;) {
            r = mongoc_stream_readv (stream, &iov, 1, -1, 0);

            assert (r >= 0);

            if (r == 0) {
                break;
            }


            _return += to_string(buf);
        }
    }
        
    

}

class MongoTweetStorageCloneFactory: virtual public MongoTweetStorageIfFactory {
public:
    virtual ~MongoTweetStorageCloneFactory() {}
    virtual MongoTweetStorageIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo)
    {
        boost::shared_ptr<TSocket> sock = boost::dynamic_pointer_cast<TSocket>(connInfo.transport);
        return new MongoTweetStorageHandler;
    }
    virtual void releaseHandler(MongoTweetStorageIf* handler) {
        delete handler;
    }
};


int main(int argc, char *argv[]) {
//    boost::shared_ptr<TProcessor> processor(new MongoTweetStorageProcessor(boost::make_shared<MongoTweetStorageHandler>()));
//    boost::shared_ptr<TProtocolFactory> protocol_factory(new TBinaryProtocolFactory());
//    boost::shared_ptr<ThreadManager> thread_manager = ThreadManager::newSimpleThreadManager(4);
//    boost::shared_ptr<PosixThreadFactory> thread_factory = boost::shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
//    thread_manager->threadFactory(thread_factory);
//    thread_manager->start();
//    TNonblockingServer server(processor, protocol_factory, 9098, thread_manager);
    LOG_PATH += "TweetStorage_" + to_string(stoi(argv[1])) + ".log";
    mongoc_init();
    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);
    
    TSimpleServer server(
            boost::make_shared<MongoTweetStorageProcessor>(boost::make_shared<MongoTweetStorageHandler>()),
            boost::make_shared<TServerSocket>(stoi(argv[1])),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());

    // const int workerCount = 1;
  
    // boost::shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(workerCount);
    // threadManager->threadFactory(boost::make_shared<PlatformThreadFactory>());
    // threadManager->start();

    // TThreadPoolServer server(
    //     boost::make_shared<MongoTweetStorageProcessorFactory>(boost::make_shared<MongoTweetStorageCloneFactory>()),
    //     boost::make_shared<TServerSocket>(stoi(argv[1])), //port
    //     boost::make_shared<TBufferedTransportFactory>(),
    //     boost::make_shared<TBinaryProtocolFactory>(),
    //     threadManager);


    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}








