//
// Created by Yu Gan on 8/4/17.
//

#include "netflix_microservices.h"
#include "libmemcached/memcached.h"
#include <libmongoc-1.0/mongoc.h>
#include "../gen-cpp/ReviewStorage.h"

#define MONGO_PORT 32020
#define MMC_PORT 32021


#define SERVER_PORT_START 10000

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
    void get_review(Review& _return, const string& req_id, const string& movie_id, const string& unique_id);

private:
    mongoc_client_t *mongo_client;
    mongoc_collection_t *collection;
    memcached_st *mmc;

};

ReviewStorageHandler::ReviewStorageHandler() {
    string mmc_configs;
    mongo_client = mongoc_client_new (("mongodb://" + to_string(DOCKER_IP_ADDR) + ":" + to_string(MONGO_PORT) +
                                                "/?appname=review_storage").c_str());
    assert(mongo_client);
    mmc_configs = "--SERVER=" + to_string(DOCKER_IP_ADDR) + ":" + to_string(MMC_PORT);
    mmc = memcached(mmc_configs.c_str(), mmc_configs.length());
    assert(mmc);
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_NO_BLOCK, 1);
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_TCP_NODELAY, 1);
//    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_NOREPLY, 1);
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_TCP_KEEPALIVE, 1);
}

ReviewStorageHandler::~ReviewStorageHandler() {
    mongoc_client_destroy (mongo_client);
    memcached_free(mmc);
}

void ReviewStorageHandler::review_storage(const string &req_id, const Review &review) {
    if (IF_TRACE)
        logger(req_id, "ReviewStorage", "review_storage", "begin");


    bson_t *document = bson_new ();
    bson_error_t bson_error;
    memcached_return_t mmc_rc;

    BSON_APPEND_UTF8(document, "unique_id", review.unique_id.c_str());
    BSON_APPEND_UTF8(document, "user_id", review.user_id.c_str());
    BSON_APPEND_UTF8(document, "movie_id", review.movie_id.c_str());
    BSON_APPEND_UTF8(document, "rating", review.rating.c_str());
    BSON_APPEND_UTF8(document, "text", review.text.c_str());

    char *mmc_value = bson_as_json (document, NULL);
    mmc_rc = memcached_set(mmc, review.unique_id.c_str(), review.unique_id.length(), mmc_value, strlen(mmc_value),
                           (time_t)0, (uint32_t)0);
    assert(mmc_rc == MEMCACHED_SUCCESS);

    collection = mongoc_client_get_collection (mongo_client, "review_storage", review.movie_id.c_str());
    assert(collection);
    bool rc = mongoc_collection_insert(collection, MONGOC_INSERT_NONE, document, NULL, &bson_error);
    assert(rc);
    bson_destroy(document);
    mongoc_collection_destroy (collection);

    if (IF_TRACE)
        logger(req_id, "ReviewStorage", "review_storage", "end");

}

void ReviewStorageHandler::get_review(Review& _return, const string& req_id, const string& movie_id,
                                      const string& unique_id) {
    if (IF_TRACE)
        logger(req_id, "ReviewStorage", "get_review", "begin");
    char * mmc_value_char;
    memcached_return_t mmc_rc;
    uint32_t mmc_flags;
    size_t mmc_value_length;
    mmc_value_char = memcached_get(mmc, unique_id.c_str(), unique_id.length(), &mmc_value_length, &mmc_flags, &mmc_rc);
    if (mmc_value_char) {
        json mmc_value_json = json::parse(mmc_value_char);
        _return.rating = mmc_value_json["rating"];
        _return.unique_id = mmc_value_json["unique_id"];
        _return.movie_id = mmc_value_json["movie_id"];
        _return.user_id = mmc_value_json["user_id"];
        _return.text = mmc_value_json["text"];
        _return.req_id = req_id;
    } else {
        collection = mongoc_client_get_collection (mongo_client, "review_storage", movie_id.c_str());
        assert(collection);
        bson_t *query;
        query = bson_new();
        mongoc_cursor_t *cursor;
        const bson_t *doc;
        json doc_json;
        BSON_APPEND_UTF8(query, "unique_id", unique_id.c_str());
        cursor = mongoc_collection_find_with_opts (collection, query, NULL, NULL);
        if (mongoc_cursor_next(cursor, &doc)) {
            doc_json = json::parse(bson_as_json (doc, NULL));
            _return.rating = doc_json["rating"];
            _return.unique_id = doc_json["unique_id"];
            _return.movie_id = doc_json["movie_id"];
            _return.user_id = doc_json["user_id"];
            _return.text = doc_json["text"];
            _return.req_id = req_id;
        }
    }


    if (IF_TRACE)
        logger(req_id, "ReviewStorage", "get_review", "end");
}

int main (int argc, char *argv[]) {
    IF_TRACE = true;
    LOG_PATH = LOG_DIR_PATH + "ReviewStorage_" + to_string(stoi(argv[1]) - 1) + ".log";

    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    TSimpleServer server(
            boost::make_shared<ReviewStorageProcessor>(boost::make_shared<ReviewStorageHandler>()),
            boost::make_shared<TServerSocket>(stoi(argv[1]) - 1 + SERVER_PORT_START),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());
    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}