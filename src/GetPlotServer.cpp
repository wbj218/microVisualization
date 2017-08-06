//
// Created by Yu Gan on 8/5/17.
//

#include "netflix_microservices.h"
#include "../gen-cpp/MovieInfoStorage.h"
#include "../gen-cpp/GetPlot.h"
#include <random>

#define STORAGE_PORT 10030

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

class GetPlotHandler: public GetPlotIf {
public:
    GetPlotHandler(const int n_movie_info_store);
    void ping() { cout << "ping(from server)" << endl; }
    ~GetPlotHandler();
    void get_plot(std::string& _return, const std::string& req_id, const std::string& movie_id);
private:
    int n_movie_info_store;
    boost::shared_ptr<TTransport>* store_socket;
    boost::shared_ptr<TTransport>* store_transport;
    boost::shared_ptr<TProtocol>* store_protocol;
    boost::shared_ptr<MovieInfoStorageClient>* store_client;
};

GetPlotHandler::GetPlotHandler(const int n_movie_info_store) {
    this->n_movie_info_store = n_movie_info_store;
    try {
        store_socket = new boost::shared_ptr<TTransport>[n_movie_info_store];
        store_transport = new boost::shared_ptr<TTransport>[n_movie_info_store];
        store_protocol = new boost::shared_ptr<TProtocol>[n_movie_info_store];
        store_client = new boost::shared_ptr<MovieInfoStorageClient>[n_movie_info_store];

        for (int i = 0; i < n_movie_info_store; i++) {
            store_socket[i] = (boost::shared_ptr<TTransport>) new TSocket("localhost", STORAGE_PORT + i);
            store_transport[i] = (boost::shared_ptr<TTransport>) new TBufferedTransport(store_socket[i]);
            store_protocol[i] = (boost::shared_ptr<TProtocol>) new TBinaryProtocol(store_transport[i]);
            store_client[i] = (boost::shared_ptr<MovieInfoStorageClient>) new MovieInfoStorageClient(store_protocol[i]);
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
}

void GetPlotHandler::get_plot(std::string& _return, const std::string& req_id, const std::string& movie_id) {
    if (IF_TRACE)
        logger(req_id, "GetPlot", "get_plot", "begin");

    try {
        int store_index;
        store_index = rand() % n_movie_info_store;
        store_transport[store_index]->open();
        store_client[store_index]->get_info(_return, req_id, movie_id, "plot");
        store_transport[store_index]->close();

    } catch (TException &tx) {
        cout << "ERROR: " << tx.what() << endl;
    }

    if (IF_TRACE)
        logger(req_id, "GetPlot", "get_plot", "end");
}

int main(int argc, char *argv[]) {
    IF_TRACE = true;
    LOG_PATH = "../logs/GetPlot";

    int n_store = stoi(argv[1]);
    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    TSimpleServer server(
            boost::make_shared<GetPlotProcessor>(boost::make_shared<GetPlotHandler>(n_store)),
            boost::make_shared<TServerSocket>(10040),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}
