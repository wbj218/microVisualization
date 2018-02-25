//
// Created by Yu Gan on 8/5/17.
//

#include "utils.h"




//#define COMPOSE_PAGE_PORT 10050
//
//#define MONGO_PHOTO_PORT 32026
//#define MMC_PHOTO_PORT 32027


using namespace NetflixMicroservices;

json logs;
bool IF_TRACE;
string LOG_PATH;

std::mutex log_lock;

ServerInfo compose_page_server;
ServerInfo get_photo_server;
ServerInfo docker_mongo_photo;
ServerInfo docker_mmc_photo;

void exit_handler(int sig) {
    ofstream log_file;
    log_file.open(LOG_PATH);
    log_file << logs;
    log_file.close();
}

class GetPhotoHandler: public GetPhotoIf {
public:
    GetPhotoHandler();
    void ping() override;
    ~GetPhotoHandler() override;
    void get_photo(const std::string& req_id, const std::string& movie_id, const int32_t server_no) override;
private:


    mongoc_client_t *mongo_client;
    mongoc_collection_t *collection;
    memcached_st *mmc;

    boost::shared_ptr<TTransport>* compose_page_socket;
    boost::shared_ptr<TTransport>* compose_page_transport;
    boost::shared_ptr<TProtocol>* compose_page_protocol;
    boost::shared_ptr<ComposePageClient>* compose_page_client;
};

GetPhotoHandler::GetPhotoHandler() {
    string mmc_configs;
    mongo_client = mongoc_client_new (("mongodb://" + docker_mongo_photo.address + ":" + to_string(docker_mongo_photo.port) +
                                       "/?appname=photo").c_str());
    assert(mongo_client);
    collection =
            mongoc_client_get_collection (mongo_client, "photo", "photo");
    assert(collection);
    mmc_configs = "--SERVER=" + docker_mmc_photo.address + ":" + to_string(docker_mmc_photo.port);
    mmc = memcached(mmc_configs.c_str(), mmc_configs.length());
    assert(mmc);
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_NO_BLOCK, 1);
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_TCP_NODELAY, 1);
//        memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_NOREPLY, 1);
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_TCP_KEEPALIVE, 1);
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_BINARY_PROTOCOL, 1);

    try {
        compose_page_socket = new boost::shared_ptr<TTransport>[compose_page_server.num];
        compose_page_transport = new boost::shared_ptr<TTransport>[compose_page_server.num];
        compose_page_protocol = new boost::shared_ptr<TProtocol>[compose_page_server.num];
        compose_page_client = new boost::shared_ptr<ComposePageClient>[compose_page_server.num];

        for (int i = 0; i < compose_page_server.num; i++) {
            compose_page_socket[i] = (boost::shared_ptr<TTransport>) new TSocket(compose_page_server.address, compose_page_server.port + i);
            compose_page_transport[i] = (boost::shared_ptr<TTransport>) new TBufferedTransport(compose_page_socket[i]);
            compose_page_protocol[i] = (boost::shared_ptr<TProtocol>) new TBinaryProtocol(compose_page_transport[i]);
            compose_page_client[i] = (boost::shared_ptr<ComposePageClient>) new ComposePageClient(compose_page_protocol[i]);
        }

    } catch (TException& tx) {
        cout << "ERROR: " << tx.what() << endl;
    }
}

GetPhotoHandler::~GetPhotoHandler() {
    delete[] compose_page_socket;
    delete[] compose_page_transport;
    delete[] compose_page_protocol;
    delete[] compose_page_client;
}

void GetPhotoHandler::get_photo(const std::string& req_id, const std::string& movie_id, const int32_t server_no) {
    if (IF_TRACE)
        logger(req_id, "GetPhoto", "get_photo", "begin", logs, log_lock);

    
    char* mmc_value = NULL;
    size_t mmc_value_length;
    uint32_t mmc_flags;
    memcached_return_t mmc_rc;
    const bson_t *doc;
    bson_t *query;
    mongoc_cursor_t *cursor;
    json doc_json;
    string photo;

    mmc_value = memcached_get(mmc, movie_id.c_str(), movie_id.length(), &mmc_value_length, &mmc_flags, &mmc_rc);
    if (mmc_value) {
        photo = mmc_value;
    } else {
        query = bson_new();
        BSON_APPEND_UTF8(query, "movie_id", movie_id.c_str());
        cursor = mongoc_collection_find_with_opts (collection, query, NULL, NULL);
        if (mongoc_cursor_next(cursor, &doc)) {
            doc_json = json::parse(bson_as_json (doc, NULL));
            photo = doc_json["photo"];
        }
    }

    
    try {
        compose_page_transport[server_no]->open();
        compose_page_client[server_no]->upload_photo(req_id, movie_id, photo);
        compose_page_transport[server_no]->close();
    } catch (TException &tx) {
        cout << "ERROR: " << tx.what() << endl;
    }




    if (IF_TRACE)
        logger(req_id, "GetPhoto", "get_photo", "end", logs, log_lock);
}

void GetPhotoHandler::ping () { cout << "ping(from server)" << endl; }

class GetPhotoCloneFactory: virtual public GetPhotoIfFactory {
public:
    virtual ~GetPhotoCloneFactory() {}
    GetPhotoCloneFactory() {

    }

    virtual GetPhotoIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo)
    {
        boost::shared_ptr<TSocket> sock = boost::dynamic_pointer_cast<TSocket>(connInfo.transport);
        return new GetPhotoHandler();
    }
    virtual void releaseHandler(GetPhotoIf* handler) {
        delete handler;
    }

private:
    
    int n_compose_page;

};

int main(int argc, char *argv[]) {
    IF_TRACE = true;
    LOG_PATH = LOG_DIR_PATH + "GetPhoto.log";

    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    json config;
    config = load_config_file(CONFIG_FILE);
    compose_page_server = load_server_config("compose_page_server", config);
    get_photo_server = load_server_config("get_photo_server", config);
    docker_mongo_photo = load_server_config("docker_mongo_photo", config);
    docker_mmc_photo = load_server_config("docker_mmc_photo", config);

    TThreadedServer server(
            boost::make_shared<GetPhotoProcessorFactory>(boost::make_shared<GetPhotoCloneFactory>()),
            boost::make_shared<TServerSocket>(get_photo_server.port),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());

    // TSimpleServer server(
    //         boost::make_shared<GetPhotoProcessor>(boost::make_shared<GetPhotoHandler>(n_compose_page)),
    //         boost::make_shared<TServerSocket>(10044),
    //         boost::make_shared<TBufferedTransportFactory>(),
    //         boost::make_shared<TBinaryProtocolFactory>());

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}
