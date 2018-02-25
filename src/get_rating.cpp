//
// Created by Yu Gan on 8/5/17.
//

#include "utils.h"



//#define STORAGE_PORT 10030
//#define COMPOSE_PAGE_PORT 10050

using namespace NetflixMicroservices;

json logs;
bool IF_TRACE;
string LOG_PATH;

std::mutex log_lock;

ServerInfo compose_page_server;
ServerInfo movie_info_server;
ServerInfo get_rating_server;

void exit_handler(int sig) {
    ofstream log_file;
    log_file.open(LOG_PATH);
    log_file << logs;
    log_file.close();
}

class GetRatingHandler: public GetRatingIf {
public:
    GetRatingHandler();
    void ping() { cout << "ping(from server)" << endl; }
    ~GetRatingHandler() override;
    void get_rating(const std::string& req_id, const std::string& movie_id, int32_t server_no);
private:
    boost::shared_ptr<TTransport>* store_socket;
    boost::shared_ptr<TTransport>* store_transport;
    boost::shared_ptr<TProtocol>* store_protocol;
    boost::shared_ptr<MovieInfoStorageClient>* store_client;

    boost::shared_ptr<TTransport>* compose_page_socket;
    boost::shared_ptr<TTransport>* compose_page_transport;
    boost::shared_ptr<TProtocol>* compose_page_protocol;
    boost::shared_ptr<ComposePageClient>* compose_page_client;
};

GetRatingHandler::GetRatingHandler() {
    try {
        store_socket = new boost::shared_ptr<TTransport>[movie_info_server.num];
        store_transport = new boost::shared_ptr<TTransport>[movie_info_server.num];
        store_protocol = new boost::shared_ptr<TProtocol>[movie_info_server.num];
        store_client = new boost::shared_ptr<MovieInfoStorageClient>[movie_info_server.num];

        compose_page_socket = new boost::shared_ptr<TTransport>[compose_page_server.num];
        compose_page_transport = new boost::shared_ptr<TTransport>[compose_page_server.num];
        compose_page_protocol = new boost::shared_ptr<TProtocol>[compose_page_server.num];
        compose_page_client = new boost::shared_ptr<ComposePageClient>[compose_page_server.num];

        for (int i = 0; i < movie_info_server.num; i++) {
            store_socket[i] = (boost::shared_ptr<TTransport>) new TSocket(movie_info_server.address, movie_info_server.port + i);
            store_transport[i] = (boost::shared_ptr<TTransport>) new TBufferedTransport(store_socket[i]);
            store_protocol[i] = (boost::shared_ptr<TProtocol>) new TBinaryProtocol(store_transport[i]);
            store_client[i] = (boost::shared_ptr<MovieInfoStorageClient>) new MovieInfoStorageClient(store_protocol[i]);
        }

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

GetRatingHandler::~GetRatingHandler() {
    delete[] store_socket;
    delete[] store_transport;
    delete[] store_protocol;
    delete[] store_client;

    delete[] compose_page_socket;
    delete[] compose_page_transport;
    delete[] compose_page_protocol;
    delete[] compose_page_client;
}

void GetRatingHandler::get_rating(const std::string& req_id, const std::string& movie_id, const int32_t server_no) {
    if (IF_TRACE)
        logger(req_id, "GetRating", "get_rating", "begin", logs, log_lock);

    int store_index;

    store_index = rand() % movie_info_server.num;
    string rating;

    try {
        store_transport[store_index]->open();
        store_client[store_index]->get_info(rating, req_id, movie_id, "rating");
        store_transport[store_index]->close();

    } catch (TException &tx) {
        cout << "ERROR: " << tx.what() << endl;
    }

    
    try {
        compose_page_transport[server_no]->open();
        compose_page_client[server_no]->upload_rating(req_id, movie_id, rating);
        compose_page_transport[server_no]->close();
    } catch (TException &tx) {
        cout << "ERROR: " << tx.what() << endl;
    }

    if (IF_TRACE)
        logger(req_id, "GetRating", "get_rating", "end", logs, log_lock);
}

class GetRatingCloneFactory: virtual public GetRatingIfFactory {
public:
    virtual ~GetRatingCloneFactory() {}
    GetRatingCloneFactory() {

    }

    virtual GetRatingIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo)
    {
        boost::shared_ptr<TSocket> sock = boost::dynamic_pointer_cast<TSocket>(connInfo.transport);
        return new GetRatingHandler();
    }
    virtual void releaseHandler(GetRatingIf* handler) {
        delete handler;
    }

private:
    int n_store;    
    int n_compose_page;

};

int main(int argc, char *argv[]) {
    IF_TRACE = true;
    LOG_PATH = LOG_DIR_PATH + "GetRating.log";


    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    json config;
    config = load_config_file(CONFIG_FILE);
    compose_page_server = load_server_config("compose_page_server", config);
    movie_info_server = load_server_config("movie_info_server", config);
    get_rating_server = load_server_config("get_rating_server", config);

    TThreadedServer server(
            boost::make_shared<GetRatingProcessorFactory>(boost::make_shared<GetRatingCloneFactory>()),
            boost::make_shared<TServerSocket>(get_rating_server.port),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());

    // TSimpleServer server(
    //         boost::make_shared<GetRatingProcessor>(boost::make_shared<GetRatingHandler>(n_store, n_compose_page)),
    //         boost::make_shared<TServerSocket>(10042),
    //         boost::make_shared<TBufferedTransportFactory>(),
    //         boost::make_shared<TBinaryProtocolFactory>());

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}
