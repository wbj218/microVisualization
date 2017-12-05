//
// Created by Yu Gan on 8/3/17.
//

#include "utils.h"


using namespace NetflixMicroservices;

json logs;

bool IF_TRACE;
string LOG_PATH;
mutex log_lock;

ServerInfo process_unique_id_server;
ServerInfo compose_review_server;


void exit_handler(int sig) {
    ofstream log_file;
    log_file.open(LOG_PATH);
    log_file << logs;
    log_file.close();
}

class ProcessUniqueIDHandler: public ProcessUniqueIDIf {
public:
    ProcessUniqueIDHandler();
    ~ProcessUniqueIDHandler();
    void ping() {cout << "ping(from server)" << endl;}
    void get_unique_id(const string &, const string &);

private:
    default_random_engine generator;

    boost::shared_ptr<TTransport> compose_socket;
    boost::shared_ptr<TTransport> compose_transport;
    boost::shared_ptr<TProtocol> compose_protocol;
    ComposeReviewClient *compose_client;

};

ProcessUniqueIDHandler::ProcessUniqueIDHandler() {
    generator.seed((unsigned int) time(NULL));
    try {
        compose_socket = (boost::shared_ptr<TTransport>)
                new TSocket(compose_review_server.address, compose_review_server.port);
        compose_transport = (boost::shared_ptr<TTransport>) new TBufferedTransport(compose_socket);
        compose_protocol = (boost::shared_ptr<TProtocol>) new TBinaryProtocol(compose_transport);
        compose_client = new ComposeReviewClient(compose_protocol);
    } catch (TException& tx) {
        cout << "ERROR: " << tx.what() << endl;
    }
}

ProcessUniqueIDHandler::~ProcessUniqueIDHandler() {
//    delete compose_client;

}

void ProcessUniqueIDHandler::get_unique_id(const string &req_id, const string &user_id) {
    if (IF_TRACE)
        logger(req_id, "ProcessUniqueID", "process_unique_id", "begin", logs, log_lock);
    uniform_int_distribution<unsigned long long > distribution(0, ULLONG_MAX);

//    cout<<req_id<<" "<<to_string(distribution(generator))<<endl;
    try {
        compose_transport->open();
        // compose_client->upload(req_id, user_id, "unique_id", to_string(distribution(generator)));
        compose_client->upload(req_id, user_id, "unique_id", req_id);
        compose_transport->close();
    } catch (TException& tx) {
        cout << "ERROR: " << tx.what() << endl;
    }

    if (IF_TRACE)
        logger(req_id, "ProcessUniqueID", "process_unique_id", "end", logs, log_lock);
}

int main() {

    IF_TRACE = true;
    LOG_PATH = LOG_DIR_PATH + "ProcessUniqueID.log";

    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    json config;
    config = load_config_file(CONFIG_FILE);
    process_unique_id_server = load_server_config("process_unique_id_server", config);
    compose_review_server = load_server_config("compose_review_server", config);

    TSimpleServer server(
            boost::make_shared<ProcessUniqueIDProcessor>(boost::make_shared<ProcessUniqueIDHandler>()),
            boost::make_shared<TServerSocket>(process_unique_id_server.port),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}

