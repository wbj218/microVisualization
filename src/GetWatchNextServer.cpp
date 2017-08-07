//
// Created by Yu Gan on 8/6/17.
//
#include "netflix_microservices.h"
#include "../gen-cpp/GetWatchNext.h"
#include <fstream>
#include <sstream>
#include <boost/algorithm/string/split.hpp>

#define RECOMMEND_FILE "../../recommendations/part-m-00000"
#define NUM_RECOMMENDATIONS 5

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
    GetWatchNextHandler();
    
    ~GetWatchNextHandler();

    void ping() { cout << "ping(from server)" << endl; }

    void get_watch_next(vector<string> & _return, const string& req_id, const string& user_id);
private:


};

GetWatchNextHandler::GetWatchNextHandler() {

}

GetWatchNextHandler::~GetWatchNextHandler() {

}

void GetWatchNextHandler::get_watch_next(vector<string> & _return, const string& req_id, const string& user_id) {
    if (IF_TRACE)
        logger(req_id, "GetWatchNext", "get_watch_next",  "begin");


    string str_match = "user_";

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
                    _return.push_back(recommendation_content[0]);
                }

            }
            line_counter++;
        }
        file.close();
    }

    if (IF_TRACE)
        logger(req_id, "GetWatchNext", "get_watch_next",  "end");
}

int main() {
    IF_TRACE = true;
    LOG_PATH = "../logs/GetWatchNext.log";

    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    TSimpleServer server(
            boost::make_shared<GetWatchNextProcessor>(boost::make_shared<GetWatchNextHandler>()),
            boost::make_shared<TServerSocket>(10047),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}

