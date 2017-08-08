//
// Created by Yu Gan on 8/5/17.
//

#include "netflix_microservices.h"
#include "../gen-cpp/MovieInfoStorage.h"
#include "../gen-cpp/GetPhoto.h"
#include "../gen-cpp/ComposePage.h"
#include <random>

#include "libmemcached/memcached.h"
#include <libmongoc-1.0/mongoc.h>


#define COMPOSE_PAGE_PORT 10050

#define MONGO_CAST_INFO_PORT 32026
#define MMC_CAST_INFO_PORT 32027
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

class GetPhotoHandler: public GetPhotoIf {
public:
    GetPhotoHandler(const int n_compose_page);
    void ping() { cout << "ping(from server)" << endl; }
    ~GetPhotoHandler();
    void get_photo(const std::string& req_id, const std::string& movie_id);
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

GetPhotoHandler::GetPhotoHandler(const int n_compose_page) {

    this->n_compose_page = n_compose_page;

    string mmc_configs;
    mongo_client = mongoc_client_new (("mongodb://" + to_string(IP_ADDR) + ":" + MONGO_CAST_INFO_PORT +
                                       "/?appname=photo").c_str());
    assert(mongo_client);
    collection =
            mongoc_client_get_collection (mongo_client, "photo", "photo_db");
    assert(collection);
    mmc_configs = "--SERVER=" + to_string(IP_ADDR) + ":" + MMC_CAST_INFO_PORT;
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

GetPhotoHandler::~GetPhotoHandler() {
    delete[] compose_page_socket;
    delete[] compose_page_transport;
    delete[] compose_page_protocol;
    delete[] compose_page_client;
}

void GetPhotoHandler::get_photo(const std::string& req_id, const std::string& movie_id) {
    if (IF_TRACE)
        logger(req_id, "GetPhoto", "get_photo", "begin");

    try {
        int store_index;
        store_index = rand() % n_movie_info_store;
        store_transport[store_index]->open();
        store_client[store_index]->get_info(_return, req_id, movie_id, "photo");
        store_transport[store_index]->close();

    } catch (TException &tx) {
        cout << "ERROR: " << tx.what() << endl;
    }

    if (IF_TRACE)
        logger(req_id, "GetPhoto", "get_photo", "end");
}

int main(int argc, char *argv[]) {
    IF_TRACE = true;
    LOG_PATH = "../logs/GetPhoto.log";

    int n_store = stoi(argv[1]);
    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    TSimpleServer server(
            boost::make_shared<GetPhotoProcessor>(boost::make_shared<GetPhotoHandler>(n_store)),
            boost::make_shared<TServerSocket>(10044),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}
