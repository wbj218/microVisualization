//
// Created by Yu Gan on 8/3/17.
//

#include "netflix_microservices.h"
#include "libmemcached/memcached.h"
#include <libmongoc-1.0/mongoc.h>
#include "../gen-cpp/UserReviewDB.h"

#define NUM_USERS 5
#define MONGO_PORT_START 32010
#define MMC_PORT_START 32015
#define IP_ADDR "192.168.99.100"

#define SERVER_PORT_START 10020

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

class UserReviewDBHandler: public UserReviewDBIf {
public:
    UserReviewDBHandler();
    ~UserReviewDBHandler();
    void ping() { cout << "ping(from server)" << endl; }
    void write_user_db(const string &, const string &, const string &, const string &);

private:
    mongoc_client_t *mongo_client[NUM_USERS];
    mongoc_collection_t *collection[NUM_USERS];
    memcached_st *mmc[NUM_USERS];

};

UserReviewDBHandler::UserReviewDBHandler() {
    string mmc_configs;
    for (int i = 0; i< NUM_USERS; i++) {
        this->mongo_client[i] = mongoc_client_new (("mongodb://" + to_string(IP_ADDR) + ":" + to_string(MONGO_PORT_START + i) +
                                                    "/?appname=user_db").c_str());
        assert(this->mongo_client[i]);
        this->collection[i] =
                mongoc_client_get_collection (this->mongo_client[i], ("@user_" + to_string(i)).c_str(), "user_db");
        assert(this->collection[i]);
        mmc_configs = "--SERVER=" + to_string(IP_ADDR) + ":" + to_string(MMC_PORT_START + i);
        this->mmc[i] = memcached(mmc_configs.c_str(), mmc_configs.length());
        assert(this->mmc[i]);
        memcached_behavior_set(this->mmc[i], MEMCACHED_BEHAVIOR_NO_BLOCK, 1);
        memcached_behavior_set(this->mmc[i], MEMCACHED_BEHAVIOR_TCP_NODELAY, 1);
//        memcached_behavior_set(this->mmc[i], MEMCACHED_BEHAVIOR_NOREPLY, 1);
        memcached_behavior_set(this->mmc[i], MEMCACHED_BEHAVIOR_TCP_KEEPALIVE, 1);


    }
}

UserReviewDBHandler::~UserReviewDBHandler() {
    for (int i = 0; i< NUM_USERS; i++) {
        mongoc_client_destroy (this->mongo_client[i]);
        mongoc_collection_destroy (this->collection[i]);
        memcached_free(this->mmc[i]);
    }
}

void UserReviewDBHandler::write_user_db(const string &req_id, const string &movie_id, const string &user_id,
                                         const string &unique_id) {
    if (IF_TRACE)
        logger(req_id, "UserReviewDB", "write_user_db", "begin");


// mmc key is movie_id

    string str_match = "user_";
    int index = stoi(user_id.substr(str_match.length(), string::npos));
    string mmc_value;
    mmc_value = movie_id + " " + unique_id + "\n";

    memcached_return_t mmc_rc;

    if (memcached_exist(mmc[index], movie_id.c_str(), movie_id.length())== MEMCACHED_SUCCESS) {
        memcached_prepend(mmc[index], movie_id.c_str(), movie_id.length(), mmc_value.c_str(),
                                  mmc_value.length(), (time_t) 0, (uint32_t) 0);
//        assert(mmc_rc == MEMCACHED_SUCCESS);
    }
    else {
        memcached_set(mmc[index], movie_id.c_str(), movie_id.length(), mmc_value.c_str(), mmc_value.length(),
                               (time_t) 0, (uint32_t) 0);
//        assert(mmc_rc == MEMCACHED_SUCCESS);
    }

    bson_t *document = bson_new ();
    bson_error_t bson_error;

    BSON_APPEND_UTF8(document, "movie_id", movie_id.c_str());
    BSON_APPEND_UTF8(document, "unique_id", unique_id.c_str());
    bool rc = mongoc_collection_insert(collection[index], MONGOC_INSERT_NONE, document, NULL, &bson_error);
    assert(rc);

    bson_destroy(document);

    if (IF_TRACE)
        logger(req_id, "UserReviewDB", "write_user_db", "end");
}


int main (int argc, char *argv[]) {
    IF_TRACE = true;
    LOG_PATH = LOG_PATH += "UserReviewDB" + to_string(stoi(argv[1]) - 1) + ".log";

    TSimpleServer server(
            boost::make_shared<UserReviewDBProcessor>(boost::make_shared<UserReviewDBHandler>()),
            boost::make_shared<TServerSocket>(stoi(argv[1]) - 1 + SERVER_PORT_START),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());
    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}

