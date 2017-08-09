//
// Created by Yu Gan on 8/2/17.
//

#include "netflix_microservices.h"
#include "../gen-cpp/AssignRating.h"
#include "../gen-cpp/ComposeReview.h"

using namespace NetflixMicroservices;

json logs;
string LOG_PATH;
bool IF_TRACE;


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

class AssignRatingHandler: public AssignRatingIf {
public:
    AssignRatingHandler();

    ~AssignRatingHandler();

    void ping() { cout << "ping(from server)" << endl; }

    void assign_rating(const string &, const string &);

private:
    boost::shared_ptr<TTransport> compose_socket;
    boost::shared_ptr<TTransport> compose_transport;
    boost::shared_ptr<TProtocol> compose_protocol;
    ComposeReviewClient *compose_client;
};

AssignRatingHandler::AssignRatingHandler() {
    try {
        compose_socket = (boost::shared_ptr<TTransport>) new TSocket("localhost", 9090);
        compose_transport = (boost::shared_ptr<TTransport>)new TBufferedTransport(compose_socket);
        compose_protocol = (boost::shared_ptr<TProtocol>)new TBinaryProtocol(compose_transport);
        compose_client = new ComposeReviewClient(compose_protocol);
    } catch (TException& tx) {
        cout << "ERROR: " << tx.what() << endl;
    }
}


AssignRatingHandler::~AssignRatingHandler() {
//    delete compose_client;
}

void AssignRatingHandler::assign_rating(const string& req_id, const string& rating) {
    if (IF_TRACE)
        logger(req_id, "AssignRating", "assign_rating", "begin");
    try {
        compose_transport->open();
        compose_client->upload(req_id, "rating", rating);
        compose_transport->close();
    } catch (TException& tx) {
        cout << "ERROR: " << tx.what() << endl;
    }
    if (IF_TRACE)
        logger(req_id, "AssignRating", "assign_rating", "end");
}


int main() {
    IF_TRACE = true;
    LOG_PATH = LOG_DIR_PATH + "AssignRating.log";

    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    TSimpleServer server(
            boost::make_shared<AssignRatingProcessor>(boost::make_shared<AssignRatingHandler>()),
            boost::make_shared<TServerSocket>(9093),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}