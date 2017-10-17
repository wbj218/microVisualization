//
// Created by Yu Gan on 8/2/17.
//

#include "utils.h"


using namespace NetflixMicroservices;

json logs;

bool IF_TRACE;
string LOG_PATH;


void exit_handler(int sig) {
    ofstream log_file;
    log_file.open(LOG_PATH);
    log_file << logs;
    log_file.close();
}

class ProcessTextHandler: public ProcessTextIf {
public:
    ProcessTextHandler();

    ~ProcessTextHandler();

    void ping() { cout << "ping(from server)" << endl; }

    void process_text(const string &, const string &, const string &);

private:
    boost::shared_ptr<TTransport> compose_socket;
    boost::shared_ptr<TTransport> compose_transport;
    boost::shared_ptr<TProtocol> compose_protocol;
    ComposeReviewClient *compose_client;
};

ProcessTextHandler::ProcessTextHandler() {
    try {
        compose_socket = (boost::shared_ptr<TTransport>) new TSocket("localhost", COMPOSE_REVIEW_PORT);
        compose_transport = (boost::shared_ptr<TTransport>)new TBufferedTransport(compose_socket);
        compose_protocol = (boost::shared_ptr<TProtocol>)new TBinaryProtocol(compose_transport);
        compose_client = new ComposeReviewClient(compose_protocol);
    } catch (TException& tx) {
        cout << "ERROR: " << tx.what() << endl;
    }
}


ProcessTextHandler::~ProcessTextHandler() {
//    delete compose_client;
}

void ProcessTextHandler::process_text(const string& req_id, const string &user_id, const string& text_data) {
    if (IF_TRACE)
        logger(req_id, "ProcessText", "process_text", "begin");
    try {
        compose_transport->open();
        compose_client->upload(req_id, user_id, "text", text_data);
        compose_transport->close();
    } catch (TException& tx) {
        cout << "ERROR: " << tx.what() << endl;
    }
    if (IF_TRACE)
        logger(req_id, "ProcessText", "process_text", "end");
}


int main() {
    IF_TRACE = true;
    LOG_PATH = LOG_DIR_PATH + "ProcessText.log";

    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    TSimpleServer server(
            boost::make_shared<ProcessTextProcessor>(boost::make_shared<ProcessTextHandler>()),
            boost::make_shared<TServerSocket>(TEXT_PORT),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}