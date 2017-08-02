


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

#include "json.hpp"
#include <signal.h>
#include <sys/time.h>
#include <fstream>

#define LOG_PATH "../logs/ComposeReply.log"
#define IF_TRACE true

#include <boost/make_shared.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "../gen-cpp/ComposeReply.h"
#include "../gen-cpp/MongoTweetStorage.h"
#include "../gen-cpp/WriteTimeline.h"
#include "../gen-cpp/BlockStats.h"


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


//mutex thread_mx;
vector<Tweet> tweet_buf;



class ComposeReplyHandler: public ComposeReplyIf {
public:
    ComposeReplyHandler();
    ~ComposeReplyHandler();
    void ping() {cout << "ping(from server)" << endl;}

    void upload(const string&, const string&, const string&);


private:


    boost::shared_ptr<TTransport> mongo_tweet_storage_socket;
    boost::shared_ptr<TTransport> mongo_tweet_storage_transport;
    boost::shared_ptr<TProtocol> mongo_tweet_storage_protocol;
    MongoTweetStorageClient *mongo_tweet_storage_client;



    boost::shared_ptr<TTransport> write_timeline_socket;
    boost::shared_ptr<TTransport> write_timeline_transport;
    boost::shared_ptr<TProtocol> write_timeline_protocol;
    WriteTimelineClient *write_timeline_client;

    boost::shared_ptr<TTransport> block_socket;
    boost::shared_ptr<TTransport> block_transport;
    boost::shared_ptr<TProtocol> block_protocol;
    BlockStatsClient *block_client;


};

ComposeReplyHandler::ComposeReplyHandler() {
    try {


        mongo_tweet_storage_socket = (boost::shared_ptr<TTransport>) new TSocket("localhost", 9098);
        mongo_tweet_storage_transport =
                (boost::shared_ptr<TTransport>) new TBufferedTransport(mongo_tweet_storage_socket);
        mongo_tweet_storage_protocol =
                (boost::shared_ptr<TProtocol>) new TBinaryProtocol(mongo_tweet_storage_transport);
        mongo_tweet_storage_client = new MongoTweetStorageClient(mongo_tweet_storage_protocol);


        write_timeline_socket = (boost::shared_ptr<TTransport>) new TSocket("localhost", 9100);
        write_timeline_transport = (boost::shared_ptr<TTransport>)new TBufferedTransport(write_timeline_socket);
        write_timeline_protocol = (boost::shared_ptr<TProtocol>)new TBinaryProtocol(write_timeline_transport);
        write_timeline_client = new WriteTimelineClient(write_timeline_protocol);

        block_socket = (boost::shared_ptr<TTransport>) new TSocket("localhost", 9111);
        block_transport = (boost::shared_ptr<TTransport>)new TBufferedTransport(block_socket);
        block_protocol = (boost::shared_ptr<TProtocol>)new TBinaryProtocol(block_transport);
        block_client = new BlockStatsClient(block_protocol);

    } catch (TException& tx) {
        cout << "ERROR: " << tx.what() << endl;
    }
}

ComposeReplyHandler::~ComposeReplyHandler() {
    delete mongo_tweet_storage_client;
    delete write_timeline_client;


}


void ComposeReplyHandler::upload(const string& req_id, const string& type, const string& data){
    if (IF_TRACE)
        logger(req_id, "ComposeReply", "compose_" + type,  "begin");

//    thread_mx.lock();
    // cout<<req_id<<" "<<type<<" "<<data<<endl;
    auto it = std::find_if (tweet_buf.begin(), tweet_buf.end(), [&req_id](Tweet& t){return t.req_id == req_id;});

    if (it != tweet_buf.end()){
        if (type == "text")
            it->text = data;
        else if (type == "image")
            it->image = data;
        else if (type == "video"){
            it->video = data;
//            cout<<"video received"<<endl;
        }

        else if (type == "tag")
            it->tags = data;
        else if (type == "url")
            it->urls = data;
        else if (type == "unique_id")
            it->unique_id = data;



        if (it->text != "" && it->image != "" && it->video != "" && it->unique_id != "" && it->tags != ""
            && it->urls != "") {

            cout<<"Reply"<<req_id<<" "<<"complete!!"<<endl;


            try {
                vector<string> results;
                boost::split(results, it->text, [](char c){return c == ' ';});
                string recipient_id;
                recipient_id = results.front();
                it->recipient_id = recipient_id;

                mongo_tweet_storage_transport->open();
                mongo_tweet_storage_client->store_tweet(*it.base(), req_id);
                mongo_tweet_storage_transport->close();


                write_timeline_transport->open();
                write_timeline_client->write_timeline(it->user_id, it->unique_id, it->req_id, "REPLY_SEND");
                write_timeline_client->write_timeline(it->recipient_id, it->unique_id, it->req_id, "REPLY_RECV");
                istringstream iss(it->tags);
                string line;
                while (std::getline(iss, line))
                {
                    boost::split(results, line, [](char c){return c == ' ';});
                    if (results.front()!= "NULL" ) {
                        bool if_blocked;
                        block_transport->open();
                        if_blocked = block_client->if_blocked(results.front(), it->user_id, req_id);
                        block_transport->close();
                        if (!if_blocked)
                            write_timeline_client->write_timeline(results.front(), it->unique_id, it->req_id, "REPLY_TAGGED");

                    }
                }
                write_timeline_transport->close();


            } catch (TException& tx) {
                cout << "ERROR: " << tx.what() << endl;
            }
            swap(*it, tweet_buf.back());
            tweet_buf.pop_back();
        }


    } else{
        Tweet new_tweet;
        new_tweet.req_id = req_id;

        vector<string> results;
        boost::split(results, req_id, [](char c){return c == ' ';});
        new_tweet.user_id = results.front();
        if (type == "text")
            new_tweet.text = data;
        else if (type == "image")
            new_tweet.image = data;
        else if (type == "video")
            new_tweet.video = data;
        else if (type == "tag")
            new_tweet.tags = data;
        else if (type == "url")
            new_tweet.urls = data;
        else if (type == "unique_id")
            new_tweet.unique_id = data;

        new_tweet.tweet_type = "REPLY";
        tweet_buf.push_back(new_tweet);

    }
    if (IF_TRACE)
        logger(req_id, "ComposeReply", "compose_" + type, "end");

//    thread_mx.unlock();


}

int main() {

//    boost::shared_ptr<TProcessor> processor(new ComposeTweetProcessor(boost::make_shared<ComposeTweetHandler>()));
//    boost::shared_ptr<TProtocolFactory> protocol_factory(new TBinaryProtocolFactory());
//    shared_ptr<TTransportFactory> transport_factory(new TBufferedTransportFactory());
//    shared_ptr<TServerTransport> server_transport(new TServerSocket(9090));
//
//    boost::shared_ptr<ThreadManager> thread_manager = ThreadManager::newSimpleThreadManager(64);
//    boost::shared_ptr<PosixThreadFactory> thread_factory(new PosixThreadFactory());
//    thread_manager->threadFactory(thread_factory);
//    thread_manager->start();
//    TNonblockingServer server(processor, protocol_factory, 9090, thread_manager);
    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    TSimpleServer server(
            boost::make_shared<ComposeReplyProcessor>(boost::make_shared<ComposeReplyHandler>()),
            boost::make_shared<TServerSocket>(9103),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}








