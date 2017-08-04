//
// Created by Yu Gan on 8/3/17.
//

#include "netflix_microservices.h"
#include "libmemcached/memcached.h"
#include <libmongoc-1.0/mongoc.h>
#include "../gen-cpp/WriteMovieDB.h"

#define NUM_MOVIES 5
#define MONGO_PORT_START 32000
#define MMC_PORT_START 32005
#define IP_ADDR "192.168.99.100"

#define SERVER_PORT_START 10010

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

class WriteMovieDBHandler: public WriteMovieDBIf {
public:
    WriteMovieDBHandler();
    ~WriteMovieDBHandler();
    void ping() { cout << "ping(from server)" << endl; }
    void write_movie_db(const string &, const string &, const string &, const string &, const string &);

private:
    mongoc_client_t *mongo_client[NUM_MOVIES];
    mongoc_collection_t *collection[NUM_MOVIES];
    memcached_st *mmc[NUM_MOVIES];

};

WriteMovieDBHandler::WriteMovieDBHandler() {
    string mmc_configs;
    for (int i = 0; i< NUM_MOVIES; i++) {
        mongo_client[i] = mongoc_client_new (("mongodb://" + to_string(IP_ADDR) + ":" + to_string(MONGO_PORT_START + i) +
                                              "/?appname=movie_db").c_str());
        assert(mongo_client[i]);
        collection[i] =
                mongoc_client_get_collection (mongo_client[i], ("@user_" + to_string(i)).c_str(), "movie_db");
        assert(collection[i]);
        mmc_configs = "--SERVER=" + to_string(IP_ADDR) + ":" + to_string(MMC_PORT_START + i);
        mmc[i] = memcached(mmc_configs.c_str(), mmc_configs.length());
        assert(mmc[i]);
        memcached_behavior_set(mmc[i], MEMCACHED_BEHAVIOR_NO_BLOCK, 1);
        memcached_behavior_set(mmc[i], MEMCACHED_BEHAVIOR_TCP_NODELAY, 1);
        memcached_behavior_set(mmc[i], MEMCACHED_BEHAVIOR_NOREPLY, 1);
        memcached_behavior_set(mmc[i], MEMCACHED_BEHAVIOR_TCP_KEEPALIVE, 1);


    }
}

WriteMovieDBHandler::~WriteMovieDBHandler() {
    for (int i = 0; i< NUM_MOVIES; i++) {
        mongoc_client_destroy (mongo_client[i]);
        mongoc_collection_destroy (collection[i]);
        memcached_free(mmc[i]);
    }
}

void WriteMovieDBHandler::write_movie_db(const string &req_id, const string &movie_id, const string &user_id,
                                         const string &unique_id, const string & rating) {
    if (IF_TRACE)
        logger(req_id, "WriteMovieDB", "write_movie_db", "begin");


// mmc key is movie_id

    string str_match = "movie_";
    int index = stoi(movie_id.substr(str_match.length(), string::npos));
    string mmc_value;
    mmc_value = user_id + " " + unique_id + "\n";

    memcached_return_t mmc_rc;


    string mmc_key = "reviews";
    if (memcached_exist(mmc[index], mmc_key.c_str(), mmc_key.length())== MEMCACHED_SUCCESS) {
        memcached_append(mmc[index], mmc_key.c_str(), mmc_key.length(), mmc_value.c_str(),
                                  mmc_value.length(), (time_t) 0, (uint32_t) 0);
//        assert(mmc_rc == MEMCACHED_SUCCESS);
    }
    else {
        memcached_set(mmc[index], mmc_key.c_str(), mmc_key.length(), mmc_value.c_str(), mmc_value.length(),
                      (time_t) 0, (uint32_t) 0);
//        assert(mmc_rc == MEMCACHED_SUCCESS);
    }

    mmc_key = "new_ratings";
    mmc_value = user_id + " " + rating + "\n";
    if (memcached_exist(mmc[index], mmc_key.c_str(), mmc_key.length())== MEMCACHED_SUCCESS) {
        memcached_append(mmc[index], mmc_key.c_str(), mmc_key.length(), mmc_value.c_str(),
                         mmc_value.length(), (time_t) 0, (uint32_t) 0);
//        assert(mmc_rc == MEMCACHED_SUCCESS);
    }
    else {
        memcached_set(mmc[index], mmc_key.c_str(), mmc_key.length(), mmc_value.c_str(), mmc_value.length(),
                      (time_t) 0, (uint32_t) 0);
//        assert(mmc_rc == MEMCACHED_SUCCESS);
    }


    bson_t *document = bson_new ();
    bson_error_t bson_error;

    BSON_APPEND_UTF8(document, "user_id", user_id.c_str());
    BSON_APPEND_UTF8(document, "unique_id", unique_id.c_str());
    bool rc = mongoc_collection_insert(collection[index], MONGOC_INSERT_NONE, document, NULL, &bson_error);
    assert(rc);

    bson_destroy(document);

    if (IF_TRACE)
        logger(req_id, "WriteMovieDB", "write_movie_db", "end");
}


int main (int argc, char *argv[]) {
    IF_TRACE = true;
    LOG_PATH = LOG_PATH += "WriteMovieDB" + to_string(stoi(argv[1])) + ".log";

    TSimpleServer server(
            boost::make_shared<WriteMovieDBProcessor>(boost::make_shared<WriteMovieDBHandler>()),
            boost::make_shared<TServerSocket>(stoi(argv[1]) + SERVER_PORT_START),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());
    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}

