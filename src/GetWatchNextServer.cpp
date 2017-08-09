//
// Created by Yu Gan on 8/6/17.
//
#include "netflix_microservices.h"
#include "../gen-cpp/GetWatchNext.h"
#include "../gen-cpp/ComposePage.h"
#include <fstream>
#include <sstream>
#include <boost/algorithm/string/split.hpp>

#define RECOMMEND_FILE "../../recommendations/part-m-00000"
#define NUM_RECOMMENDATIONS 5
#define COMPOSE_PAGE_PORT 10050

using namespace NetflixMicroservices;

json logs;

bool IF_TRACE;
string LOG_PATH;

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

class GetWatchNextHandler: public GetWatchNextIf {
public:
    GetWatchNextHandler(const int n_compose_page);
    
    ~GetWatchNextHandler();

    void ping() { cout << "ping(from server)" << endl; }

    void get_watch_next(const string& req_id, const string& user_id);
private:
    int n_compose_page;
    boost::shared_ptr<TTransport>* compose_page_socket;
    boost::shared_ptr<TTransport>* compose_page_transport;
    boost::shared_ptr<TProtocol>* compose_page_protocol;
    boost::shared_ptr<ComposePageClient>* compose_page_client;


};

GetWatchNextHandler::GetWatchNextHandler(const int n_compose_page) {
    this->n_compose_page = n_compose_page;

    try {
        compose_page_socket = new boost::shared_ptr<TTransport>[n_compose_page];
        compose_page_transport = new boost::shared_ptr<TTransport>[n_compose_page];
        compose_page_protocol = new boost::shared_ptr<TProtocol>[n_compose_page];
        compose_page_client = new boost::shared_ptr<ComposePageClient>[n_compose_page];

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

GetWatchNextHandler::~GetWatchNextHandler() {
    delete[] compose_page_socket;
    delete[] compose_page_transport;
    delete[] compose_page_protocol;
    delete[] compose_page_client;
}

void GetWatchNextHandler::get_watch_next(const string& req_id, const string& user_id) {
    if (IF_TRACE)
        logger(req_id, "GetWatchNext", "get_watch_next",  "begin");


    string str_match = "user_";
    vector<string> watch_next;
    int compose_page_index;
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

    compose_page_index = rand() % n_compose_page;
    try {
        compose_page_transport[compose_page_index]->open();
        compose_page_client[compose_page_index]->upload_watch_next(req_id, user_id, watch_next);
        compose_page_transport[compose_page_index]->close();
    } catch (TException &tx) {
        cout << "ERROR: " << tx.what() << endl;
    }

    if (IF_TRACE)
        logger(req_id, "GetWatchNext", "get_watch_next",  "end");
}

int main(int argc, char *argv[]) {
    IF_TRACE = true;
    LOG_PATH = LOG_DIR_PATH + "GetWatchNext.log";

    int n_compose_page = stoi(argv[1]);

    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    TSimpleServer server(
            boost::make_shared<GetWatchNextProcessor>(boost::make_shared<GetWatchNextHandler>(n_compose_page)),
            boost::make_shared<TServerSocket>(10047),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}

