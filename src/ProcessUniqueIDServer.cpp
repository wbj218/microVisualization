//
// Created by Yu Gan on 8/3/17.
//

#include "netflix_microservices.h"
#include "../gen-cpp/ProcessUniqueID.h"
#include "../gen-cpp/ComposeReview.h"
#include <random>

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

class ProcessUniqueIDHandler: public ProcessUniqueIDIf {
public:
    ProcessUniqueIDHandler();
    ~ProcessUniqueIDHandler();
    void ping() {cout << "ping(from server)" << endl;}
    void get_unique_id(const string&);

private:
    default_random_engine generator;

    boost::shared_ptr<TTransport> compose_socket;
    boost::shared_ptr<TTransport> compose_transport;
    boost::shared_ptr<TProtocol> compose_protocol;
    ComposeReviewClient *compose_client;

};

ProcessUniqueIDHandler::ProcessUniqueIDHandler() {
    generator.seed((unsigned int) time(NULL));
    try {
        compose_socket = (boost::shared_ptr<TTransport>) new TSocket("localhost", 9090);
        compose_transport = (boost::shared_ptr<TTransport>) new TBufferedTransport(compose_socket);
        compose_protocol = (boost::shared_ptr<TProtocol>) new TBinaryProtocol(compose_transport);
        compose_client = new ComposeReviewClient(compose_protocol);
    } catch (TException& tx) {
        cout << "ERROR: " << tx.what() << endl;
    }
}

ProcessUniqueIDHandler::~ProcessUniqueIDHandler() {
//    delete compose_client;

}

void ProcessUniqueIDHandler::get_unique_id(const string& req_id) {
    if (IF_TRACE)
        logger(req_id, "ProcessUniqueID", "process_unique_id", "begin");
    uniform_int_distribution<unsigned long long > distribution(0, ULLONG_MAX);

//    cout<<req_id<<" "<<to_string(distribution(generator))<<endl;
    try {
        compose_transport->open();
        compose_client->upload(req_id, "unique_id", to_string(distribution(generator)));
        compose_transport->close();
    } catch (TException& tx) {
        cout << "ERROR: " << tx.what() << endl;
    }

    if (IF_TRACE)
        logger(req_id, "ProcessUniqueID", "process_unique_id", "end");
}

int main() {

    IF_TRACE = false;
    LOG_PATH = "../logs/ProcessUniqueID";

    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    TSimpleServer server(
            boost::make_shared<ProcessUniqueIDProcessor>(boost::make_shared<ProcessUniqueIDHandler>()),
            boost::make_shared<TServerSocket>(9091),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}

