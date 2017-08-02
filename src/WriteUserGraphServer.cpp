//
// Created by Yu Gan on 7/13/17.
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
#include <json.hpp>
#include <regex>


#include <iostream>
#include <fstream>
#include <string>
#include <libmongoc-1.0/mongoc.h>
#include "../gen-cpp/WriteUserGraph.h"
#include "libmemcached/memcached.h"
#include <json.hpp>
#include "../gen-cpp/Generator.h"

#include "json.hpp"
#include <signal.h>
#include <sys/time.h>
#include <fstream>


#define NUM_USERS 5
#define MONGO_PORT_START 32772
#define MMC_PORT_START 32793
#define IP_ADDR "128.253.128.64"
#define USER_GRAPH_PATH "../user_graph/user_graph.json"

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


class WriteUserGraphHandler: public WriteUserGraphIf {
public:
    WriteUserGraphHandler();
    ~WriteUserGraphHandler();
    void ping() { cout << "ping(from server)" << endl; }
    void write_graph(const string &, const string &, const string &);

private:
    json user_graph;
    mongoc_client_t *mongo_client[NUM_USERS];
    mongoc_collection_t *collection[NUM_USERS];
    memcached_st *mmc[NUM_USERS];
    memcached_return_t mmc_rc;

//    boost::shared_ptr<TTransport> generator_socket;
//    boost::shared_ptr<TTransport> generator_transport;
//    boost::shared_ptr<TProtocol> generator_protocol;
//    GeneratorClient *generator_client;


};

WriteUserGraphHandler::WriteUserGraphHandler() {

//    try {
//        generator_socket = (boost::shared_ptr<TTransport>) new TSocket("localhost", 9102);
//        generator_transport = (boost::shared_ptr<TTransport>) new TBufferedTransport(generator_socket);
//        generator_protocol = (boost::shared_ptr<TProtocol>) new TBinaryProtocol(generator_transport);
//        generator_client = new GeneratorClient(generator_protocol);
//    } catch (TException& tx) {
//        cout << "ERROR: " << tx.what() << endl;
//    }

    ifstream json_file;
    json_file.open(USER_GRAPH_PATH);
    json_file>>user_graph;
    json_file.close();


    string mmc_configs;
    for (int i = 0; i< NUM_USERS; i++) {
        mongo_client[i] = mongoc_client_new (("mongodb://" + to_string(IP_ADDR) + ":" + to_string(MONGO_PORT_START + i) +
                                              "/?appname=graph").c_str());
        collection[i] =
                mongoc_client_get_collection (mongo_client[i], ("@user_" + to_string(i)).c_str(), "graph");
        mmc_configs = "--SERVER=" + to_string(IP_ADDR) + ":" + to_string(MMC_PORT_START + i);
        mmc[i] = memcached(mmc_configs.c_str(), mmc_configs.length());
        memcached_behavior_set(mmc[i], MEMCACHED_BEHAVIOR_NO_BLOCK, 1);
        memcached_behavior_set(mmc[i], MEMCACHED_BEHAVIOR_TCP_NODELAY, 1);
        memcached_behavior_set(mmc[i], MEMCACHED_BEHAVIOR_NOREPLY, 1);
        memcached_behavior_set(mmc[i], MEMCACHED_BEHAVIOR_TCP_KEEPALIVE, 1);
    }

}

WriteUserGraphHandler::~WriteUserGraphHandler() {
    for (int i = 0; i< NUM_USERS; i++) {
        mongoc_client_destroy (mongo_client[i]);
        mongoc_collection_destroy (collection[i]);
        memcached_free(mmc[i]);
    }
//    delete generator_client;
}

