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
#include <boost/algorithm/string.hpp>
#include <json.hpp>
#include <regex>


#include <iostream>
#include <string>

#include <libmongoc-1.0/mongoc.h>
#include "../gen-cpp/WriteDM.h"
#include "libmemcached/memcached.h"

#include "../gen-cpp/Generator.h"

#define NUM_USERS 5
#define MONGO_PORT_START 32810
#define MMC_PORT_START 32815
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

class WriteDMHandler: public WriteDMIf {
public:
    WriteDMHandler();

    ~WriteDMHandler();

    void ping() { cout << "ping(from server)" << endl; }

    void write_dm(const string &, const string &, const string &);

private:

    mongoc_client_t *mongo_client[NUM_USERS];
    mongoc_collection_t *collection[NUM_USERS];
    memcached_st *mmc[NUM_USERS];
    memcached_return_t mmc_rc;
};

WriteDMHandler::WriteDMHandler() {
    string mmc_configs;
    for (int i = 0; i< NUM_USERS; i++) {
        mongo_client[i] = mongoc_client_new (("mongodb://" + to_string(IP_ADDR) + ":" + to_string(MONGO_PORT_START + i) +
                                              "/?appname=dm").c_str());
        collection[i] =
                mongoc_client_get_collection (mongo_client[i], ("@user_" + to_string(i)).c_str(), "dm");
         mmc_configs = "--SERVER=" + to_string(IP_ADDR) + ":" + to_string(MMC_PORT_START + i);
        memcached_behavior_set(mmc[i], MEMCACHED_BEHAVIOR_NO_BLOCK, 1);
        memcached_behavior_set(mmc[i], MEMCACHED_BEHAVIOR_TCP_NODELAY, 1);
        memcached_behavior_set(mmc[i], MEMCACHED_BEHAVIOR_NOREPLY, 1);
        memcached_behavior_set(mmc[i], MEMCACHED_BEHAVIOR_TCP_KEEPALIVE, 1);
    }
}

WriteDMHandler::~WriteDMHandler() {
    for (int i = 0; i< NUM_USERS; i++) {
        mongoc_client_destroy (mongo_client[i]);
        mongoc_collection_destroy (collection[i]);
        memcached_free(mmc[i]);
    }
}

void WriteDMHandler::write_dm(const string &user_id, const string& unique_id, const string& req_id){
    vector<string> results;
    boost::split(results, req_id, [](char c){return c == ' ';});
    string sender_id;
    sender_id = results.front();

    smatch m;
    regex e("([0-9]+)");
    regex_search(user_id, m, e);
    int index = stoi(m.str());

    const bson_t *doc;
    bson_t *query;
    bson_t *update;
    bson_error_t bson_error;
    mongoc_cursor_t *cursor;


    string mmc_key = sender_id;
    memcached_return_t mmc_rc;
    uint32_t mmc_flags;
    size_t mmc_value_length;

    json mmc_value_json;
    string mmc_value;
    char* mmc_value_char;

    mmc_value_char = memcached_get(mmc[index], mmc_key.c_str(), mmc_key.length(), &mmc_value_length, &mmc_flags, &mmc_rc);
    if (mmc_value_char){
        mmc_value_json = json::parse(mmc_value_char);
//        cout<<mmc_value_json.dump(4);
        mmc_value_json[sender_id].insert(mmc_value_json[sender_id].begin(), unique_id);
        mmc_value = mmc_value_json.dump(4);
        memcached_set(mmc[index], mmc_key.c_str(), mmc_key.length(), mmc_value.c_str(), mmc_value.length(),
                      (time_t)0, (uint32_t)0);
        update = bson_new_from_json ((const uint8_t *)mmc_value.c_str(), -1, &bson_error);
        query = bson_new();
        BSON_APPEND_UTF8(query, "sender_id", sender_id.c_str());
//        while (mongoc_collection_update(collection[index], MONGOC_UPDATE_NONE, query, update, NULL, &bson_error)) {}
        bool rc = mongoc_collection_update(collection[index], MONGOC_UPDATE_NONE, query, update, NULL, &bson_error);
        assert (rc);
        bson_destroy(query);
        bson_destroy(update);

    }
    else {
        query = bson_new();
        BSON_APPEND_UTF8(query, "sender_id", sender_id.c_str());
        cursor = mongoc_collection_find_with_opts(collection[index], query, NULL, NULL);
        if (mongoc_cursor_next(cursor, &doc)) {
            mmc_value_json = json::parse(bson_as_json(doc, NULL));
//                cout<<mmc_value_json.dump(4);
            mmc_value_json[sender_id].insert(mmc_value_json[sender_id].begin(), unique_id);
            mmc_value = mmc_value_json.dump(4);
            memcached_set(mmc[index], mmc_key.c_str(), mmc_key.length(), mmc_value.c_str(), mmc_value.length(),
                          (time_t) 0, (uint32_t) 0);
            update = bson_new_from_json((const uint8_t *) mmc_value.c_str(), -1, &bson_error);
//            while (mongoc_collection_update(collection[index], MONGOC_UPDATE_NONE, query, update, NULL, &bson_error)) {}
            bool rc = mongoc_collection_update(collection[index], MONGOC_UPDATE_NONE, query, update, NULL, &bson_error);
            assert (rc);
            bson_destroy(query);
            bson_destroy(update);

        } else {
            mmc_value_json[sender_id] = {unique_id};
            mmc_value_json["sender_id"] = sender_id;
            mmc_value = mmc_value_json.dump(4);
            memcached_set(mmc[index], mmc_key.c_str(), mmc_key.length(), mmc_value.c_str(), mmc_value.length(),
                          (time_t) 0, (uint32_t) 0);
            update = bson_new_from_json((const uint8_t *) mmc_value.c_str(), -1, &bson_error);
//            while (mongoc_collection_insert(collection[index], MONGOC_INSERT_NONE, update, NULL, &bson_error)) {}
            bool rc = mongoc_collection_insert(collection[index], MONGOC_INSERT_NONE, update, NULL, &bson_error);
            assert (rc);

            bson_destroy(query);
            bson_destroy(update);

        }
        mongoc_cursor_destroy(cursor);
    }
    free(mmc_value_char);

//    generator_transport->open();
//    generator_client->response(req_id, "WriteTimeline");
//    generator_transport->close();


}


int main() {
//    boost::shared_ptr<TProcessor> processor(new WriteTimelineProcessor(boost::make_shared<WriteTimelineHandler>()));
//    boost::shared_ptr<TProtocolFactory> protocol_factory(new TBinaryProtocolFactory());
//    boost::shared_ptr<ThreadManager> thread_manager = ThreadManager::newSimpleThreadManager(4);
//    boost::shared_ptr<PosixThreadFactory> thread_factory = boost::shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
//    thread_manager->threadFactory(thread_factory);
//    thread_manager->start();
//    TNonblockingServer server(processor, protocol_factory, 9100, thread_manager);

    TSimpleServer server(
            boost::make_shared<WriteDMProcessor>(boost::make_shared<WriteDMHandler>()),
            boost::make_shared<TServerSocket>(9109),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}

