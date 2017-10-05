//
// Created by Yu Gan on 8/5/17.
//

#include "netflix_microservices.h"
#include "../gen-cpp/MovieInfoStorage.h"
#include "../gen-cpp/GetCastInfo.h"
#include "../gen-cpp/ComposePage.h"


#include "libmemcached/memcached.h"
#include <libmongoc-1.0/mongoc.h>
#include <boost/algorithm/string.hpp>
#include <random>
#include <vector>
#include <mutex>

#define STORAGE_PORT 10030
#define COMPOSE_PAGE_PORT 10050

#define MONGO_CAST_INFO_PORT 32024
#define MMC_CAST_INFO_PORT 32025



using namespace NetflixMicroservices;

json logs;
bool IF_TRACE;
string LOG_PATH;
std::mutex thread_mutex;


void logger(const string &log_id, const string &service, const string &stage, const string &state) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long time_in_us = tv.tv_sec * 1000000 + tv.tv_usec;
    thread_mutex.lock();
    logs[log_id][service][stage][state] = time_in_us;
    thread_mutex.unlock();
}

void exit_handler(int sig) {
    ofstream log_file;
    log_file.open(LOG_PATH);
    log_file << logs;
    log_file.close();
}

class GetCastInfoHandler: public GetCastInfoIf {
public:
    GetCastInfoHandler(const int n_movie_info_store, const int n_compose_page);
    void ping() { cout << "ping(from server)" << endl; }
    ~GetCastInfoHandler();
    void get_cast_info(const std::string& req_id, const std::string& movie_id, const int32_t server_no);
private:
    int n_movie_info_store;
    int n_compose_page;
    
    boost::shared_ptr<TTransport>* store_socket;
    boost::shared_ptr<TTransport>* store_transport;
    boost::shared_ptr<TProtocol>* store_protocol;
    boost::shared_ptr<MovieInfoStorageClient>* store_client;

    boost::shared_ptr<TTransport>* compose_page_socket;
    boost::shared_ptr<TTransport>* compose_page_transport;
    boost::shared_ptr<TProtocol>* compose_page_protocol;
    boost::shared_ptr<ComposePageClient>* compose_page_client;

    mongoc_client_t *mongo_client;
    mongoc_collection_t *collection;
    memcached_st *mmc;
};

