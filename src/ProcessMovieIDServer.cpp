//
// Created by Yu Gan on 8/2/17.
//


#include "netflix_microservices.h"
#include "../gen-cpp/ProcessMovieID.h"
#include "../gen-cpp/ComposeReview.h"

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

class ProcessMovieIDHandler: public ProcessMovieIDIf {
public:
    ProcessMovieIDHandler();

    ~ProcessMovieIDHandler();

    void ping() { cout << "ping(from server)" << endl; }

    void process_movie_id(const string &, const string &);
    void get_movie_id(string &, const string &, const string &);

private:
    boost::shared_ptr<TTransport> compose_socket;
    boost::shared_ptr<TTransport> compose_transport;
    boost::shared_ptr<TProtocol> compose_protocol;
    ComposeReviewClient *compose_client;
};

ProcessMovieIDHandler::ProcessMovieIDHandler() {
    try {
        compose_socket = (boost::shared_ptr<TTransport>) new TSocket("localhost", 9090);
        compose_transport = (boost::shared_ptr<TTransport>)new TBufferedTransport(compose_socket);
        compose_protocol = (boost::shared_ptr<TProtocol>)new TBinaryProtocol(compose_transport);
        compose_client = new ComposeReviewClient(compose_protocol);
    } catch (TException& tx) {
        cout << "ERROR: " << tx.what() << endl;
    }
}


ProcessMovieIDHandler::~ProcessMovieIDHandler() {
//    delete compose_client;
}

void ProcessMovieIDHandler::process_movie_id(const string& req_id, const string& url) {
    if (IF_TRACE)
        logger(req_id, "ProcessMovieID", "process_movie_id", "begin");
    string str_match = "www.imdb.com/title/";
    size_t found = url.find(str_match);
    assert(found!=string::npos);
    string _return = url.substr(found + str_match.length(), string::npos);

//    cout<<_return<<endl;

    try {
        compose_transport->open();
        compose_client->upload(req_id, "movie_id", _return);
        compose_transport->close();
    } catch (TException& tx) {
        cout << "ERROR: " << tx.what() << endl;
    }
    if (IF_TRACE)
        logger(req_id, "ProcessMovieID", "process_movie_id", "end");
}

void ProcessMovieIDHandler::get_movie_id(string& _return, const string& req_id, const string& url) {
    if (IF_TRACE)
        logger(req_id, "ProcessMovieID", "get_movie_id", "begin");

    string str_match = "www.imdb.com/title/";
    size_t found = url.find(str_match);
    assert(found!=string::npos);
    _return = url.substr(found + str_match.length(), string::npos);

    if (IF_TRACE)
        logger(req_id, "ProcessMovieID", "get_movie_id", "end");
}


int main() {
    IF_TRACE = true;
    LOG_PATH = LOG_DIR_PATH + "ProcessMovieID.log";

    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    TSimpleServer server(
            boost::make_shared<ProcessMovieIDProcessor>(boost::make_shared<ProcessMovieIDHandler>()),
            boost::make_shared<TServerSocket>(9094),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}