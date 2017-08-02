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
#include <random>
#include "../gen-cpp/ComposeRetweet.h"
#include "../gen-cpp/MongoTweetStorage.h"
#include "../gen-cpp/WriteTimeline.h"
#include "../gen-cpp/WriteUserGraph.h"
#include "../gen-cpp/BlockStats.h"

#include "json.hpp"
#include <signal.h>
#include <sys/time.h>
#include <fstream>


#define LOG_PATH "../logs/ComposeRetweet.log"
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



//mutex thread_mx;
vector<Tweet> tweet_buf;



class ComposeRetweetHandler: public ComposeRetweetIf {
public:
    ComposeRetweetHandler(const int, const int, const int, const int*, const int*, const int*);
    ~ComposeRetweetHandler();
    void ping() {cout << "ping(from server)" << endl;}

    void upload(const string&, const string&, const string&);

    void upload_tweet(const Tweet&, const string &);
private:

    int n_tweet_store;
    int n_timeline;
    int n_graph;
    const int *tweet_store_ports;
    const int *timeline_ports;
    const int *graph_ports;

    boost::shared_ptr<TTransport>* mongo_tweet_storage_socket;
    boost::shared_ptr<TTransport>* mongo_tweet_storage_transport;
    boost::shared_ptr<TProtocol>* mongo_tweet_storage_protocol;
    boost::shared_ptr<MongoTweetStorageClient>* mongo_tweet_storage_client;

    boost::shared_ptr<TTransport>* write_graph_socket;
    boost::shared_ptr<TTransport>* write_graph_transport;
    boost::shared_ptr<TProtocol>* write_graph_protocol;
    boost::shared_ptr<WriteUserGraphClient>* write_graph_client;

    boost::shared_ptr<TTransport>* write_timeline_socket;
    boost::shared_ptr<TTransport>* write_timeline_transport;
    boost::shared_ptr<TProtocol>* write_timeline_protocol;
    boost::shared_ptr<WriteTimelineClient>* write_timeline_client;

    boost::shared_ptr<TTransport> block_socket;
    boost::shared_ptr<TTransport> block_transport;
    boost::shared_ptr<TProtocol> block_protocol;
    boost::shared_ptr<BlockStatsClient> block_client;


};

ComposeRetweetHandler::ComposeRetweetHandler(const int n_tweet_store, const int n_timeline, const int n_graph,
                                             const int* tweet_store_ports, const int* timeline_ports, const int* graph_ports)
{
    srand((int) time(0));
    this->n_tweet_store = n_tweet_store;
    this->n_timeline = n_timeline;
    this->n_graph = n_graph;
    this->tweet_store_ports = tweet_store_ports;
    this->timeline_ports = timeline_ports;
    this->graph_ports = graph_ports;

    try {
        mongo_tweet_storage_socket = new boost::shared_ptr<TTransport>[n_tweet_store];
        mongo_tweet_storage_transport = new boost::shared_ptr<TTransport>[n_tweet_store];
        mongo_tweet_storage_protocol = new boost::shared_ptr<TProtocol>[n_tweet_store];
        mongo_tweet_storage_client = new boost::shared_ptr<MongoTweetStorageClient>[n_tweet_store];
        write_graph_socket = new boost::shared_ptr<TTransport>[n_graph];
        write_graph_transport = new boost::shared_ptr<TTransport>[n_graph];
        write_graph_protocol = new boost::shared_ptr<TProtocol>[n_graph];
        write_graph_client = new boost::shared_ptr<WriteUserGraphClient>[n_graph];
        write_timeline_socket = new boost::shared_ptr<TTransport>[n_timeline];
        write_timeline_transport = new boost::shared_ptr<TTransport>[n_timeline];
        write_timeline_protocol = new boost::shared_ptr<TProtocol>[n_timeline];
        write_timeline_client = new boost::shared_ptr<WriteTimelineClient>[n_timeline];


        for (int i = 0; i < n_tweet_store; i++) {
            cout<<"store"<<tweet_store_ports[i]<<endl;
            mongo_tweet_storage_socket[i] = (boost::shared_ptr<TTransport>) new TSocket("localhost", tweet_store_ports[i]);
            mongo_tweet_storage_transport[i] =
                    (boost::shared_ptr<TTransport>) new TBufferedTransport(mongo_tweet_storage_socket[i]);
            mongo_tweet_storage_protocol[i] =
                    (boost::shared_ptr<TProtocol>) new TBinaryProtocol(mongo_tweet_storage_transport[i]);
            mongo_tweet_storage_client[i] =
                    (boost::shared_ptr<MongoTweetStorageClient>)new MongoTweetStorageClient(mongo_tweet_storage_protocol[i]);
        }

        for (int i = 0; i < n_graph; i++) {

            cout<<"graph"<<graph_ports[i]<<endl;
            write_graph_socket[i] = (boost::shared_ptr<TTransport>) new TSocket("localhost", graph_ports[i]);
            write_graph_transport[i] = (boost::shared_ptr<TTransport>)new TBufferedTransport(write_graph_socket[i]);
            write_graph_protocol[i] = (boost::shared_ptr<TProtocol>)new TBinaryProtocol(write_graph_transport[i]);
            write_graph_client[i] = (boost::shared_ptr<WriteUserGraphClient>) new WriteUserGraphClient(write_graph_protocol[i]);
        }

        for (int i = 0; i < n_timeline; i++) {

            cout<<"timeline"<<timeline_ports[i]<<endl;
            write_timeline_socket[i] = (boost::shared_ptr<TTransport>) new TSocket("localhost", timeline_ports[i]);
            write_timeline_transport[i] = (boost::shared_ptr<TTransport>)new TBufferedTransport(write_timeline_socket[i]);
            write_timeline_protocol[i] = (boost::shared_ptr<TProtocol>)new TBinaryProtocol(write_timeline_transport[i]);
            write_timeline_client[i] = (boost::shared_ptr<WriteTimelineClient>)new WriteTimelineClient(write_timeline_protocol[i]);
        }

        block_socket = (boost::shared_ptr<TTransport>) new TSocket("localhost", 9111);
        block_transport = (boost::shared_ptr<TTransport>)new TBufferedTransport(block_socket);
        block_protocol = (boost::shared_ptr<TProtocol>)new TBinaryProtocol(block_transport);
        block_client = (boost::shared_ptr<BlockStatsClient>)new BlockStatsClient(block_protocol);

    } catch (TException& tx) {
        cout << "ERROR: " << tx.what() << endl;
    }
}

