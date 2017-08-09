//
// Created by Yu Gan on 8/3/17.
//

#include "netflix_microservices.h"
#include "libmemcached/memcached.h"
#include <libmongoc-1.0/mongoc.h>
#include "../gen-cpp/MovieReviewDB.h"
#include <sstream>

#define NUM_MOVIES 5
#define MONGO_PORT_START 32000
#define MMC_PORT_START 32005


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

class MovieReviewDBHandler: public MovieReviewDBIf {
public:
    MovieReviewDBHandler();
    ~MovieReviewDBHandler();
    void ping() { cout << "ping(from server)" << endl; }
    void write_movie_review(const std::string& req_id, const std::string& movie_id, const std::string& user_id,
                                 const std::string& unique_id, const std::string& rating);
    void get_movie_review(std::string& _return, const std::string& req_id, const std::string& movie_id,
                          const int32_t begin_no, const int32_t num);

private:
    mongoc_client_t *mongo_client[NUM_MOVIES];
    mongoc_collection_t *collection[NUM_MOVIES];
    memcached_st *mmc[NUM_MOVIES];

};

MovieReviewDBHandler::MovieReviewDBHandler() {
    string mmc_configs;
    for (int i = 0; i< NUM_MOVIES; i++) {
        mongo_client[i] = mongoc_client_new (("mongodb://" + to_string(DOCKER_IP_ADDR) + ":" + to_string(MONGO_PORT_START + i) +
                                              "/?appname=movie_db").c_str());
        assert(mongo_client[i]);
        collection[i] =
                mongoc_client_get_collection (mongo_client[i], ("movie_" + to_string(i)).c_str(), "movie_db");
        assert(collection[i]);
        mmc_configs = "--SERVER=" + to_string(DOCKER_IP_ADDR) + ":" + to_string(MMC_PORT_START + i);
        mmc[i] = memcached(mmc_configs.c_str(), mmc_configs.length());
        assert(mmc[i]);
        memcached_behavior_set(mmc[i], MEMCACHED_BEHAVIOR_NO_BLOCK, 1);
        memcached_behavior_set(mmc[i], MEMCACHED_BEHAVIOR_TCP_NODELAY, 1);
//        memcached_behavior_set(mmc[i], MEMCACHED_BEHAVIOR_NOREPLY, 1);
        memcached_behavior_set(mmc[i], MEMCACHED_BEHAVIOR_TCP_KEEPALIVE, 1);


    }
}

MovieReviewDBHandler::~MovieReviewDBHandler() {
    for (int i = 0; i< NUM_MOVIES; i++) {
        mongoc_client_destroy (mongo_client[i]);
        mongoc_collection_destroy (collection[i]);
        memcached_free(mmc[i]);
    }
}

void MovieReviewDBHandler::write_movie_review(const string &req_id, const string &movie_id, const string &user_id,
                                         const string &unique_id, const string & rating) {
    if (IF_TRACE)
        logger(req_id, "MovieReviewDB", "write_movie_review", "begin");


// mmc key is movie_id

    string str_match = "movie_";
    int index = stoi(movie_id.substr(str_match.length(), string::npos));
    string mmc_value;
    mmc_value = user_id + " " + unique_id + " " + rating + "\n";

//    memcached_return_t mmc_rc;


    string mmc_key = "review";
    if (memcached_exist(mmc[index], mmc_key.c_str(), mmc_key.length())== MEMCACHED_SUCCESS) {
        memcached_prepend(mmc[index], mmc_key.c_str(), mmc_key.length(), mmc_value.c_str(),
                                  mmc_value.length(), (time_t) 0, (uint32_t) 0);
//        assert(mmc_rc == MEMCACHED_SUCCESS);
    }
    else {
        memcached_set(mmc[index], mmc_key.c_str(), mmc_key.length(), mmc_value.c_str(), mmc_value.length(),
                      (time_t) 0, (uint32_t) 0);
//        assert(mmc_rc == MEMCACHED_SUCCESS);
    }

    mmc_key = "new_rating";
    mmc_value = user_id + " " + rating + "\n";
    if (memcached_exist(mmc[index], mmc_key.c_str(), mmc_key.length())== MEMCACHED_SUCCESS) {
        memcached_prepend(mmc[index], mmc_key.c_str(), mmc_key.length(), mmc_value.c_str(),
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

    BSON_APPEND_UTF8(document, "type", "review");
    BSON_APPEND_UTF8(document, "user_id", user_id.c_str());
    BSON_APPEND_UTF8(document, "unique_id", unique_id.c_str());
    BSON_APPEND_UTF8(document, "rating", rating.c_str());
    bool rc = mongoc_collection_insert(collection[index], MONGOC_INSERT_NONE, document, NULL, &bson_error);
    assert(rc);

    bson_destroy(document);

    if (IF_TRACE)
        logger(req_id, "MovieReviewDB", "write_movie_review", "end");
}

void MovieReviewDBHandler::get_movie_review(std::string& _return, const std::string& req_id, const std::string& movie_id,
                                            const int32_t begin_no, const int32_t num) {
    if (IF_TRACE)
        logger(req_id, "MovieReviewDB", "get_movie_review",  "begin");

    string str_match = "movie_";
    int index = stoi(movie_id.substr(str_match.length(), string::npos));
    memcached_return_t mmc_rc;
    uint32_t mmc_flags;
    size_t mmc_value_length;
    char * value_char;
    value_char = memcached_get(mmc[index], "review", strlen("review"), &mmc_value_length, &mmc_flags, &mmc_rc);
    if (value_char){
        istringstream ss(value_char);
        string line;
        for (int i = 0; i < begin_no + num; i++) {
            getline(ss, line);
            if (i >= begin_no) {
                _return += line + "\n";

            }
        }
    } else {
        bson_t *query;
        query = bson_new();
        mongoc_cursor_t *cursor;
        const bson_t *doc;
        json doc_json;
        BSON_APPEND_UTF8(query, "type", "review");
        cursor = mongoc_collection_find_with_opts (collection[index], query, NULL, NULL);
        for (int i = 0; i < begin_no + num; i++) {
            if (mongoc_cursor_next(cursor, &doc)) {
                if (i >= begin_no) {
                    doc_json = json::parse(bson_as_json (doc, NULL));
                    string user_id = doc_json["user_id"];
                    string unique_id = doc_json["unique_id"];
                    string rating = doc_json["rating"];

                    _return += user_id + " " + unique_id + " " + rating + "\n";

                }
            }

        }
        mongoc_cursor_destroy (cursor);
        bson_destroy(query);

    }
    cout<<_return<<endl;

    if (IF_TRACE)
        logger(req_id, "MovieReviewDB", "get_movie_review",  "end");
}


int main (int argc, char *argv[]) {
    IF_TRACE = true;
    LOG_PATH = LOG_DIR_PATH + "MovieReviewDB_" + to_string(stoi(argv[1]) - 1) + ".log";

    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    TSimpleServer server(
            boost::make_shared<MovieReviewDBProcessor>(boost::make_shared<MovieReviewDBHandler>()),
            boost::make_shared<TServerSocket>(stoi(argv[1]) - 1 + SERVER_PORT_START),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());
    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}

