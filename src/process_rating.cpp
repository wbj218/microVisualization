//
// Created by Yu Gan on 8/2/17.
//

#include "utils.h"


using namespace NetflixMicroservices;

json logs;
string LOG_PATH;
bool IF_TRACE;
mutex log_lock;


ServerInfo process_rating_server;
ServerInfo compose_review_server;


void exit_handler(int sig) {
    ofstream log_file;
    log_file.open(LOG_PATH);
    log_file << logs;
    log_file.close();
}

class AssignRatingHandler: public AssignRatingIf {
public:
    AssignRatingHandler();
    ~AssignRatingHandler() override;
    void ping() override { cout << "ping(from server)" << endl; }
    void assign_rating(const string &, const string &, const string &) override;

private:
    boost::shared_ptr<TTransport> compose_socket;
    boost::shared_ptr<TTransport> compose_transport;
    boost::shared_ptr<TProtocol> compose_protocol;
    ComposeReviewClient *compose_client;
};

AssignRatingHandler::AssignRatingHandler() {
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


AssignRatingHandler::~AssignRatingHandler() = default;

void AssignRatingHandler::assign_rating(const string& req_id, const string & user_id, const string& rating) {
    if (IF_TRACE)
        logger(req_id, "AssignRating", "assign_rating", "begin", logs, log_lock);
    try {
        compose_transport->open();
        compose_client->upload(req_id, user_id, "rating", rating);
        compose_transport->close();
    } catch (TException& tx) {
        cout << "ERROR: " << tx.what() << endl;
    }
    if (IF_TRACE)
        logger(req_id, "AssignRating", "assign_rating", "end", logs, log_lock);
}


int main() {
    IF_TRACE = true;
    LOG_PATH = LOG_DIR_PATH + "AssignRating.log";

    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    json config;
    config = load_config_file(CONFIG_FILE);
    process_rating_server = load_server_config("process_rating_server", config);
    compose_review_server = load_server_config("compose_review_server", config);

    TSimpleServer server(
            boost::make_shared<AssignRatingProcessor>(boost::make_shared<AssignRatingHandler>()),
            boost::make_shared<TServerSocket>(process_rating_server.address.c_str(), process_rating_server.port),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}