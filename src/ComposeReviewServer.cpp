//
// Created by Yu Gan on 8/2/17.
//

#include "netflix_microservices.h"
#include "../gen-cpp/ComposeReview.h"
#include <boost/algorithm/string.hpp>
#include <vector>

using namespace NetflixMicroservices;

json logs;
bool IF_TRACE;
string LOG_PATH;
vector<Review> review_buf;

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

class ComposeReviewHandler: public ComposeReviewIf {
public:
    ComposeReviewHandler(const int n_review_store, const int n_movie_db, const int n_user_db,
                         const int* review_store_ports, const int* movie_db_ports, const int* user_db_ports);
    ComposeReviewHandler();
    ~ComposeReviewHandler();

    void ping() { cout << "ping(from server)" << endl; }

    void upload(const string &, const string &, const string &);

private:
    int n_review_store;
    int n_movie_db;
    int n_user_db;
    const int *review_store_ports;
    const int *movie_db_ports;
    const int *user_db_ports;

//    boost::shared_ptr<TTransport> compose_socket;
//    boost::shared_ptr<TTransport> compose_transport;
//    boost::shared_ptr<TProtocol> compose_protocol;
//    ComposeTweetClient *compose_client;
};

ComposeReviewHandler::ComposeReviewHandler() {
    this->n_review_store = 0;
    this->n_movie_db = 0;
    this->n_user_db = 0;
    this->review_store_ports = NULL;
    this->movie_db_ports = NULL;
    this->user_db_ports = NULL;
}

ComposeReviewHandler::ComposeReviewHandler(const int n_review_store, const int n_movie_db, const int n_user_db,
               const int* review_store_ports, const int* movie_db_ports, const int* user_db_ports) {
    this->n_review_store = n_review_store;
    this->n_movie_db = n_movie_db;
    this->n_user_db = n_user_db;
    this->review_store_ports = review_store_ports;
    this->movie_db_ports = movie_db_ports;
    this->user_db_ports = user_db_ports;

//    try {
//        compose_socket = (boost::shared_ptr<TTransport>) new TSocket("localhost", 9090);
//        compose_transport = (boost::shared_ptr<TTransport>)new TBufferedTransport(compose_socket);
//        compose_protocol = (boost::shared_ptr<TProtocol>)new TBinaryProtocol(compose_transport);
//        compose_client = new ComposeTweetClient(compose_protocol);
//    } catch (TException& tx) {
//        cout << "ERROR: " << tx.what() << endl;
//    }
}


ComposeReviewHandler::~ComposeReviewHandler() {
//    delete compose_client;
}

void ComposeReviewHandler::upload(const string& req_id, const string& type, const string& data) {
    if (IF_TRACE)
        logger(req_id, "ComposeReview", "compose_" + type, "begin");

    auto it = std::find_if (review_buf.begin(), review_buf.end(), [&req_id](Review& r){return r.req_id == req_id;});
    if (it != review_buf.end()) {
        if (type == "text") {
            it->text = data;
        } else if (type == "unique_id") {
            it->unique_id = data;
        } else if (type == "movie_id") {
            it->movie_id = data;
        } else if (type == "rating") {
            it->rating = data;
        }

        if (it->text != "" && it->unique_id != "" && it->movie_id != "" && it->rating != "" && it->user_id != "") {
            cout << req_id << " " << "complete!!" << endl;
            try {

            } catch (TException &tx) {
                cout << "ERROR: " << tx.what() << endl;
            }
            swap(*it, review_buf.back());
            review_buf.pop_back();
        }

    } else {

        Review new_review;
        new_review.req_id = req_id;
        vector<string> results;
        boost::split(results, req_id, [](char c){return c == ' ';});
        new_review.user_id = results.front();

        if (type == "text") {
            new_review.text = data;
        } else if (type == "unique_id") {
            new_review.unique_id = data;
        } else if (type == "movie_id") {
            new_review.movie_id = data;
        } else if (type == "rating") {
            new_review.rating = data;
        }
        review_buf.push_back(new_review);
    }



    if (IF_TRACE)
        logger(req_id, "ComposeReview", "compose_" + type, "end");
}


int main() {
    IF_TRACE = true;
    LOG_PATH = "../logs/ComposeReview";

    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    TSimpleServer server(
            boost::make_shared<ComposeReviewProcessor>(boost::make_shared<ComposeReviewHandler>()),
            boost::make_shared<TServerSocket>(9090),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}