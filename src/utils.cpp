//
// Created by yugan on 10/17/17.
//
#include "utils.h"

using namespace std;

void logger(const string &log_id, const string &service, const string &function, const string &state,
            mutex &mutex, json &logs) {
    struct timeval tv{};
    gettimeofday(&tv, nullptr);
    long time_in_us = tv.tv_sec * 1000000 + tv.tv_usec;
    mutex.lock();
    logs[log_id][service][function][state] = time_in_us;
    mutex.unlock();
}



