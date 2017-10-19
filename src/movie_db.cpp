//
// Created by Yu Gan on 8/3/17.
//

#include "utils.h"


using namespace NetflixMicroservices;

json logs;

bool IF_TRACE;
string LOG_PATH;

std::mutex log_lock;

ServerInfo movie_db_server;
ServerInfo docker_mongo_movie_db;
ServerInfo docker_mmc_movie_db;
int num_movie;


void exit_handler(int sig) {
    ofstream log_file;
    log_file.open(LOG_PATH);
    log_file << logs;
    log_file.close();
}

class MovieReviewDBHandler: public MovieReviewDBIf {
public:
    MovieReviewDBHandler();
    ~MovieReviewDBHandler() override;
    void ping() override { cout << "ping(from server)" << endl; }
    void write_movie_review(const std::string& req_id, const std::string& movie_id, const std::string& user_id,
                                 const std::string& unique_id, const std::string& rating) override;
    void get_movie_review(std::string& _return, const std::string& req_id, const std::string& movie_id,
                          int32_t begin_no, int32_t num) override;

private:
    mongoc_client_t **mongo_client;
    mongoc_collection_t **collection;
    memcached_st **mmc;

};

MovieReviewDBHandler::MovieReviewDBHandler() {

    mongo_client = new mongoc_client_t* [num_movie];
    collection = new mongoc_collection_t* [num_movie];
    mmc = new memcached_st* [num_movie];

    string mmc_configs;
    for (int i = 0; i< num_movie; i++) {
        mongo_client[i] = mongoc_client_new (("mongodb://" + docker_mongo_movie_db.address + ":"
                                              + to_string(docker_mongo_movie_db.port + i)
                                              + "/?appname=movie_db").c_str());
        assert(mongo_client[i]);
        collection[i] =
                mongoc_client_get_collection (mongo_client[i], ("movie_" + to_string(i)).c_str(), "movie_db");
        assert(collection[i]);
        mmc_configs = "--SERVER=" + docker_mmc_movie_db.address + ":" + to_string(docker_mmc_movie_db.port + i);
        mmc[i] = memcached(mmc_configs.c_str(), mmc_configs.length());
        assert(mmc[i]);
        memcached_behavior_set(mmc[i], MEMCACHED_BEHAVIOR_NO_BLOCK, 1);
        memcached_behavior_set(mmc[i], MEMCACHED_BEHAVIOR_TCP_NODELAY, 1);
//        memcached_behavior_set(mmc[i], MEMCACHED_BEHAVIOR_NOREPLY, 1);
        memcached_behavior_set(mmc[i], MEMCACHED_BEHAVIOR_TCP_KEEPALIVE, 1);
        memcached_behavior_set(mmc[i], MEMCACHED_BEHAVIOR_BINARY_PROTOCOL, 1);

    }
}

MovieReviewDBHandler::~MovieReviewDBHandler() {
    for (int i = 0; i< num_movie; i++) {
        mongoc_client_destroy (mongo_client[i]);
        mongoc_collection_destroy (collection[i]);
        memcached_free(mmc[i]);
    }
    delete[] mongo_client;
    delete[] collection;
    delete[] mmc;

}