ComposeRetweetHandler::~ComposeRetweetHandler() {
    delete[] mongo_tweet_storage_socket;
    delete[] mongo_tweet_storage_socket;
    delete[] mongo_tweet_storage_transport;
    delete[] mongo_tweet_storage_protocol;
    delete[] mongo_tweet_storage_client;
    delete[] write_graph_socket;
    delete[] write_graph_transport;
    delete[] write_graph_protocol;
    delete[] write_graph_client;
    delete[] write_timeline_socket;
    delete[]  write_timeline_transport;
    delete[] write_timeline_protocol;
    delete[] write_timeline_client;


}


void ComposeRetweetHandler::upload(const string& req_id, const string& type, const string& data){

//    thread_mx.lock();
    cout<<req_id<<" "<<type<<" "<<data<<endl;
    if (IF_TRACE)
        logger(req_id, "ComposeRetweet", "compose_" + type,  "begin");

    auto it = std::find_if (tweet_buf.begin(), tweet_buf.end(), [&req_id](Tweet& t){return t.req_id == req_id;});

    if (it != tweet_buf.end()){
        if (type == "text")
            it->retweet_text = data;

        else if (type == "tag")
            it->retweet_tags = data;
        else if (type == "url")
            it->retweet_urls = data;
        else if (type == "unique_id")
            it->unique_id = data;



        if (it->retweet_text != "" && it->unique_id != "" && it->retweet_tags != "" && it->retweet_urls != "" &&
            it->orig_tweet_id != "" && it->text != "") {

            cout<<req_id<<" "<<"complete!!"<<endl;


            try {
                int tweet_store_index;
                int graph_index;
                int timeline_index;

                tweet_store_index = rand() % n_tweet_store;
                mongo_tweet_storage_transport[tweet_store_index]->open();
                mongo_tweet_storage_client[tweet_store_index]->store_tweet(*it.base(), req_id);
                mongo_tweet_storage_transport[tweet_store_index]->close();

                timeline_index = rand() % n_timeline;
                write_timeline_transport[timeline_index]->open();
                write_timeline_client[timeline_index]->write_timeline(it->user_id, it->unique_id, it->req_id, "RETWEET");
                write_timeline_transport[timeline_index]->close();

                istringstream iss(it->tags);
                string line;
                vector<string> results;
                while (std::getline(iss, line)) {
                    boost::split(results, line, [](char c) { return c == ' '; });
                    if (results.front()!= "NULL" ) {
                        bool if_blocked;
                        block_transport->open();
                        if_blocked = block_client->if_blocked(results.front(), it->user_id, req_id);
                        block_transport->close();
                        if (!if_blocked) {
                            timeline_index = rand() % n_timeline;
                            write_timeline_transport[timeline_index]->open();
                            write_timeline_client[timeline_index]->write_timeline(results.front(), it->unique_id, it->req_id, "RETWEET_TAGGED");
                            write_timeline_transport[timeline_index]->close();
                        }

                    }
                }

                graph_index = rand() % n_graph;
                write_graph_transport[graph_index]->open();
                write_graph_client[graph_index]->write_graph(it->user_id, it->unique_id, it->req_id);
                write_graph_transport[graph_index]->close();


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
        if (type == "retweet_text")
            new_tweet.text = data;
        else if (type == "retweet_tag")
            new_tweet.tags = data;
        else if (type == "retweet_url")
            new_tweet.urls = data;
        else if (type == "unique_id")
            new_tweet.unique_id = data;

        new_tweet.tweet_type = "RETWEET";
        tweet_buf.push_back(new_tweet);

    }

//    thread_mx.unlock();
    if (IF_TRACE)
        logger(req_id, "ComposeRetweet", "compose_" + type,  "end");


}

void ComposeRetweetHandler::upload_tweet(const Tweet &tweet, const string &req_id) {
    if (IF_TRACE)
        logger(req_id, "ComposeRetweet", "compose_tweet",  "begin");

    auto it = std::find_if (tweet_buf.begin(), tweet_buf.end(), [&req_id](Tweet& t){return t.req_id == req_id;});
    if (it != tweet_buf.end()) {
        it->text = tweet.text;
        it->image = tweet.image;
        it->video = tweet.video;
        it->tags = tweet.tags;
        it->urls = tweet.urls;
        it->orig_tweet_id = tweet.unique_id;
        it->orig_user_id = tweet.user_id;


        if (it->retweet_text != "" && it->unique_id != "" && it->retweet_tags != "" && it->retweet_urls != "" &&
            it->orig_tweet_id != "" && it->text != "") {
            cout << req_id << " " << "complete!!" << endl;


            try {
                int tweet_store_index;
                int graph_index;
                int timeline_index;

                tweet_store_index = rand() % n_tweet_store;
                mongo_tweet_storage_transport[tweet_store_index]->open();
                mongo_tweet_storage_client[tweet_store_index]->store_tweet(*it.base(), req_id);
                mongo_tweet_storage_transport[tweet_store_index]->close();

                timeline_index = rand() % n_timeline;
                write_timeline_transport[timeline_index]->open();
                write_timeline_client[timeline_index]->write_timeline(it->user_id, it->unique_id, it->req_id, "RETWEET");
                write_timeline_transport[timeline_index]->close();

                istringstream iss(it->tags);
                string line;
                vector<string> results;
                while (std::getline(iss, line)) {
                    boost::split(results, line, [](char c) { return c == ' '; });
                    if (results.front()!= "NULL" ) {
                        bool if_blocked;
                        block_transport->open();
                        if_blocked = block_client->if_blocked(results.front(), it->user_id, req_id);
                        block_transport->close();
                        if (!if_blocked) {
                            timeline_index = rand() % n_timeline;
                            write_timeline_transport[timeline_index]->open();
                            write_timeline_client[timeline_index]->write_timeline(results.front(), it->unique_id, it->req_id, "RETWEET_TAGGED");
                            write_timeline_transport[timeline_index]->close();
                        }

                    }
                }

                graph_index = rand() % n_graph;
                write_graph_transport[graph_index]->open();
                write_graph_client[graph_index]->write_graph(it->user_id, it->unique_id, it->req_id);
                write_graph_transport[graph_index]->close();

            } catch (TException &tx) {
                cout << "ERROR: " << tx.what() << endl;
            }
            swap(*it, tweet_buf.back());
            tweet_buf.pop_back();
        }
    }
    else {
        Tweet new_tweet;
        new_tweet.req_id = req_id;
        new_tweet.text = tweet.text;
        new_tweet.image = tweet.image;
        new_tweet.video = tweet.video;
        new_tweet.tags = tweet.tags;
        new_tweet.urls = tweet.urls;
        new_tweet.orig_tweet_id = tweet.unique_id;
        new_tweet.orig_user_id = tweet.user_id;
        new_tweet.tweet_type = "RETWEET";
        tweet_buf.push_back(new_tweet);
    }
    if (IF_TRACE)
        logger(req_id, "ComposeRetweet", "compose_tweet",  "end");

}



int main(int argc, char *argv[]) {

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

    int n_tweet_store = stoi(argv[2]);
    int n_timeline = stoi(argv[3]);
    int n_graph = stoi(argv[4]);
    int *tweet_store_ports = new int[n_tweet_store];
    int *timeline_ports = new int[n_timeline];
    int *graph_ports = new int[n_graph];

    for (int i = 0; i < n_tweet_store; i++){
        tweet_store_ports[i] = stoi(argv[5 + i]);
        cout<<"store "<<tweet_store_ports[i]<<endl;
    }


    for (int i = 0; i < n_timeline; i++) {
        timeline_ports[i] = stoi(argv[5 + n_tweet_store + i]);
        cout<<"timeline "<<timeline_ports[i]<<endl;
    }


    for (int i = 0; i < n_graph; i++) {
        graph_ports[i] = stoi(argv[5 + n_tweet_store + n_timeline + i]);
        cout<<"graph "<<graph_ports[i]<<endl;
    }

    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    TSimpleServer server(
            boost::make_shared<ComposeRetweetProcessor>(boost::make_shared<ComposeRetweetHandler>(n_tweet_store, n_timeline, n_graph, tweet_store_ports, timeline_ports, graph_ports)),
            boost::make_shared<TServerSocket>(9104),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}








