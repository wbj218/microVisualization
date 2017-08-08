//
// Created by Yu Gan on 8/5/17.
//

#include "netflix_microservices.h"
#include "../gen-cpp/MovieInfoStorage.h"
#include "../gen-cpp/GetVideo.h"
#include "../gen-cpp/ComposePage.h"
#include <random>

#include "libmemcached/memcached.h"
#include <libmongoc-1.0/mongoc.h>


#define COMPOSE_PAGE_PORT 10050

#define MONGO_VIDEO_PORT 32028
#define MMC_VIDEO_PORT 32029
#define IP_ADDR "192.168.99.100"

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

class GetVideoHandler: public GetVideoIf {
public:
    GetVideoHandler(const int n_compose_page);
    void ping() { cout << "ping(from server)" << endl; }
    ~GetVideoHandler();
    void get_video(const std::string& req_id, const std::string& movie_id);
private:
    int n_compose_page;

    mongoc_client_t *mongo_client;
    mongoc_collection_t *collection;
    memcached_st *mmc;

    boost::shared_ptr<TTransport>* compose_page_socket;
    boost::shared_ptr<TTransport>* compose_page_transport;
    boost::shared_ptr<TProtocol>* compose_page_protocol;
    boost::shared_ptr<ComposePageClient>* compose_page_client;
};

GetVideoHandler::GetVideoHandler(const int n_compose_page) {

    this->n_compose_page = n_compose_page;

    string mmc_configs;
    mongo_client = mongoc_client_new (("mongodb://" + to_string(IP_ADDR) + ":" + MONGO_VIDEO_PORT +
                                       "/?appname=video").c_str());
    assert(mongo_client);
    collection =
            mongoc_client_get_collection (mongo_client, "video", "video_db");
    assert(collection);
    mmc_configs = "--SERVER=" + to_string(IP_ADDR) + ":" + MMC_VIDEO_PORT;
    mmc = memcached(mmc_configs.c_str(), mmc_configs.length());
    assert(mmc);
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_NO_BLOCK, 1);
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_TCP_NODELAY, 1);
//        memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_NOREPLY, 1);
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_TCP_KEEPALIVE, 1);

    try {
        compose_page_socket = new boost::shared_ptr<TTransport>[n_compose_page];
        compose_page_transport = new boost::shared_ptr<TTransport>[n_compose_page];
        compose_page_protocol = new boost::shared_ptr<TProtocol>[n_compose_page];
        compose_page_client = new boost::shared_ptr<ComposePageClient>[n_compose_page];

        for (int i = 0; i < n_compose_page; i++) {
            compose_page_socket[i] = (boost::shared_ptr<TTransport>) new TSocket("localhost", COMPOSE_PAGE_PORT + i);
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

void GetVideoHandler::get_video(const std::string& req_id, const std::string& movie_id) {
    if (IF_TRACE)
        logger(req_id, "GetVideo", "get_video", "begin");

    int compose_page_index;
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

    compose_page_index = rand() % n_compose_page;
    try {
        compose_page_transport[compose_page_index]->open();
        compose_page_client[compose_page_index]->upload_video(req_id, movie_id, video);
        compose_page_transport[compose_page_index]->close();
    } catch (TException &tx) {
        cout << "ERROR: " << tx.what() << endl;
    }




    if (IF_TRACE)
        logger(req_id, "GetVideo", "get_video", "end");
}

int main(int argc, char *argv[]) {
    IF_TRACE = true;
    LOG_PATH = "../logs/GetVideo.log";

    int n_compose_page = stoi(argv[1]);
    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    TSimpleServer server(
            boost::make_shared<GetVideoProcessor>(boost::make_shared<GetVideoHandler>(n_compose_page)),
            boost::make_shared<TServerSocket>(10045),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}
