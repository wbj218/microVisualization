//
// Created by Yu Gan on 8/4/17.
//

#include "netflix_microservices.h"
#include "libmemcached/memcached.h"
#include <libmongoc-1.0/mongoc.h>
#include "../gen-cpp/MovieInfoStorage.h"
#include <vector>


#define MONGO_PORT 32022
#define MMC_PORT 32023


#define SERVER_PORT_START 10030

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

class MovieInfoStorageHandler: public MovieInfoStorageIf {
public:
    MovieInfoStorageHandler();
    ~MovieInfoStorageHandler();
    void ping() { cout << "ping(from server)" << endl; }
    void get_info(std::string& _return, const std::string& req_id, const std::string& movie_id, const string &type);

private:
    mongoc_client_t *mongo_client;
    
    memcached_st *mmc;
};

MovieInfoStorageHandler::MovieInfoStorageHandler() {
    string mmc_configs;
    mongo_client = mongoc_client_new (("mongodb://" + to_string(DOCKER_IP_ADDR) + ":" + to_string(MONGO_PORT) +
                                       "/?appname=movie_info").c_str());
    assert(mongo_client);
    mmc_configs = "--SERVER=" + to_string(DOCKER_IP_ADDR) + ":" + to_string(MMC_PORT);
    mmc = memcached(mmc_configs.c_str(), mmc_configs.length());
    assert(mmc);
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_NO_BLOCK, 1);
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_TCP_NODELAY, 1);
//    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_NOREPLY, 1);
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_TCP_KEEPALIVE, 1);
}

MovieInfoStorageHandler::~MovieInfoStorageHandler() {
    mongoc_client_destroy (mongo_client);
    memcached_free(mmc);
}

void MovieInfoStorageHandler::get_info(std::string& _return, const std::string& req_id, const std::string& movie_id,
                                       const string &type) {
    if (IF_TRACE)
        logger(req_id, "MovieInfoStorage", "get_" + type, "begin");

    char* mmc_value = NULL;
    size_t mmc_value_length;
    uint32_t mmc_flags;
    string mmc_key = movie_id + "_" + type;
    memcached_return_t mmc_rc;
    mmc_value = memcached_get(mmc, mmc_key.c_str(), mmc_key.length(), &mmc_value_length, &mmc_flags, &mmc_rc);

    if (mmc_value) {
        _return = mmc_value;
    } else {
        mongoc_collection_t *collection;
        collection = mongoc_client_get_collection (mongo_client, "movie_info", type.c_str());
        const bson_t *doc;
        bson_t *query;
        mongoc_cursor_t *cursor;
        query = bson_new();
        BSON_APPEND_UTF8(query, "movie_id", movie_id.c_str());
        cursor = mongoc_collection_find_with_opts (collection, query, NULL, NULL);
        json doc_json;
        if (mongoc_cursor_next(cursor, &doc)) {
            doc_json = json::parse(bson_as_json (doc, NULL));
            _return = doc_json[type];
        }
        bson_destroy(query);
        mongoc_cursor_destroy (cursor);
    }
    free(mmc_value);

    if (IF_TRACE)
        logger(req_id, "MovieInfoStorage", "get_" + type, "end");
}

int main (int argc, char *argv[]) {
    IF_TRACE = true;
    LOG_PATH = LOG_DIR_PATH +  "MovieInfoStorage_" + to_string(stoi(argv[1]) - 1) + ".log";

    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    TSimpleServer server(
            boost::make_shared<MovieInfoStorageProcessor>(boost::make_shared<MovieInfoStorageHandler>()),
            boost::make_shared<TServerSocket>(stoi(argv[1]) - 1 + SERVER_PORT_START),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());
    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}





