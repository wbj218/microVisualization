
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/server/TNonblockingServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TSocket.h>

#include <thrift/transport/TTransportUtils.h>
#include <thrift/TToString.h>

#include <boost/make_shared.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <string>

#include "../gen-cpp/BlockStats.h"
#include "json.hpp"
#include <signal.h>
#include <sys/time.h>
#include <fstream>



#define USER_GRAPH_PATH "../user_graph/user_graph.json"
#define LOG_PATH "../logs/BlockStats.log"


using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;
using namespace TwitterMicroservices;
using json = nlohmann::json;

json logs;

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

class BlockStatsHandler: public BlockStatsIf {
public:
    BlockStatsHandler();

    ~BlockStatsHandler();

    void ping() { cout << "ping(from server)" << endl; }

    bool if_blocked(const string &, const string &, const string &);

    void add_block(const string &, const string &);

    void remove_block(const string &, const string &);





private:
    json user_graph;


};


BlockStatsHandler::BlockStatsHandler() {
    ifstream json_file;
    json_file.open(USER_GRAPH_PATH);
    assert(json_file.is_open());
    json_file>>user_graph;
    json_file.close();
}

BlockStatsHandler::~BlockStatsHandler() {


}

bool BlockStatsHandler::if_blocked(const string &user1_id, const string &user2_id, const string &log_id) {
    logger(log_id, "BlockStats", "if_blocked", "begin");
    bool block_flag = false;
    for (json::iterator it = user_graph[user1_id]["blocked_users"].begin();
         it != user_graph[user1_id]["blocked_users"].end(); ++it) {
        if (*it == user2_id) {
            block_flag = true;
            break;
        }
    }
    logger(log_id, "BlockStats", "if_blocked", "end");
    return block_flag;
}

void BlockStatsHandler::add_block(const string &user1_id, const string &user2_id) {
    bool block_flag = false;
    for (json::iterator it = user_graph[user1_id]["blocked_users"].begin();
         it != user_graph[user1_id]["blocked_users"].end(); ++it) {
        if (*it == user2_id) {
            block_flag = true;
            break;
        }
    }

    if (!block_flag) {
        if (user_graph[user1_id]["blocked_users"].is_null())
            user_graph[user1_id]["blocked_users"] = {user2_id};
        else
            user_graph[user1_id]["blocked_users"].insert(user_graph[user1_id]["blocked_users"].end(), user2_id);

        ofstream json_file;
        json_file.open(USER_GRAPH_PATH);
        assert(json_file.is_open());
        json_file<<user_graph;
        json_file.close();
    }
}

void BlockStatsHandler::remove_block(const string &user1_id, const string &user2_id) {
    for (json::iterator it = user_graph[user1_id]["blocked_users"].begin();
         it != user_graph[user1_id]["blocked_users"].end(); ++it) {
        if (*it == user2_id) {
            user_graph[user1_id]["blocked_users"].erase(it);

            ofstream json_file;
            json_file.open(USER_GRAPH_PATH);
            assert(json_file.is_open());
            json_file<<user_graph;
            json_file.close();

            break;
        }
    }
}




int main() {
//    boost::shared_ptr<TProcessor> processor(new WriteUserGraphProcessor(boost::make_shared<WriteUserGraphHandler>()));
//    boost::shared_ptr<TProtocolFactory> protocol_factory(new TBinaryProtocolFactory());
//    boost::shared_ptr<ThreadManager> thread_manager = ThreadManager::newSimpleThreadManager(4);
//    boost::shared_ptr<PosixThreadFactory> thread_factory = boost::shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
//    thread_manager->threadFactory(thread_factory);
//    thread_manager->start();
//    TNonblockingServer server(processor, protocol_factory, 9099, thread_manager);
    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    TSimpleServer server(
            boost::make_shared<BlockStatsProcessor>(boost::make_shared<BlockStatsHandler>()),
            boost::make_shared<TServerSocket>(9111),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}


