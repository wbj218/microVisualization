//
// Created by Yu Gan on 7/19/17.
//

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

#include <xapian.h>
#include "../gen-cpp/Xapian.h"

#include "json.hpp"
#include <signal.h>
#include <sys/time.h>
#include <fstream>


#define LOG_PATH "../logs/Xapian.log"
#define IF_TRACE true


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

class XapianHandler: public XapianIf {
public:
    XapianHandler();

    ~XapianHandler();

    void ping() { cout << "ping(from server)" << endl; }

    void search(string &, const string &, const string &);

private:
    string db_path = "/home/yg397/Research/twitter_microservices/TwitterMicroservices_2/xapian/db";
    Xapian::Database *db;
    Xapian::Enquire *enquire;
    Xapian::QueryParser *qp;
    Xapian::Stem *stemmer;

};

XapianHandler::XapianHandler() {
    db = new Xapian::Database(db_path);
    enquire = new Xapian::Enquire(*db);
    qp = new Xapian::QueryParser;
    stemmer = new Xapian::Stem("english");
    qp->set_stemmer(*stemmer);
    qp->set_database(*db);
    qp->set_stemming_strategy(Xapian::QueryParser::STEM_SOME);
}

XapianHandler::~XapianHandler() {
    delete stemmer;
    delete qp;
    delete enquire;
    delete db;
}

void XapianHandler::search(string &_return, const string &query_string, const string &req_id) {
    if (IF_TRACE)
        logger(req_id, "Xapian", "search",  "begin");

    Xapian::Query query = qp->parse_query(query_string);
    cout << "Parsed query is: " << query.get_description() << endl;
    enquire->set_query(query);
    Xapian::MSet matches = enquire->get_mset(0, 10);
    cout << matches.get_matches_estimated() << " results found.\n";
    cout << "Matches 1-" << matches.size() << ":\n" << endl;

    for (Xapian::MSetIterator i = matches.begin(); i != matches.end(); ++i) {
        // cout << i.get_rank() + 1 << ": " << i.get_weight() << " docid=" << *i
        //      << " [" << i.get_document().get_data() << "]\n\n";
        _return += i.get_document().get_data();
    }

    if (IF_TRACE)
        logger(req_id, "Xapian", "search",  "end");

}

int main() {

//    boost::shared_ptr<TProcessor> processor(new XapianProcessor(boost::make_shared<XapianHandler>()));
//    boost::shared_ptr<TProtocolFactory> protocol_factory(new TBinaryProtocolFactory());
//    boost::shared_ptr<ThreadManager> thread_manager = ThreadManager::newSimpleThreadManager(4);
//    boost::shared_ptr<PosixThreadFactory> thread_factory = boost::shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
//    thread_manager->threadFactory(thread_factory);
//    thread_manager->start();
//    TNonblockingServer server(processor, protocol_factory, 9101, thread_manager);

    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);
    
	TSimpleServer server(
		boost::make_shared<XapianProcessor>(boost::make_shared<XapianHandler>()),
		boost::make_shared<TServerSocket>(9101),
		boost::make_shared<TBufferedTransportFactory>(),
		boost::make_shared<TBinaryProtocolFactory>());

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}






