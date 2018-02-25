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
ServerInfo get_plot_server;

void exit_handler(int sig) {
    ofstream log_file;
    log_file.open(LOG_PATH);
    log_file << logs;
    log_file.close();
}

class GetPlotHandler: public GetPlotIf {
public:
    GetPlotHandler();
    void ping() override;
    ~GetPlotHandler() override;
    void get_plot(const std::string& req_id, const std::string& movie_id, int32_t server_no) override;
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

GetPlotHandler::GetPlotHandler() {


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

GetPlotHandler::~GetPlotHandler() {
    delete[] store_socket;
    delete[] store_transport;
    delete[] store_protocol;
    delete[] store_client;

    delete[] compose_page_socket;
    delete[] compose_page_transport;
    delete[] compose_page_protocol;
    delete[] compose_page_client;
}

void GetPlotHandler::get_plot(const std::string& req_id, const std::string& movie_id, const int32_t server_no) {
    if (IF_TRACE)
        logger(req_id, "GetPlot", "get_plot", "begin", logs, log_lock);
    int store_index;
   
    string plot;

    store_index = rand() % movie_info_server.num;
    try {
        store_transport[store_index]->open();
        store_client[store_index]->get_info(plot, req_id, movie_id, "plot");
        store_transport[store_index]->close();
    } catch (TException &tx) {
        cout << "ERROR: " << tx.what() << endl;
    }

    
    try {
        compose_page_transport[server_no]->open();
        compose_page_client[server_no]->upload_plot(req_id, movie_id, plot);
        compose_page_transport[server_no]->close();
    } catch (TException &tx) {
        cout << "ERROR: " << tx.what() << endl;
    }

    if (IF_TRACE)
        logger(req_id, "GetPlot", "get_plot", "end", logs, log_lock);
}

void GetPlotHandler::ping () { cout << "ping(from server)" << endl; }

class GetPlotCloneFactory: virtual public GetPlotIfFactory {
public:
    ~GetPlotCloneFactory() override {}
    GetPlotCloneFactory() {

    }

    GetPlotIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) override {
        boost::shared_ptr<TSocket> sock = boost::dynamic_pointer_cast<TSocket>(connInfo.transport);
        return new GetPlotHandler();
    }

    void releaseHandler(GetPlotIf* handler) override {
        delete handler;
    }

};

int main(int argc, char *argv[]) {
    IF_TRACE = true;
    LOG_PATH = LOG_DIR_PATH + "GetPlot.log";


    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    json config;
    config = load_config_file(CONFIG_FILE);
    compose_page_server = load_server_config("compose_page_server", config);
    movie_info_server = load_server_config("movie_info_server", config);
    get_plot_server = load_server_config("get_plot_server", config);


    TThreadedServer server(
            boost::make_shared<GetPlotProcessorFactory>(boost::make_shared<GetPlotCloneFactory>()),
            boost::make_shared<TServerSocket>(get_plot_server.port),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());

    // TSimpleServer server(
    //         boost::make_shared<GetPlotProcessor>(boost::make_shared<GetPlotHandler>(n_store, n_compose_page)),
    //         boost::make_shared<TServerSocket>(10040),
    //         boost::make_shared<TBufferedTransportFactory>(),
    //         boost::make_shared<TBinaryProtocolFactory>());

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}
