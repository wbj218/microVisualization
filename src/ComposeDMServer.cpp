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
#include <vector>
#include <sstream>
#include "../gen-cpp/ComposeDM.h"
#include "../gen-cpp/MongoDMStorage.h"

#include "../gen-cpp/WriteDM.h"
#include "json.hpp"
#include <signal.h>
#include <sys/time.h>
#include <fstream>
#define LOG_PATH "../logs/ComposeDM.log"
#define IF_TRACE true

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;
using namespace TwitterMicroservices;
using json = nlohmann::json;





//mutex thread_mx;
vector<Tweet> tweet_buf;

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



class ComposeDMHandler: public ComposeDMIf {
public:
    ComposeDMHandler();
    ~ComposeDMHandler();
    void ping() {cout << "ping(from server)" << endl;}

    void upload(const string&, const string&, const string&);


private:


    boost::shared_ptr<TTransport> mongo_dm_storage_socket;
    boost::shared_ptr<TTransport> mongo_dm_storage_transport;
    boost::shared_ptr<TProtocol> mongo_dm_storage_protocol;
    boost::shared_ptr<MongoDMStorageClient> mongo_dm_storage_client;




    boost::shared_ptr<TTransport> write_dm_socket;
    boost::shared_ptr<TTransport> write_dm_transport;
    boost::shared_ptr<TProtocol> write_dm_protocol;
    boost::shared_ptr<WriteDMClient> write_dm_client;



};

ComposeDMHandler::ComposeDMHandler() {
    try {


        mongo_dm_storage_socket = (boost::shared_ptr<TTransport>) new TSocket("localhost", 9106);
        mongo_dm_storage_transport =
                (boost::shared_ptr<TTransport>) new TBufferedTransport(mongo_dm_storage_socket);
        mongo_dm_storage_protocol =
                (boost::shared_ptr<TProtocol>) new TBinaryProtocol(mongo_dm_storage_transport);
        mongo_dm_storage_client = (boost::shared_ptr<MongoDMStorageClient>)
                new MongoDMStorageClient(mongo_dm_storage_protocol);


        write_dm_socket = (boost::shared_ptr<TTransport>) new TSocket("localhost", 9109);
        write_dm_transport = (boost::shared_ptr<TTransport>) new TBufferedTransport(write_dm_socket);
        write_dm_protocol = (boost::shared_ptr<TProtocol>) new TBinaryProtocol(write_dm_transport);
        write_dm_client = (boost::shared_ptr<WriteDMClient>) new WriteDMClient(write_dm_protocol);

    } catch (TException& tx) {
        cout << "ERROR: " << tx.what() << endl;
    }
}

ComposeDMHandler::~ComposeDMHandler() {


}


void ComposeDMHandler::upload(const string& req_id, const string& type, const string& data){

//    thread_mx.lock();
//    cout<<req_id<<" "<<type<<" "<<data<<endl;
     if (IF_TRACE)
        logger(req_id, "ComposeDM", "compose_" + type,  "begin");


    auto it = std::find_if (tweet_buf.begin(), tweet_buf.end(), [&req_id](Tweet& t){return t.req_id == req_id;});

    if (it != tweet_buf.end()){
        if (type == "text")
            it->text = data;
        else if (type == "image")
            it->image = data;
        else if (type == "video")
            it->video = data;
        else if (type == "tag")
            it->tags = data;
        else if (type == "url")
            it->urls = data;
        else if (type == "unique_id")
            it->unique_id = data;



        if (it->text != "" && it->image != "" && it->video != "" && it->unique_id != "" && it->tags != ""
            && it->urls != "") {

            cout<<req_id<<" "<<"complete!!"<<endl;


            try {
                vector<string> results;
                boost::split(results, it->text, [](char c){return c == ' ';});
                string recipient_id;

                recipient_id = results.front();
                it->recipient_id = recipient_id;

                mongo_dm_storage_transport->open();
                mongo_dm_storage_client->store_dm(*it.base(), req_id);
                mongo_dm_storage_transport->close();


                write_dm_transport->open();

                write_dm_client->write_dm(it->user_id, it->unique_id, it->req_id);
                write_dm_client->write_dm(recipient_id, it->unique_id, it->req_id);

                write_dm_transport->close();


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

        new_tweet.tweet_type = "DM";
        tweet_buf.push_back(new_tweet);

    }
    if (IF_TRACE)
        logger(req_id, "ComposeDM", "compose_" + type,  "end");
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
            boost::make_shared<ComposeDMProcessor>(boost::make_shared<ComposeDMHandler>()),
            boost::make_shared<TServerSocket>(9105),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}








