//
// Created by Yu Gan on 8/3/17.
//

#include "utils.h"


using namespace NetflixMicroservices;

json logs;

bool IF_TRACE;
string LOG_PATH;

std::mutex log_lock;

ServerInfo user_db_server;
ServerInfo docker_mongo_user_db;
ServerInfo docker_mmc_user_db;
int num_user;

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
    void write_user_review(const string &, const string &, const string &, const string &);

private:
    mongoc_client_t **mongo_client;
    mongoc_collection_t **collection;
    memcached_st **mmc;

};

UserReviewDBHandler::UserReviewDBHandler() {

    mongo_client = new mongoc_client_t* [num_user];
    collection = new mongoc_collection_t* [num_user];
    mmc = new memcached_st* [num_user];

    string mmc_configs;
    for (int i = 0; i< num_user; i++) {
        mongo_client[i] = mongoc_client_new (("mongodb://" + docker_mongo_user_db.address + ":"
                                              + to_string(docker_mongo_user_db.port + i)
                                              + "/?appname=user_db").c_str());
        assert(mongo_client[i]);
        collection[i] =
                mongoc_client_get_collection (mongo_client[i], ("user_" + to_string(i)).c_str(), "user_db");
        assert(collection[i]);
        mmc_configs = "--SERVER=" + docker_mmc_user_db.address + ":" + to_string(docker_mmc_user_db.port + i);
        mmc[i] = memcached(mmc_configs.c_str(), mmc_configs.length());
        assert(mmc[i]);
        memcached_behavior_set(mmc[i], MEMCACHED_BEHAVIOR_NO_BLOCK, 1);
        memcached_behavior_set(mmc[i], MEMCACHED_BEHAVIOR_TCP_NODELAY, 1);
//        memcached_behavior_set(this->mmc[i], MEMCACHED_BEHAVIOR_NOREPLY, 1);
        memcached_behavior_set(mmc[i], MEMCACHED_BEHAVIOR_TCP_KEEPALIVE, 1);
        memcached_behavior_set(mmc[i], MEMCACHED_BEHAVIOR_BINARY_PROTOCOL, 1);
    }
}

UserReviewDBHandler::~UserReviewDBHandler() {
    for (int i = 0; i< num_user; i++) {
        mongoc_client_destroy (mongo_client[i]);
        mongoc_collection_destroy (collection[i]);
        memcached_free(mmc[i]);
    }
    delete[] mongo_client;
    delete[] collection;
    delete[] mmc;
}

void UserReviewDBHandler::write_user_review(const string &req_id, const string &movie_id, const string &user_id, const string &unique_id) {

    
    if (IF_TRACE)
        logger(req_id, "UserReviewDB", "write_user_db", "begin", logs, log_lock);


// mmc key is movie_id
                                            
    string str_match = "user_";
    int index = stoi(user_id.substr(str_match.length(), string::npos));
    string mmc_value;
    mmc_value = movie_id + " " + unique_id + "\n";

    // memcached_return_t mmc_rc;
    string mmc_key = "review";
    if (IF_TRACE)
        logger(req_id, "UserReviewDB", "write_user_db_memcached_set", "begin", logs, log_lock);
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
    if (IF_TRACE)
        logger(req_id, "UserReviewDB", "write_user_db_memcached_set", "end", logs, log_lock);

    bson_t *document = bson_new ();
    bson_error_t bson_error;

    BSON_APPEND_UTF8(document, "type", "review");
    BSON_APPEND_UTF8(document, "movie_id", movie_id.c_str());
    BSON_APPEND_UTF8(document, "unique_id", unique_id.c_str());

    if (IF_TRACE)
        logger(req_id, "UserReviewDB", "write_user_db_mongodb_insert", "begin", logs, log_lock);
    bool rc = mongoc_collection_insert(collection[index], MONGOC_INSERT_NONE, document, nullptr, &bson_error);
    if (IF_TRACE)
        logger(req_id, "UserReviewDB", "write_user_db_mongodb_insert", "end", logs, log_lock);
    assert(rc);

    bson_destroy(document);

    if (IF_TRACE)
        logger(req_id, "UserReviewDB", "write_user_db", "end", logs, log_lock);
}


int main (int argc, char *argv[]) {
    IF_TRACE = true;
    LOG_PATH = LOG_DIR_PATH +  "UserReviewDB_" + to_string(stoi(argv[1])) + ".log";

    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    json config;
    config = load_config_file(CONFIG_FILE);
    user_db_server = load_server_config("user_db_server", config);
    docker_mongo_user_db = load_server_config("docker_mongo_user_db", config);
    docker_mmc_user_db = load_server_config("docker_mmc_user_db", config);
    num_user = load_num_user(config);

    TSimpleServer server(
            boost::make_shared<UserReviewDBProcessor>(boost::make_shared<UserReviewDBHandler>()),
            boost::make_shared<TServerSocket>(stoi(argv[1]) + user_db_server.port),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());
    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}

