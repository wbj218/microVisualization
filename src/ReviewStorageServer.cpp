//
// Created by Yu Gan on 8/4/17.
//

#include "netflix_microservices.h"
#include "libmemcached/memcached.h"
#include <libmongoc-1.0/mongoc.h>
#include "../gen-cpp/ReviewStorage.h"

#define MONGO_PORT_START 32020
#define MMC_PORT_START 32021
#define IP_ADDR "192.168.99.100"

#define SERVER_PORT_START 1000

using namespace NetflixMicroservices;

json logs;
bool IF_TRACE;
string LOG_PATH;

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

class ReviewStorageHandler: public ReviewStorageIf {
public:
    ReviewStorageHandler();
    ~ReviewStorageHandler();
    void ping() { cout << "ping(from server)" << endl; }
    void review_storage(const string &req_id, const Review &review);

private:
    mongoc_client_t *mongo_client;
    mongoc_collection_t *collection;
    memcached_st *mmc;

};

ReviewStorageHandler::ReviewStorageHandler() {
    string mmc_configs;
    mongo_client = mongoc_client_new (("mongodb://" + to_string(IP_ADDR) + ":" + to_string(MONGO_PORT_START) +
                                                "/?appname=review_storage").c_str());
    assert(mongo_client);
    collection =
            mongoc_client_get_collection (mongo_client, "review_storage", "db");
    assert(collection);
    mmc_configs = "--SERVER=" + to_string(IP_ADDR) + ":" + to_string(MMC_PORT_START );
    mmc = memcached(mmc_configs.c_str(), mmc_configs.length());
    assert(mmc);
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_NO_BLOCK, 1);
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_TCP_NODELAY, 1);
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_NOREPLY, 1);
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_TCP_KEEPALIVE, 1);
}

ReviewStorageHandler::~ReviewStorageHandler() {
    mongoc_client_destroy (mongo_client);
    mongoc_collection_destroy (collection);
    memcached_free(mmc);
}

void ReviewStorageHandler::review_storage(const string &req_id, const Review &review) {
    if (IF_TRACE)
        logger(req_id, "ReviewStorage", "review_storage", "begin");



    if (IF_TRACE)
        logger(req_id, "ReviewStorage", "review_storage", "end");

}