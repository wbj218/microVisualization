//
// Created by Yu Gan on 8/5/17.
//

#include "netflix_microservices.h"
#include "../gen-cpp/MovieInfoStorage.h"
#include "../gen-cpp/GetPlot.h"
#include "../gen-cpp/ComposePage.h"
#include <random>
#include <mutex>

#define STORAGE_PORT 10030
#define COMPOSE_PAGE_PORT 10050

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

class GetPlotHandler: public GetPlotIf {
public:
    GetPlotHandler(const int n_movie_info_store, const int n_compose_page);
    void ping() { cout << "ping(from server)" << endl; }
    ~GetPlotHandler();
    void get_plot(const std::string& req_id, const std::string& movie_id, const int32_t server_no);
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
};

GetPlotHandler::GetPlotHandler(const int n_movie_info_store, const int n_compose_page) {
    this->n_movie_info_store = n_movie_info_store;
    this->n_compose_page = n_compose_page;

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
        logger(req_id, "GetPlot", "get_plot", "begin");
    int store_index;
   
    string plot;

    store_index = rand() % n_movie_info_store;
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
        logger(req_id, "GetPlot", "get_plot", "end");
}

class GetPlotCloneFactory: virtual public GetPlotIfFactory {
public:
    virtual ~GetPlotCloneFactory() {}
    GetPlotCloneFactory(int n_store, int n_compose_page) {
        this->n_store = n_store;        
        this->n_compose_page = n_compose_page;
    }

    virtual GetPlotIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo)
    {
        boost::shared_ptr<TSocket> sock = boost::dynamic_pointer_cast<TSocket>(connInfo.transport);
        return new GetPlotHandler(n_store, n_compose_page);
    }
    virtual void releaseHandler(GetPlotIf* handler) {
        delete handler;
    }

private:
    int n_store;    
    int n_compose_page;

};

int main(int argc, char *argv[]) {
    IF_TRACE = true;
    LOG_PATH = LOG_DIR_PATH + "GetPlot.log";

    int n_store = stoi(argv[1]);
    int n_compose_page = stoi(argv[2]);

    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    TThreadedServer server(
            boost::make_shared<GetPlotProcessorFactory>(boost::make_shared<GetPlotCloneFactory>(n_store, n_compose_page)),
            boost::make_shared<TServerSocket>(10040),
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
