//
// Created by Yu Gan on 8/5/17.
//

#include "utils.h"



//#define COMPOSE_PAGE_PORT 10050
//
//#define MONGO_VIDEO_PORT 32028
//#define MMC_VIDEO_PORT 32029


using namespace NetflixMicroservices;

json logs;
bool IF_TRACE;
string LOG_PATH;

std::mutex log_lock;

ServerInfo compose_page_server;
ServerInfo get_video_server;
ServerInfo docker_mongo_video;
ServerInfo docker_mmc_video;


void exit_handler(int sig) {
    ofstream log_file;
    log_file.open(LOG_PATH);
    log_file << logs;
    log_file.close();
}

class GetVideoHandler: public GetVideoIf {
public:
    GetVideoHandler();
    void ping();
    ~GetVideoHandler();
    void get_video(const std::string& req_id, const std::string& movie_id, const int32_t server_no);
private:
    mongoc_client_t *mongo_client;
    mongoc_collection_t *collection;
    memcached_st *mmc;

    boost::shared_ptr<TTransport>* compose_page_socket;
    boost::shared_ptr<TTransport>* compose_page_transport;
    boost::shared_ptr<TProtocol>* compose_page_protocol;
    boost::shared_ptr<ComposePageClient>* compose_page_client;
};

GetVideoHandler::GetVideoHandler() {

    string mmc_configs;
    mongo_client = mongoc_client_new (("mongodb://" + docker_mongo_video.address + ":" + to_string(docker_mongo_video.port) +
                                       "/?appname=video").c_str());
    assert(mongo_client);
    collection =
            mongoc_client_get_collection (mongo_client, "video", "video");
    assert(collection);
    mmc_configs = "--SERVER=" + docker_mmc_video.address + ":" + to_string(docker_mmc_video.port);
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

GetVideoHandler::~GetVideoHandler() {
    delete[] compose_page_socket;
    delete[] compose_page_transport;
    delete[] compose_page_protocol;
    delete[] compose_page_client;
}

void GetVideoHandler::get_video(const std::string& req_id, const std::string& movie_id, const int32_t server_no) {
    if (IF_TRACE)
        logger(req_id, "GetVideo", "get_video", "begin", logs, log_lock);
    
    char* mmc_value = NULL;
    size_t mmc_value_length;
    uint32_t mmc_flags;
    memcached_return_t mmc_rc;
    const bson_t *doc;
    bson_t *query;
    mongoc_cursor_t *cursor;
    json doc_json;
    string video;

    mmc_value = memcached_get(mmc, movie_id.c_str(), movie_id.length(), &mmc_value_length, &mmc_flags, &mmc_rc);
    if (mmc_value) {
        video = mmc_value;
    } else {
        query = bson_new();
        BSON_APPEND_UTF8(query, "movie_id", movie_id.c_str());
        cursor = mongoc_collection_find_with_opts (collection, query, NULL, NULL);
        if (mongoc_cursor_next(cursor, &doc)) {
            doc_json = json::parse(bson_as_json (doc, NULL));
            video = doc_json["video"];
        }
    }
    
    try {
        compose_page_transport[server_no]->open();
        compose_page_client[server_no]->upload_video(req_id, movie_id, video);
        compose_page_transport[server_no]->close();
    } catch (TException &tx) {
        cout << "ERROR: " << tx.what() << endl;
    }

    if (IF_TRACE)
        logger(req_id, "GetVideo", "get_video", "end", logs, log_lock);
}

void GetVideoHandler::ping () { cout << "ping(from server)" << endl; }

class GetVideoCloneFactory: virtual public GetVideoIfFactory {
public:
    virtual ~GetVideoCloneFactory() {}
    GetVideoCloneFactory() {
          

    }

    virtual GetVideoIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo)
    {
        boost::shared_ptr<TSocket> sock = boost::dynamic_pointer_cast<TSocket>(connInfo.transport);
        return new GetVideoHandler();
    }
    virtual void releaseHandler(GetVideoIf* handler) {
        delete handler;
    }

private:
      
    int n_compose_page;

};

int main(int argc, char *argv[]) {
    IF_TRACE = true;
    LOG_PATH = LOG_DIR_PATH + "GetVideo.log";


    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    json config;
    config = load_config_file(CONFIG_FILE);
    compose_page_server = load_server_config("compose_page_server", config);
    get_video_server = load_server_config("get_video_server", config);
    docker_mongo_video = load_server_config("docker_mongo_video", config);
    docker_mmc_video = load_server_config("docker_mmc_video", config);

    TThreadedServer server(
            boost::make_shared<GetVideoProcessorFactory>(boost::make_shared<GetVideoCloneFactory>()),
            boost::make_shared<TServerSocket>(get_video_server.port),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());

    // TSimpleServer server(
    //         boost::make_shared<GetVideoProcessor>(boost::make_shared<GetVideoHandler>(n_compose_page)),
    //         boost::make_shared<TServerSocket>(10045),
    //         boost::make_shared<TBufferedTransportFactory>(),
    //         boost::make_shared<TBinaryProtocolFactory>());

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}
