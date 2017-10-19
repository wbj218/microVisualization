//
// Created by yugan on 10/17/17.
//
#include "utils.h"

using namespace std;

void logger(const string &log_id, const string &service, const string &function, const string &state,
            json &logs, mutex &mutex) {
    struct timeval tv{};
    gettimeofday(&tv, nullptr);
    long time_in_us = tv.tv_sec * 1000000 + tv.tv_usec;
    mutex.lock();
    logs[log_id][service][function][state] = time_in_us;
    mutex.unlock();
}

json load_config_file(const char* file_name) {
    ifstream json_file;
    json config;
    json_file.open(file_name);
    assert(json_file.is_open());
    json_file>>config;
    json_file.close();
    return config;
}

ServerInfo load_server_config(const string &server_name, const json &config) {
    ServerInfo new_server;
    if (config.find(server_name) != config.end()) {
        new_server.num = config[server_name]["num"];
        new_server.address = config[server_name]["address"];
        new_server.port = config[server_name]["port"];
    }
    else {
        cerr<<"Server name mismatch!"<<endl;
        new_server.num = 0;
        new_server.address = "NULL";
        new_server.port = 0;
    }
    return new_server;
}

int load_num_user(const json &config) {
    return config["num_user"];
}

int load_num_movie(const json &config) {
    return config["num_movie"];
}


