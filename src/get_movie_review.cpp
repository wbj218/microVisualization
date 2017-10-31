//
// Created by Yu Gan on 8/6/17.
//

#include "utils.h"



//#define REVIEW_STORAGE_PORT 10000
//#define MOVIE_REVIEW_DB_PORT 10010
//#define COMPOSE_PAGE_PORT 10050

using namespace NetflixMicroservices;

json logs;
bool IF_TRACE;
string LOG_PATH = "../logs/";

std::mutex log_lock;

ServerInfo review_store_server;
ServerInfo movie_db_server;
ServerInfo compose_page_server;
ServerInfo get_movie_review_server;



void exit_handler(int sig) {
    ofstream log_file;
    log_file.open(LOG_PATH);
    log_file << logs;
    log_file.close();
}

class GetMovieReviewHandler: public GetMovieReviewIf {
public:
    GetMovieReviewHandler();

    ~GetMovieReviewHandler();

    void ping() { cout << "ping(from server)" << endl; }

    void get_movie_review(const string &req_id, const string &movie_id, int32_t begin_no, int32_t num, int32_t server_no);

private:

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

GetMovieReviewHandler::GetMovieReviewHandler() {

    try {
        store_socket = new boost::shared_ptr<TTransport>[review_store_server.num];
        store_transport = new boost::shared_ptr<TTransport>[review_store_server.num];
        store_protocol = new boost::shared_ptr<TProtocol>[review_store_server.num];
        store_client = new boost::shared_ptr<ReviewStorageClient>[review_store_server.num];

        movie_db_socket = new boost::shared_ptr<TTransport>[movie_db_server.num];
        movie_db_transport = new boost::shared_ptr<TTransport>[movie_db_server.num];
        movie_db_protocol = new boost::shared_ptr<TProtocol>[movie_db_server.num];
        movie_db_client = new boost::shared_ptr<MovieReviewDBClient>[movie_db_server.num];

        compose_page_socket = new boost::shared_ptr<TTransport>[compose_page_server.num];
        compose_page_transport = new boost::shared_ptr<TTransport>[compose_page_server.num];
        compose_page_protocol = new boost::shared_ptr<TProtocol>[compose_page_server.num];
        compose_page_client = new boost::shared_ptr<ComposePageClient>[compose_page_server.num];

        for (int i = 0; i < review_store_server.num; i++) {
            store_socket[i] = (boost::shared_ptr<TTransport>) new TSocket("localhost", review_store_server.port + i);
            store_transport[i] = (boost::shared_ptr<TTransport>) new TBufferedTransport(store_socket[i]);
            store_protocol[i] = (boost::shared_ptr<TProtocol>) new TBinaryProtocol(store_transport[i]);
            store_client[i] = (boost::shared_ptr<ReviewStorageClient>) new ReviewStorageClient(store_protocol[i]);
        }

        for (int i = 0; i < movie_db_server.num; i++) {
            movie_db_socket[i] = (boost::shared_ptr<TTransport>) new TSocket("localhost", movie_db_server.port + i);
            movie_db_transport[i] = (boost::shared_ptr<TTransport>) new TBufferedTransport(movie_db_socket[i]);
            movie_db_protocol[i] = (boost::shared_ptr<TProtocol>) new TBinaryProtocol(movie_db_transport[i]);
            movie_db_client[i] = (boost::shared_ptr<MovieReviewDBClient>) new MovieReviewDBClient(movie_db_protocol[i]);
        }

        for (int i = 0; i < compose_page_server.num; i++) {
            compose_page_socket[i] = (boost::shared_ptr<TTransport>) new TSocket("localhost", compose_page_server.port + i);
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
                                             const int32_t num,
                                             const int32_t server_no) {
    if (IF_TRACE)
        logger(req_id, "GetMovieReview", "get_movie_review", "begin", logs, log_lock);
    int store_index;
    int movie_db_index;
    
    string movie_reviews;
    vector<Review> review_list;

    movie_db_index = rand() % movie_db_server.num;
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
        store_index = rand() % review_store_server.num;
        try {
            store_transport[store_index]->open();
            store_client[store_index]->get_review(new_review, req_id, movie_id, unique_id);
            store_transport[store_index]->close();
        } catch (TException& tx) {
            cout << "ERROR: " << tx.what() << endl;
        }
        review_list.push_back(new_review);
    }

    
    try {
        compose_page_transport[server_no]->open();
        compose_page_client[server_no]->upload_movie_review(req_id, movie_id, review_list);
        compose_page_transport[server_no]->close();
    } catch (TException &tx) {
        cout << "ERROR: " << tx.what() << endl;
    }

    if (IF_TRACE)
        logger(req_id, "GetMovieReview", "get_movie_review", "end", logs, log_lock);

}

class GetMovieReviewCloneFactory: virtual public GetMovieReviewIfFactory {
public:
    virtual ~GetMovieReviewCloneFactory() {}
    GetMovieReviewCloneFactory() {

    }

    virtual GetMovieReviewIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) override {
        boost::shared_ptr<TSocket> sock = boost::dynamic_pointer_cast<TSocket>(connInfo.transport);
        return new GetMovieReviewHandler();
    }

    void releaseHandler(GetMovieReviewIf* handler) override {
        delete handler;
    }

private:
    int n_review_store;
    int n_movie_db;
    int n_compose_page;

};



int main(int argc, char *argv[]) {
    IF_TRACE = true;
    LOG_PATH = LOG_DIR_PATH + "GetMovieReview.log";


    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    json config;
    config = load_config_file(CONFIG_FILE);
    review_store_server = load_server_config("review_store_server", config);
    movie_db_server = load_server_config("movie_db_server", config);
    get_movie_review_server = load_server_config("get_movie_review_server", config);
    compose_page_server = load_server_config("compose_page_server", config);


    TThreadedServer server(
            boost::make_shared<GetMovieReviewProcessorFactory>
                    (boost::make_shared<GetMovieReviewCloneFactory>()),
            boost::make_shared<TServerSocket>(get_movie_review_server.port),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());

    // TSimpleServer server(
    //         boost::make_shared<GetMovieReviewProcessor>(boost::make_shared<GetMovieReviewHandler>(n_review_store, n_movie_db, n_compose_page)),
    //         boost::make_shared<TServerSocket>(10046),
    //         boost::make_shared<TBufferedTransportFactory>(),
    //         boost::make_shared<TBinaryProtocolFactory>());

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}