void MovieReviewDBHandler::write_movie_review(const string &req_id, const string &movie_id, const string &user_id,
                                         const string &unique_id, const string & rating) {
    if (IF_TRACE)
        logger(req_id, "MovieReviewDB", "write_movie_review", "begin", logs, log_lock);


    // mmc key is movie_id
    string str_match = "movie_";
    int index = stoi(movie_id.substr(str_match.length(), string::npos));
    string mmc_value;
    mmc_value = user_id + " " + unique_id + " " + rating + "\n";

    memcached_return_t mmc_rc;
    string mmc_key = "review";
    if (IF_TRACE)
        logger(req_id, "MovieReviewDB", "write_movie_db_memcached_set", "begin", logs, log_lock);
    
    if (memcached_exist(mmc[index], mmc_key.c_str(), mmc_key.length())== MEMCACHED_SUCCESS) {
        mmc_rc =memcached_prepend(mmc[index], mmc_key.c_str(), mmc_key.length(), mmc_value.c_str(),
                                  mmc_value.length(), (time_t) 0, (uint32_t) 0);
        assert(mmc_rc == MEMCACHED_SUCCESS);
    }
    else {
        mmc_rc = memcached_set(mmc[index], mmc_key.c_str(), mmc_key.length(), mmc_value.c_str(), mmc_value.length(),
                      (time_t) 0, (uint32_t) 0);
        assert(mmc_rc == MEMCACHED_SUCCESS);
    }
    if (IF_TRACE)
        logger(req_id, "MovieReviewDB", "write_movie_db_memcached_set", "end", logs, log_lock);


//     mmc_key = "new_rating";
//     mmc_value = user_id + " " + rating + "\n";
//     if (memcached_exist(mmc[index], mmc_key.c_str(), mmc_key.length())== MEMCACHED_SUCCESS) {
//         memcached_prepend(mmc[index], mmc_key.c_str(), mmc_key.length(), mmc_value.c_str(),
//                          mmc_value.length(), (time_t) 0, (uint32_t) 0);
// //        assert(mmc_rc == MEMCACHED_SUCCESS);
//     }
//     else {
//         memcached_set(mmc[index], mmc_key.c_str(), mmc_key.length(), mmc_value.c_str(), mmc_value.length(),
//                       (time_t) 0, (uint32_t) 0);
// //        assert(mmc_rc == MEMCACHED_SUCCESS);
//     }


    bson_t *document = bson_new ();
    bson_error_t bson_error;

    BSON_APPEND_UTF8(document, "type", "review");
    BSON_APPEND_UTF8(document, "user_id", user_id.c_str());
    BSON_APPEND_UTF8(document, "unique_id", unique_id.c_str());
     BSON_APPEND_UTF8(document, "rating", rating.c_str());

    if (IF_TRACE)
        logger(req_id, "MovieReviewDB", "write_movie_db_mongodb_insert", "begin", logs, log_lock);
    bool rc = mongoc_collection_insert(collection[index], MONGOC_INSERT_NONE, document, nullptr, &bson_error);
    if (IF_TRACE)
        logger(req_id, "MovieReviewDB", "write_movie_db_mongodb_insert", "end", logs, log_lock);        
    assert(rc);
    bson_destroy(document);
    if (IF_TRACE)
        logger(req_id, "MovieReviewDB", "write_movie_review", "end", logs, log_lock);
}

void MovieReviewDBHandler::get_movie_review(std::string& _return, const std::string& req_id, const std::string& movie_id,
                                            const int32_t begin_no, const int32_t num) {
    if (IF_TRACE)
        logger(req_id, "MovieReviewDB", "get_movie_review",  "begin", logs, log_lock);

    string str_match = "movie_";
    int index = stoi(movie_id.substr(str_match.length(), string::npos));
    memcached_return_t mmc_rc;
    uint32_t mmc_flags;
    size_t mmc_value_length;
    char * value_char;
    if (IF_TRACE)
        logger(req_id, "MovieReviewDB", "get_movie_db_memcached_get", "begin", logs, log_lock);
    value_char = memcached_get(mmc[index], "review", strlen("review"), &mmc_value_length, &mmc_flags, &mmc_rc);
    if (IF_TRACE)
        logger(req_id, "MovieReviewDB", "get_movie_db_memcached_get", "end", logs, log_lock);
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
        cursor = mongoc_collection_find_with_opts (collection[index], query, nullptr, nullptr);
        for (int i = 0; i < begin_no + num; i++) {
            if (mongoc_cursor_next(cursor, &doc)) {
                if (i >= begin_no) {
                    doc_json = json::parse(bson_as_json (doc, nullptr));
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
    // cout<<_return<<endl;

    if (IF_TRACE)
        logger(req_id, "MovieReviewDB", "get_movie_review",  "end", logs, log_lock);
}

class MovieReviewDBCloneFactory: virtual public MovieReviewDBIfFactory {
public:
    virtual ~MovieReviewDBCloneFactory() {}
    virtual MovieReviewDBIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) {
        boost::shared_ptr<TSocket> sock = boost::dynamic_pointer_cast<TSocket>(connInfo.transport);
        return new MovieReviewDBHandler();
    }
    virtual void releaseHandler(MovieReviewDBIf* handler) {
        delete handler;
    }


};


int main (int argc, char *argv[]) {
    IF_TRACE = true;
    LOG_PATH = LOG_DIR_PATH + "MovieReviewDB_" + to_string(stoi(argv[1]) - 1) + ".log";

    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    json config;
    config = load_config_file(CONFIG_FILE);
    movie_db_server = load_server_config("movie_db_server", config);
    docker_mongo_movie_db = load_server_config("docker_mongo_movie_db", config);
    docker_mmc_movie_db = load_server_config("docker_mmc_movie_db", config);
    num_movie = load_num_movie(config);


    TThreadedServer server(
            boost::make_shared<MovieReviewDBProcessorFactory>(boost::make_shared<MovieReviewDBCloneFactory>()),
            boost::make_shared<TServerSocket>(stoi(argv[1]) + movie_db_server.port),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());

    // TSimpleServer server(
    //         boost::make_shared<MovieReviewDBProcessor>(boost::make_shared<MovieReviewDBHandler>()),
    //         boost::make_shared<TServerSocket>(stoi(argv[1]) - 1 + MOVIE_DB_PORT_START),
    //         boost::make_shared<TBufferedTransportFactory>(),
    //         boost::make_shared<TBinaryProtocolFactory>());
    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}

