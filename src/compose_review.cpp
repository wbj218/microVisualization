//
// Created by Yu Gan on 8/2/17.
//

#include "utils.h"

using namespace NetflixMicroservices;

json logs;
bool IF_TRACE;
string LOG_PATH;
vector<Review> review_buf;
mutex log_lock;

ServerInfo compose_review_server;
ServerInfo review_store_server;
ServerInfo movie_db_server;
ServerInfo user_db_server;




void exit_handler(int sig) {
    ofstream log_file;
    log_file.open(LOG_PATH);
    log_file << logs;
    log_file.close();
}

class ComposeReviewHandler: public ComposeReviewIf {
public:
    ComposeReviewHandler();
    ~ComposeReviewHandler() override;
    void ping() override;
    void upload(const string &, const string &, const string &, const string &) override;

private:
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



ComposeReviewHandler::ComposeReviewHandler() {

    try {
        store_socket = new boost::shared_ptr<TTransport>[review_store_server.num];
        store_transport = new boost::shared_ptr<TTransport>[review_store_server.num];
        store_protocol = new boost::shared_ptr<TProtocol>[review_store_server.num];
        store_client = new boost::shared_ptr<ReviewStorageClient>[review_store_server.num];

        movie_socket = new boost::shared_ptr<TTransport>[movie_db_server.num];
        movie_transport = new boost::shared_ptr<TTransport>[movie_db_server.num];
        movie_protocol = new boost::shared_ptr<TProtocol>[movie_db_server.num];
        movie_client = new boost::shared_ptr<MovieReviewDBClient>[movie_db_server.num];

        user_socket = new boost::shared_ptr<TTransport>[user_db_server.num];
        user_transport = new boost::shared_ptr<TTransport>[user_db_server.num];
        user_protocol = new boost::shared_ptr<TProtocol>[user_db_server.num];
        user_client = new boost::shared_ptr<UserReviewDBClient>[user_db_server.num];

        for (int i = 0; i < review_store_server.num; i++) {
                store_socket[i] = (boost::shared_ptr<TTransport>)
                        new TSocket(review_store_server.address, review_store_server.port + i);
                store_transport[i] = (boost::shared_ptr<TTransport>)
                        new TBufferedTransport(store_socket[i]);
                store_protocol[i] = (boost::shared_ptr<TProtocol>)
                        new TBinaryProtocol(store_transport[i]);
                store_client[i] = (boost::shared_ptr<ReviewStorageClient>)
                        new ReviewStorageClient(store_protocol[i]);
        }

        for (int i = 0; i < movie_db_server.num; i++) {
            movie_socket[i] = (boost::shared_ptr<TTransport>)
                    new TSocket(movie_db_server.address, movie_db_server.port + i);
            movie_transport[i] = (boost::shared_ptr<TTransport>)
                    new TBufferedTransport(movie_socket[i]);
            movie_protocol[i] = (boost::shared_ptr<TProtocol>)
                    new TBinaryProtocol(movie_transport[i]);
            movie_client[i] = (boost::shared_ptr<MovieReviewDBClient>)
                    new MovieReviewDBClient(movie_protocol[i]);
        }

        for (int i = 0; i < user_db_server.num; i++) {
            user_socket[i] = (boost::shared_ptr<TTransport>)
                    new TSocket(user_db_server.address, user_db_server.port + i);
            user_transport[i] = (boost::shared_ptr<TTransport>)
                    new TBufferedTransport(user_socket[i]);
            user_protocol[i] = (boost::shared_ptr<TProtocol>)
                    new TBinaryProtocol(user_transport[i]);
            user_client[i] = (boost::shared_ptr<UserReviewDBClient>)
                    new UserReviewDBClient(user_protocol[i]);
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
        logger(req_id, "ComposeReview", "compose_" + type, "begin", logs, log_lock);

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

        if (!it->text.empty() && !it->unique_id.empty() && !it->movie_id.empty() &&
                !it->rating.empty() && !it->user_id.empty()) {
            cout << req_id << " " << "complete!!" << endl;
            try {
                int store_index;
                int movie_index;
                int user_index;

                store_index = rand() % review_store_server.num;
                store_transport[store_index]->open();
                store_client[store_index]->review_storage(req_id, *it.base());
                store_transport[store_index]->close();

                movie_index = rand() % movie_db_server.num;
                movie_transport[movie_index]->open();
                movie_client[movie_index]->write_movie_review(req_id, it->movie_id, it->user_id,
                                                              it->unique_id, it->rating);
                movie_transport[movie_index]->close();

                user_index = rand() % user_db_server.num;
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
        logger(req_id, "ComposeReview", "compose_" + type, "end", logs, log_lock);
}

void ComposeReviewHandler::ping () { cout << "ping(from server)" << endl; }


int main(int argc, char *argv[]) {
    IF_TRACE = true;
    LOG_PATH = LOG_DIR_PATH + "ComposeReview.log";

    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    json config;
    config = load_config_file(CONFIG_FILE);
    compose_review_server = load_server_config("compose_review_server", config);
    review_store_server = load_server_config("review_store_server", config);
    user_db_server = load_server_config("user_db_server", config);
    movie_db_server = load_server_config("movie_db_server", config);

    TSimpleServer server(
            boost::make_shared<ComposeReviewProcessor>(boost::make_shared<ComposeReviewHandler>()),
            boost::make_shared<TServerSocket>(stoi(argv[1]) + compose_review_server.port),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}