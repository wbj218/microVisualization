//
// Created by Yu Gan on 8/2/17.
//

#include "netflix_microservices.h"
#include "../gen-cpp/ComposeReview.h"
#include "../gen-cpp/ReviewStorage.h"
#include "../gen-cpp/MovieReviewDB.h"
#include "../gen-cpp/UserReviewDB.h"
#include <boost/algorithm/string.hpp>
#include <vector>
#include <random>

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
    ComposeReviewHandler(const int n_review_store, const int n_movie_db, const int n_user_db);
    ComposeReviewHandler();
    ~ComposeReviewHandler();

    void ping() override { cout << "ping(from server)" << endl; }

    void upload(const string &, const string &, const string &, const string &) override;

private:
    int n_review_store;
    int n_movie_db;
    int n_user_db;


    boost::shared_ptr<TTransport>* store_socket{};
    boost::shared_ptr<TTransport>* store_transport{};
    boost::shared_ptr<TProtocol>* store_protocol{};
    boost::shared_ptr<ReviewStorageClient>* store_client{};

    boost::shared_ptr<TTransport>* movie_socket{};
    boost::shared_ptr<TTransport>* movie_transport{};
    boost::shared_ptr<TProtocol>* movie_protocol{};
    boost::shared_ptr<MovieReviewDBClient>* movie_client{};

    boost::shared_ptr<TTransport>* user_socket{};
    boost::shared_ptr<TTransport>* user_transport{};
    boost::shared_ptr<TProtocol>* user_protocol{};
    boost::shared_ptr<UserReviewDBClient>* user_client{};
};

//ComposeReviewHandler::ComposeReviewHandler() {
//    this->n_review_store = 0;
//    this->n_movie_db = 0;
//    this->n_user_db = 0;
//
//}

ComposeReviewHandler::ComposeReviewHandler(const int n_review_store, const int n_movie_db, const int n_user_db) {
    this->n_review_store = n_review_store;
    this->n_movie_db = n_movie_db;
    this->n_user_db = n_user_db;

    try {
        store_socket = new boost::shared_ptr<TTransport>[n_review_store];
        store_transport = new boost::shared_ptr<TTransport>[n_review_store];
        store_protocol = new boost::shared_ptr<TProtocol>[n_review_store];
        store_client = new boost::shared_ptr<ReviewStorageClient>[n_review_store];

        movie_socket = new boost::shared_ptr<TTransport>[n_movie_db];
        movie_transport = new boost::shared_ptr<TTransport>[n_movie_db];
        movie_protocol = new boost::shared_ptr<TProtocol>[n_movie_db];
        movie_client = new boost::shared_ptr<MovieReviewDBClient>[n_movie_db];

        user_socket = new boost::shared_ptr<TTransport>[n_user_db];
        user_transport = new boost::shared_ptr<TTransport>[n_user_db];
        user_protocol = new boost::shared_ptr<TProtocol>[n_user_db];
        user_client = new boost::shared_ptr<UserReviewDBClient>[n_user_db];

        for (int i = 0; i < n_review_store; i++) {
                store_socket[i] = (boost::shared_ptr<TTransport>) new TSocket("localhost", REVIEW_STORE_PORT_START + i);
                store_transport[i] = (boost::shared_ptr<TTransport>) new TBufferedTransport(store_socket[i]);
                store_protocol[i] = (boost::shared_ptr<TProtocol>) new TBinaryProtocol(store_transport[i]);
                store_client[i] = (boost::shared_ptr<ReviewStorageClient>) new ReviewStorageClient(store_protocol[i]);
        }

        for (int i = 0; i < n_movie_db; i++) {
            movie_socket[i] = (boost::shared_ptr<TTransport>) new TSocket("localhost", MOVIE_DB_PORT_START + i);
            movie_transport[i] = (boost::shared_ptr<TTransport>) new TBufferedTransport(movie_socket[i]);
            movie_protocol[i] = (boost::shared_ptr<TProtocol>) new TBinaryProtocol(movie_transport[i]);
            movie_client[i] = (boost::shared_ptr<MovieReviewDBClient>) new MovieReviewDBClient(movie_protocol[i]);
        }

        for (int i = 0; i < n_user_db; i++) {
            user_socket[i] = (boost::shared_ptr<TTransport>) new TSocket("localhost", USER_DB_PORT_START + i);
            user_transport[i] = (boost::shared_ptr<TTransport>) new TBufferedTransport(user_socket[i]);
            user_protocol[i] = (boost::shared_ptr<TProtocol>) new TBinaryProtocol(user_transport[i]);
            user_client[i] = (boost::shared_ptr<UserReviewDBClient>) new UserReviewDBClient(user_protocol[i]);
        }


    } catch (TException& tx) {
        cout << "ERROR: " << tx.what() << endl;
    }
}


ComposeReviewHandler::~ComposeReviewHandler() {
    delete[] store_socket;
    delete[] store_transport;
    delete[] store_protocol;
    delete[] store_client;

    delete[] movie_socket;
    delete[] movie_transport;
    delete[] movie_protocol;
    delete[] movie_client;

    delete[] user_socket;
    delete[] user_transport;
    delete[] user_protocol;
    delete[] user_client;
}

void ComposeReviewHandler::upload(const string &req_id, const string &user_id, const string &type, const string &data) {
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
                int store_index;
                int movie_index;
                int user_index;

                store_index = rand() % n_review_store;
                store_transport[store_index]->open();
                store_client[store_index]->review_storage(req_id, *it.base());
                store_transport[store_index]->close();

                movie_index = rand() % n_movie_db;
                movie_transport[movie_index]->open();
                movie_client[movie_index]->write_movie_review(req_id, it->movie_id, it->user_id, it->unique_id, it->rating);
                movie_transport[movie_index]->close();

                // cout<<"write_user_review"<<endl;

                user_index = rand() % n_user_db;
                user_transport[user_index]->open();
                user_client[user_index]->write_user_review(req_id, it->movie_id, it->user_id, it->unique_id);
                user_transport[user_index]->close();

            } catch (TException &tx) {
                cout << "ERROR: " << tx.what() << endl;
            }
            swap(*it, review_buf.back());
            review_buf.pop_back();
        }

    } else {

        Review new_review;
        new_review.req_id = req_id;        
        new_review.user_id = user_id;

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


int main(int argc, char *argv[]) {
    IF_TRACE = true;
    LOG_PATH = LOG_DIR_PATH + "ComposeReview.log";

    int n_store = stoi(argv[1]);
    int n_movie = stoi(argv[2]);
    int n_user = stoi(argv[3]);

    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    TSimpleServer server(
            boost::make_shared<ComposeReviewProcessor>(boost::make_shared<ComposeReviewHandler>(n_store, n_movie, n_user)),
            boost::make_shared<TServerSocket>(COMPOSE_REVIEW_PORT),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}