//
// Created by Yu Gan on 8/6/17.
//

#include "netflix_microservices.h"
#include "../gen-cpp/GetMovieReview.h"
#include "../gen-cpp/MovieReviewDB.h"
#include "../gen-cpp/ReviewStorage.h"
#include "../gen-cpp/ComposePage.h"

#include <vector>
#include <random>
#include <sstream>
#include <boost/algorithm/string/split.hpp>

#define REVIEW_STORAGE_PORT 10000
#define MOVIE_REVIEW_DB_PORT 10010
#define COMPOSE_PAGE_PORT 10050

using namespace NetflixMicroservices;

json logs;
bool IF_TRACE;
string LOG_PATH = "../logs/";

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

class GetMovieReviewHandler: public GetMovieReviewIf {
public:
    GetMovieReviewHandler(const int n_review_store, const int n_movie_db, const int n_compose_page);

    ~GetMovieReviewHandler();

    void ping() { cout << "ping(from server)" << endl; }

    void get_movie_review(const string &req_id, const string &movie_id, const int32_t begin_no, const int32_t num);

private:
    int n_review_store;
    int n_movie_db;
    int n_compose_page;

    boost::shared_ptr<TTransport>* store_socket;
    boost::shared_ptr<TTransport>* store_transport;
    boost::shared_ptr<TProtocol>* store_protocol;
    boost::shared_ptr<ReviewStorageClient>* store_client;

    boost::shared_ptr<TTransport>* movie_db_socket;
    boost::shared_ptr<TTransport>* movie_db_transport;
    boost::shared_ptr<TProtocol>* movie_db_protocol;
    boost::shared_ptr<MovieReviewDBClient>* movie_db_client;

    boost::shared_ptr<TTransport>* compose_page_socket;
    boost::shared_ptr<TTransport>* compose_page_transport;
    boost::shared_ptr<TProtocol>* compose_page_protocol;
    boost::shared_ptr<ComposePageClient>* compose_page_client;
};

GetMovieReviewHandler::GetMovieReviewHandler(const int n_review_store, const int n_movie_db, const int n_compose_page) {
    this->n_review_store = n_review_store;
    this->n_movie_db = n_movie_db;
    this->n_compose_page = n_compose_page;

    try {
        store_socket = new boost::shared_ptr<TTransport>[n_review_store];
        store_transport = new boost::shared_ptr<TTransport>[n_review_store];
        store_protocol = new boost::shared_ptr<TProtocol>[n_review_store];
        store_client = new boost::shared_ptr<ReviewStorageClient>[n_review_store];

        movie_db_socket = new boost::shared_ptr<TTransport>[n_movie_db];
        movie_db_transport = new boost::shared_ptr<TTransport>[n_movie_db];
        movie_db_protocol = new boost::shared_ptr<TProtocol>[n_movie_db];
        movie_db_client = new boost::shared_ptr<MovieReviewDBClient>[n_movie_db];

        compose_page_socket = new boost::shared_ptr<TTransport>[n_compose_page];
        compose_page_transport = new boost::shared_ptr<TTransport>[n_compose_page];
        compose_page_protocol = new boost::shared_ptr<TProtocol>[n_compose_page];
        compose_page_client = new boost::shared_ptr<ComposePageClient>[n_compose_page];

        for (int i = 0; i < n_review_store; i++) {
            store_socket[i] = (boost::shared_ptr<TTransport>) new TSocket("localhost", REVIEW_STORAGE_PORT + i);
            store_transport[i] = (boost::shared_ptr<TTransport>) new TBufferedTransport(store_socket[i]);
            store_protocol[i] = (boost::shared_ptr<TProtocol>) new TBinaryProtocol(store_transport[i]);
            store_client[i] = (boost::shared_ptr<ReviewStorageClient>) new ReviewStorageClient(store_protocol[i]);
        }

        for (int i = 0; i < n_movie_db; i++) {
            movie_db_socket[i] = (boost::shared_ptr<TTransport>) new TSocket("localhost", MOVIE_REVIEW_DB_PORT + i);
            movie_db_transport[i] = (boost::shared_ptr<TTransport>) new TBufferedTransport(movie_db_socket[i]);
            movie_db_protocol[i] = (boost::shared_ptr<TProtocol>) new TBinaryProtocol(movie_db_transport[i]);
            movie_db_client[i] = (boost::shared_ptr<MovieReviewDBClient>) new MovieReviewDBClient(movie_db_protocol[i]);
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

GetMovieReviewHandler::~GetMovieReviewHandler() {
    delete[] store_socket;
    delete[] store_transport;
    delete[] store_protocol;
    delete[] store_client;

    delete[] movie_db_socket;
    delete[] movie_db_transport;
    delete[] movie_db_protocol;
    delete[] movie_db_client;

    delete[] compose_page_socket;
    delete[] compose_page_transport;
    delete[] compose_page_protocol;
    delete[] compose_page_client;
}

void GetMovieReviewHandler::get_movie_review(const string &req_id,
                                             const string &movie_id,
                                             const int32_t begin_no,
                                             const int32_t num) {
    if (IF_TRACE)
        logger(req_id, "GetMovieReview", "get_movie_review", "begin");
    int store_index;
    int movie_db_index;
    int compose_page_index;
    string movie_reviews;
    vector<Review> review_list;

    movie_db_index = rand() % n_movie_db;
    try {
        movie_db_transport[movie_db_index]->open();
        movie_db_client[movie_db_index]->get_movie_review(movie_reviews, req_id, movie_id, begin_no, num);
        movie_db_transport[movie_db_index]->close();
    } catch (TException& tx) {
        cout << "ERROR: " << tx.what() << endl;
    }

    istringstream ss(movie_reviews);
    string line;
    vector<string> elems;
    string unique_id;
    Review new_review;

    for (int i = 0; i < num; i++) {
        getline(ss, line);
        boost::algorithm::split(elems, line, [](char c){return c == ' ';});
        unique_id = elems[1];
        store_index = rand() % n_review_store;
        try {
            store_transport[store_index]->open();
            store_client[store_index]->get_review(new_review, req_id, movie_id, unique_id);
            store_transport[store_index]->close();
        } catch (TException& tx) {
            cout << "ERROR: " << tx.what() << endl;
        }
        review_list.push_back(new_review);
    }

    compose_page_index = rand() % n_compose_page;
    try {
        compose_page_transport[compose_page_index]->open();
        compose_page_client[compose_page_index]->upload_movie_review(req_id, movie_id, review_list);
        compose_page_transport[compose_page_index]->close();
    } catch (TException &tx) {
        cout << "ERROR: " << tx.what() << endl;
    }

    if (IF_TRACE)
        logger(req_id, "GetMovieReview", "get_movie_review", "end");

}

int main(int argc, char *argv[]) {
    IF_TRACE = true;
    LOG_PATH = LOG_DIR_PATH + "GetMovieReview.log";
    int n_review_store = stoi(argv[1]);
    int n_movie_db = stoi(argv[2]);
    int n_compose_page = stoi(argv[3]);

    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    TSimpleServer server(
            boost::make_shared<GetMovieReviewProcessor>(boost::make_shared<GetMovieReviewHandler>(n_review_store, n_movie_db, n_compose_page)),
            boost::make_shared<TServerSocket>(10046),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}