void WriteUserGraphHandler::write_graph(const string &user_id, const string &unique_id, const string &req_id) {
    logger(req_id, "WriteGraph", "write_graph", "begin");

    for (auto const& user:user_graph[user_id]["followers"]){

        smatch m;
        regex e("([0-9]+)");
        string user_string = user.dump(4);
        regex_search(user_string, m, e);
        int index = stoi(m.str());

        const bson_t *doc;
        bson_t *query;
        bson_t *update;
        bson_error_t bson_error;
        mongoc_cursor_t *cursor;

        string mmc_key = user_id;
        memcached_return_t mmc_rc;
        uint32_t mmc_flags;
        size_t mmc_value_length;
        json mmc_value_json;
        string mmc_value;
        char* mmc_value_char;

        mmc_value_char = memcached_get(mmc[index], mmc_key.c_str(), mmc_key.length(), &mmc_value_length, &mmc_flags, &mmc_rc);
        if (mmc_value_char){
            mmc_value_json = json::parse(mmc_value_char);
//            cout<<mmc_value_json.dump(4);
            mmc_value_json["following tweet"].insert(mmc_value_json["following tweet"].begin(), unique_id);
            mmc_value = mmc_value_json.dump(4);
            memcached_set(mmc[index], mmc_key.c_str(), mmc_key.length(), mmc_value.c_str(), mmc_value.length(),
                          (time_t)0, (uint32_t)0);
            update = bson_new_from_json ((const uint8_t *)mmc_value.c_str(), -1, &bson_error);
            query = bson_new();
            BSON_APPEND_UTF8(query, "following user", user_id.c_str());
            bool rc = mongoc_collection_update(collection[index], MONGOC_UPDATE_NONE, query, update, NULL, &bson_error);
            // cout<<"rc = "<<rc<<"error = "<<bson_error.message<<endl;
            assert(rc);

            bson_destroy(query);
            bson_destroy(update);

        }
        else {
            query = bson_new();
            BSON_APPEND_UTF8(query, "following user", user_id.c_str());
            cursor = mongoc_collection_find_with_opts (collection[index], query, NULL, NULL);
            if (mongoc_cursor_next(cursor, &doc)) {
                mmc_value_json = json::parse(bson_as_json (doc, NULL));
//                cout<<mmc_value_json.dump(4);
                mmc_value_json["following tweet"].insert(mmc_value_json["following tweet"].begin(), unique_id);
                mmc_value = mmc_value_json.dump(4);
                memcached_set(mmc[index], mmc_key.c_str(), mmc_key.length(), mmc_value.c_str(), mmc_value.length(),
                              (time_t)0, (uint32_t)0);
                update = bson_new_from_json ((const uint8_t *)mmc_value.c_str(), -1, &bson_error);
                bool rc = mongoc_collection_update(collection[index], MONGOC_UPDATE_NONE, query, update, NULL, &bson_error);
                assert(rc);
                bson_destroy(query);
                bson_destroy(update);

            }
            else {
                mmc_value_json["following tweet"] = {unique_id};
                mmc_value_json["following user"] = user_id;
                mmc_value = mmc_value_json.dump(4);
                memcached_set(mmc[index], mmc_key.c_str(), mmc_key.length(), mmc_value.c_str(), mmc_value.length(),
                              (time_t)0, (uint32_t)0);
                update = bson_new_from_json ((const uint8_t *)mmc_value.c_str(), -1, &bson_error);
                bool rc = mongoc_collection_insert(collection[index], MONGOC_INSERT_NONE, update, NULL, &bson_error);
                assert(rc);
                bson_destroy(query);
                bson_destroy(update);

            }
            mongoc_cursor_destroy(cursor);
        }
        free(mmc_value_char);
    }
    logger(req_id, "WriteGraph", "write_graph", "end");

//    generator_transport->open();
//    generator_client->response(req_id, "WriteGraph");
//    generator_transport->close();

}



class WriteUserGraphCloneFactory: virtual public  WriteUserGraphIfFactory {
public:
    virtual ~WriteUserGraphCloneFactory() {}
    virtual WriteUserGraphIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo)
    {
        boost::shared_ptr<TSocket> sock = boost::dynamic_pointer_cast<TSocket>(connInfo.transport);
        return new WriteUserGraphHandler;
    }
    virtual void releaseHandler(WriteUserGraphIf* handler) {
        delete handler;
    }
};

int main(int argc, char *argv[]) {
//    boost::shared_ptr<TProcessor> processor(new WriteUserGraphProcessor(boost::make_shared<WriteUserGraphHandler>()));
//    boost::shared_ptr<TProtocolFactory> protocol_factory(new TBinaryProtocolFactory());
//    boost::shared_ptr<ThreadManager> thread_manager = ThreadManager::newSimpleThreadManager(4);
//    boost::shared_ptr<PosixThreadFactory> thread_factory = boost::shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
//    thread_manager->threadFactory(thread_factory);
//    thread_manager->start();
//    TNonblockingServer server(processor, protocol_factory, 9099, thread_manager);

    LOG_PATH += "WriteGraph_" + to_string(stoi(argv[1])) + ".log";
    mongoc_init();
    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    TSimpleServer server(
            boost::make_shared<WriteUserGraphProcessor>(boost::make_shared<WriteUserGraphHandler>()),
            boost::make_shared<TServerSocket>(stoi(argv[1])),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());

    // const int workerCount = 1;
  
    // boost::shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(workerCount);
    // threadManager->threadFactory(boost::make_shared<PlatformThreadFactory>());
    // threadManager->start();

    // TThreadPoolServer server(
    //         boost::make_shared<WriteUserGraphProcessorFactory>(boost::make_shared<WriteUserGraphCloneFactory>()),
    //         boost::make_shared<TServerSocket>(stoi(argv[1])), //port
    //         boost::make_shared<TBufferedTransportFactory>(),
    //         boost::make_shared<TBinaryProtocolFactory>(),
    //         threadManager
    //         );

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}





