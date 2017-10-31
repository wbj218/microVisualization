//
// Created by Yu Gan on 8/4/17.
//

#include "utils.h"

//#define MONGO_PORT 32022
//#define MMC_PORT 32023
//#define SERVER_PORT_START 10030

using namespace NetflixMicroservices;

json logs;
bool IF_TRACE;
string LOG_PATH;

std::mutex log_lock;

ServerInfo docker_mongo_movie_info;
ServerInfo docker_mmc_movie_info;
ServerInfo movie_info_server;


void exit_handler(int sig) {
    ofstream log_file;
    log_file.open(LOG_PATH);
    log_file << logs;
    log_file.close();
}

class MovieInfoStorageHandler: public MovieInfoStorageIf {
public:
    MovieInfoStorageHandler();
    ~MovieInfoStorageHandler() override;
    void ping() override;
    void get_info(std::string& _return, const std::string& req_id, const std::string& movie_id, const string &type) override;

private:
    mongoc_client_t *mongo_client;
    memcached_st *mmc;
};

MovieInfoStorageHandler::MovieInfoStorageHandler() {
    string mmc_configs;
    mongo_client = mongoc_client_new (("mongodb://" + docker_mongo_movie_info.address + ":" + to_string(docker_mongo_movie_info.port) +
                                       "/?appname=movie_info").c_str());
    assert(mongo_client);
    mmc_configs = "--SERVER=" + docker_mmc_movie_info.address + ":" + to_string(docker_mmc_movie_info.port);
    mmc = memcached(mmc_configs.c_str(), mmc_configs.length());
    assert(mmc);
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_NO_BLOCK, 1);
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_TCP_NODELAY, 1);
//    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_NOREPLY, 1);
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_TCP_KEEPALIVE, 1);
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_BINARY_PROTOCOL, 1);
}

MovieInfoStorageHandler::~MovieInfoStorageHandler() {
    mongoc_client_destroy (mongo_client);
    memcached_free(mmc);
}

void MovieInfoStorageHandler::get_info(std::string& _return, const std::string& req_id, const std::string& movie_id,
                                       const string &type) {
    if (IF_TRACE)
        logger(req_id, "MovieInfoStorage", "get_" + type, "begin", logs, log_lock);

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
        logger(req_id, "MovieInfoStorage", "get_" + type, "end", logs, log_lock);
}

void MovieInfoStorageHandler::ping () { cout << "ping(from server)" << endl; }

class MovieInfoStorageCloneFactory: virtual public MovieInfoStorageIfFactory {
public:
    virtual ~MovieInfoStorageCloneFactory() {}
   

    virtual MovieInfoStorageIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo)
    {
        boost::shared_ptr<TSocket> sock = boost::dynamic_pointer_cast<TSocket>(connInfo.transport);
        return new MovieInfoStorageHandler();
    }
    virtual void releaseHandler(MovieInfoStorageIf* handler) {
        delete handler;
    }



};

int main (int argc, char *argv[]) {
    IF_TRACE = true;
    LOG_PATH = LOG_DIR_PATH +  "MovieInfoStorage_" + to_string(stoi(argv[1])) + ".log";

    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    json config;
    config = load_config_file(CONFIG_FILE);
    docker_mongo_movie_info = load_server_config("docker_mongo_movie_info", config);
    docker_mmc_movie_info = load_server_config("docker_mmc_movie_info", config);
    movie_info_server = load_server_config("movie_info_server", config);

    TThreadedServer server(
            boost::make_shared<MovieInfoStorageProcessorFactory>(boost::make_shared<MovieInfoStorageCloneFactory>()),
            boost::make_shared<TServerSocket>(stoi(argv[1]) + movie_info_server.port),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());

    // TSimpleServer server(
    //         boost::make_shared<MovieInfoStorageProcessor>(boost::make_shared<MovieInfoStorageHandler>()),
    //         boost::make_shared<TServerSocket>(stoi(argv[1]) - 1 + SERVER_PORT_START),
    //         boost::make_shared<TBufferedTransportFactory>(),
    //         boost::make_shared<TBinaryProtocolFactory>());
    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}





