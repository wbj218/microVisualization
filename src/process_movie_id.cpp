//
// Created by Yu Gan on 8/2/17.
//


#include "utils.h"


using namespace NetflixMicroservices;

json logs;

bool IF_TRACE;
string LOG_PATH;
mutex log_lock;

ServerInfo process_movie_id_server;
ServerInfo compose_review_server;



void exit_handler(int sig) {
    ofstream log_file;
    log_file.open(LOG_PATH);
    log_file << logs;
    log_file.close();
}

class ProcessMovieIDHandler: public ProcessMovieIDIf {
public:
    ProcessMovieIDHandler();
    ~ProcessMovieIDHandler() override;
    void ping() override;
    void process_movie_id(const string &, const string &, const string &) override;
    void get_movie_id(string &, const string &, const string &) override;

private:
    boost::shared_ptr<TTransport> compose_socket;
    boost::shared_ptr<TTransport> compose_transport;
    boost::shared_ptr<TProtocol> compose_protocol;
    ComposeReviewClient *compose_client;
};

ProcessMovieIDHandler::ProcessMovieIDHandler() {
    try {
        compose_socket = (boost::shared_ptr<TTransport>)
                new TSocket(compose_review_server.address, compose_review_server.port);
        compose_transport = (boost::shared_ptr<TTransport>)new TBufferedTransport(compose_socket);
        compose_protocol = (boost::shared_ptr<TProtocol>)new TBinaryProtocol(compose_transport);
        compose_client = new ComposeReviewClient(compose_protocol);
    } catch (TException& tx) {
        cout << "ERROR: " << tx.what() << endl;
    }
}


ProcessMovieIDHandler::~ProcessMovieIDHandler() = default;

void ProcessMovieIDHandler::process_movie_id(const string &req_id, const string &user_id, const string &url) {
    if (IF_TRACE)
        logger(req_id, "ProcessMovieID", "process_movie_id", "begin", logs, log_lock);

    string str_match = "www.imdb.com/title/";
    size_t found = url.find(str_match);
    assert(found!=string::npos);
    string _return = url.substr(found + str_match.length(), string::npos);
    try {
        compose_transport->open();
        compose_client->upload(req_id, user_id, "movie_id", _return);
        compose_transport->close();
    } catch (TException& tx) {
        cout << "ERROR: " << tx.what() << endl;
    }
    if (IF_TRACE)
        logger(req_id, "ProcessMovieID", "process_movie_id", "end", logs, log_lock);
}

void ProcessMovieIDHandler::get_movie_id(string& _return, const string& req_id, const string& url) {
    if (IF_TRACE)
        logger(req_id, "ProcessMovieID", "get_movie_id", "begin", logs, log_lock);

    string str_match = "www.imdb.com/title/";
    size_t found = url.find(str_match);
    assert(found!=string::npos);
    _return = url.substr(found + str_match.length(), string::npos);

    if (IF_TRACE)
        logger(req_id, "ProcessMovieID", "get_movie_id", "end", logs, log_lock);
}

void ProcessMovieIDHandler::ping () { cout << "ping(from server)" << endl; }


int main() {
    IF_TRACE = true;
    LOG_PATH = LOG_DIR_PATH + "ProcessMovieID.log";

    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    json config;
    config = load_config_file(CONFIG_FILE);
    process_movie_id_server = load_server_config("process_movie_id_server", config);
    compose_review_server = load_server_config("compose_review_server", config);


    TSimpleServer server(
            boost::make_shared<ProcessMovieIDProcessor>(boost::make_shared<ProcessMovieIDHandler>()),
            boost::make_shared<TServerSocket>(process_movie_id_server.port),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>()
    );

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}