GetCastInfoHandler::GetCastInfoHandler(const int n_movie_info_store, const int n_compose_page) {

    this->n_movie_info_store = n_movie_info_store;
    this->n_compose_page = n_compose_page;
    

    string mmc_configs;
    mongo_client = mongoc_client_new (("mongodb://" + to_string(DOCKER_IP_ADDR) + ":" + to_string(MONGO_CAST_INFO_PORT) +
                                          "/?appname=cast_info").c_str());
    assert(mongo_client);
    collection =
            mongoc_client_get_collection (mongo_client, "cast_info", "cast_info");
    assert(collection);
    mmc_configs = "--SERVER=" + to_string(DOCKER_IP_ADDR) + ":" + to_string(MMC_CAST_INFO_PORT);
    mmc = memcached(mmc_configs.c_str(), mmc_configs.length());
    assert(mmc);
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_NO_BLOCK, 1);
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_TCP_NODELAY, 1);
//        memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_NOREPLY, 1);
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_TCP_KEEPALIVE, 1);
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_BINARY_PROTOCOL, 1);
    
    
    try {
        store_socket = new boost::shared_ptr<TTransport>[n_movie_info_store];
        store_transport = new boost::shared_ptr<TTransport>[n_movie_info_store];
        store_protocol = new boost::shared_ptr<TProtocol>[n_movie_info_store];
        store_client = new boost::shared_ptr<MovieInfoStorageClient>[n_movie_info_store];

        compose_page_socket = new boost::shared_ptr<TTransport>[n_compose_page];
        compose_page_transport = new boost::shared_ptr<TTransport>[n_compose_page];
        compose_page_protocol = new boost::shared_ptr<TProtocol>[n_compose_page];
        compose_page_client = new boost::shared_ptr<ComposePageClient>[n_compose_page];

        for (int i = 0; i < n_movie_info_store; i++) {
            store_socket[i] = (boost::shared_ptr<TTransport>) new TSocket("localhost", STORAGE_PORT + i);
            store_transport[i] = (boost::shared_ptr<TTransport>) new TBufferedTransport(store_socket[i]);
            store_protocol[i] = (boost::shared_ptr<TProtocol>) new TBinaryProtocol(store_transport[i]);
            store_client[i] = (boost::shared_ptr<MovieInfoStorageClient>) new MovieInfoStorageClient(store_protocol[i]);
        }

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

GetCastInfoHandler::~GetCastInfoHandler() {
    delete[] store_socket;
    delete[] store_transport;
    delete[] store_protocol;
    delete[] store_client;

    delete[] compose_page_socket;
    delete[] compose_page_transport;
    delete[] compose_page_protocol;
    delete[] compose_page_client;

    memcached_free(mmc);
}

void GetCastInfoHandler::get_cast_info(const std::string& req_id, const std::string& movie_id, const int32_t server_no) {
    
    if (IF_TRACE)
        logger(req_id, "GetCastInfo", "get_cast_info", "begin");

    
    
    int store_index;
    
    string casts;

    store_index = rand() % n_movie_info_store;
    try {
        store_transport[store_index]->open();
        store_client[store_index]->get_info(casts, req_id, movie_id, "cast");
        store_transport[store_index]->close();
    } catch (TException &tx) {
        cout << "ERROR: " << tx.what() << endl;
    }

    vector<string> cast_list;
    vector<CastInfo> cast_info_list;
    boost::split(cast_list, casts, [](char c){return c == '\n';});

    char* mmc_value = NULL;
    size_t mmc_value_length;
    uint32_t mmc_flags;
    memcached_return_t mmc_rc;
    const bson_t *doc;
    bson_t *query;
    mongoc_cursor_t *cursor;
    json doc_json;
    CastInfo new_cast_info;

    for(auto const& cast_id: cast_list) {
        mmc_value = memcached_get(mmc, cast_id.c_str(), cast_id.length(), &mmc_value_length, &mmc_flags, &mmc_rc);
        if (mmc_value) {
            new_cast_info.cast_id = cast_id;
            new_cast_info.info = mmc_value;
            cast_info_list.push_back(new_cast_info);
        } else {
            query = bson_new();
            BSON_APPEND_UTF8(query, "cast_id", cast_id.c_str());
            
            cursor = mongoc_collection_find_with_opts (collection, query, NULL, NULL);
            if (mongoc_cursor_next(cursor, &doc)) {
                doc_json = json::parse(bson_as_json (doc, NULL));
                new_cast_info.cast_id = cast_id;
                new_cast_info.info = doc_json["info"];
                cast_info_list.push_back(new_cast_info);
            }
            bson_destroy(query);
            mongoc_cursor_destroy (cursor);
            
        }
        free(mmc_value);
    }

    
    try {
        compose_page_transport[server_no]->open();
        compose_page_client[server_no]->upload_cast_info(req_id, movie_id, cast_info_list);
        compose_page_transport[server_no]->close();
    } catch (TException &tx) {
        cout << "ERROR: " << tx.what() << endl;
    }

      
    if (IF_TRACE)
        logger(req_id, "GetCastInfo", "get_cast_info", "end");
    
}

class GetCastInfoCloneFactory: virtual public GetCastInfoIfFactory {
public:
    virtual ~GetCastInfoCloneFactory() {}
    GetCastInfoCloneFactory(int n_store, int n_compose_page) {
        this->n_store = n_store;
        this->n_compose_page = n_compose_page;
    }

    virtual GetCastInfoIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo)
    {
        boost::shared_ptr<TSocket> sock = boost::dynamic_pointer_cast<TSocket>(connInfo.transport);
        return new GetCastInfoHandler(n_store, n_compose_page);
    }
    virtual void releaseHandler(GetCastInfoIf* handler) {
        delete handler;
    }

private:
    int n_store;
    int n_compose_page;

};

int main(int argc, char *argv[]) {
    IF_TRACE = true;
    LOG_PATH = LOG_DIR_PATH + "GetCastInfo.log";

    int n_store = stoi(argv[1]);
    int n_compose_page = stoi(argv[2]);
    
    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    TThreadedServer server(
            boost::make_shared<GetCastInfoProcessorFactory>(boost::make_shared<GetCastInfoCloneFactory>(n_store, n_compose_page)),
            boost::make_shared<TServerSocket>(10043),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());

    // TSimpleServer server(
    //         boost::make_shared<GetCastInfoProcessor>(boost::make_shared<GetCastInfoHandler>(n_store, n_compose_page)),
    //         boost::make_shared<TServerSocket>(10043),
    //         boost::make_shared<TBufferedTransportFactory>(),
    //         boost::make_shared<TBinaryProtocolFactory>());

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}
