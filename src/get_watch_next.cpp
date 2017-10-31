//
// Created by Yu Gan on 8/6/17.
//
#include "utils.h"


#define RECOMMEND_FILE "../../recommendations/part-m-00000"
#define NUM_RECOMMENDATIONS 5
//#define COMPOSE_PAGE_PORT 10050

using namespace NetflixMicroservices;

json logs;

bool IF_TRACE;
string LOG_PATH;
mutex log_lock;

ServerInfo compose_page_server;
ServerInfo get_watch_next_server;


void exit_handler(int sig) {
    ofstream log_file;
    log_file.open(LOG_PATH);
    log_file << logs;
    log_file.close();
}

class GetWatchNextHandler: public GetWatchNextIf {
public:
    GetWatchNextHandler();
    
    ~GetWatchNextHandler() override;

    void ping() override;

    void get_watch_next(const string& req_id, const string& user_id, const int32_t server_no) override;
private:
    boost::shared_ptr<TTransport>* compose_page_socket;
    boost::shared_ptr<TTransport>* compose_page_transport;
    boost::shared_ptr<TProtocol>* compose_page_protocol;
    boost::shared_ptr<ComposePageClient>* compose_page_client;
};

GetWatchNextHandler::GetWatchNextHandler() {

    try {
        compose_page_socket = new boost::shared_ptr<TTransport>[compose_page_server.num];
        compose_page_transport = new boost::shared_ptr<TTransport>[compose_page_server.num];
        compose_page_protocol = new boost::shared_ptr<TProtocol>[compose_page_server.num];
        compose_page_client = new boost::shared_ptr<ComposePageClient>[compose_page_server.num];

        for (int i = 0; i < compose_page_server.num; i++) {
            compose_page_socket[i] = (boost::shared_ptr<TTransport>) new TSocket(compose_page_server.address, compose_page_server.port + i);
            compose_page_transport[i] = (boost::shared_ptr<TTransport>) new TBufferedTransport(compose_page_socket[i]);
            compose_page_protocol[i] = (boost::shared_ptr<TProtocol>) new TBinaryProtocol(compose_page_transport[i]);
            compose_page_client[i] = (boost::shared_ptr<ComposePageClient>) new ComposePageClient(compose_page_protocol[i]);
        }

    } catch (TException& tx) {
        cout << "ERROR: " << tx.what() << endl;
    }
}

GetWatchNextHandler::~GetWatchNextHandler() {
    delete[] compose_page_socket;
    delete[] compose_page_transport;
    delete[] compose_page_protocol;
    delete[] compose_page_client;
}

void GetWatchNextHandler::get_watch_next(const string& req_id, const string& user_id, const int32_t server_no) {
    if (IF_TRACE)
        logger(req_id, "GetWatchNext", "get_watch_next",  "begin", logs, log_lock);


    string str_match = "user_";
    vector<string> watch_next;
    
    int user_index = stoi(user_id.substr(str_match.length(), string::npos));

    ifstream file(RECOMMEND_FILE);
    if (file.is_open()) {
        string line;
        int line_counter = 0;


        vector<string> recommendations;
        vector<string> line_content;
        vector<string> recommendation_content;

        while (! file.eof() )
        {
            getline (file, line);
            if (line_counter == user_index) {
                boost::algorithm::split(line_content, line, [](char c){return c == '\t';});
                line_content[1] = line_content[1].substr(1, string::npos);
                boost::algorithm::split(recommendations, line_content[1], [](char c){return c == ',';});
                for (int i = 0; i < NUM_RECOMMENDATIONS; i++) {
                    boost::algorithm::split(recommendation_content, recommendations[i], [](char c){return c == ':';});
                    watch_next.push_back(recommendation_content[0]);
                }

            }
            line_counter++;
        }
        file.close();
    }

    
    try {
        compose_page_transport[server_no]->open();
        compose_page_client[server_no]->upload_watch_next(req_id, user_id, watch_next);
        compose_page_transport[server_no]->close();
    } catch (TException &tx) {
        cout << "ERROR: " << tx.what() << endl;
    }

    if (IF_TRACE)
        logger(req_id, "GetWatchNext", "get_watch_next",  "end", logs, log_lock);
}

void GetWatchNextHandler::ping () { cout << "ping(from server)" << endl; }

int main(int argc, char *argv[]) {
    IF_TRACE = true;
    LOG_PATH = LOG_DIR_PATH + "GetWatchNext.log";



    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    json config;
    config = load_config_file(CONFIG_FILE);
    compose_page_server = load_server_config("compose_page_server", config);
    get_watch_next_server = load_server_config("get_watch_next_server", config);

    TSimpleServer server(
            boost::make_shared<GetWatchNextProcessor>(boost::make_shared<GetWatchNextHandler>()),
            boost::make_shared<TServerSocket>(get_watch_next_server.port),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}

