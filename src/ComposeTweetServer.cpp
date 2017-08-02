//
// Created by Yu Gan on 7/10/17.
//


#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/Mutex.h>
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
#include <stdlib.h>
#include <time.h>
#include "../gen-cpp/ComposeTweet.h"
#include "../gen-cpp/MongoTweetStorage.h"
#include "../gen-cpp/WriteUserGraph.h"
#include "../gen-cpp/WriteTimeline.h"
#include "../gen-cpp/BlockStats.h"

#include "json.hpp"
#include <signal.h>
#include <sys/time.h>
#include <fstream>


#define LOG_PATH "../logs/ComposeTweet.log"
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
// Mutex thread_mx;

void logger(const string &log_id, const string &service, const string &stage, const string &state) {
    // thread_mx.lock();
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long time_in_us = tv.tv_sec * 1000000 + tv.tv_usec;
    logs[log_id][service][stage][state] = time_in_us;
    // thread_mx.unlock();
}

void exit_handler(int sig) {
    ofstream log_file;
    log_file.open(LOG_PATH);
    log_file << logs;
    log_file.close();

}



//mutex thread_mx;
vector<Tweet> tweet_buf;



class ComposeTweetHandler: public ComposeTweetIf {
public:
    ComposeTweetHandler(const int, const int, const int, const int*, const int*, const int*);
    ~ComposeTweetHandler();
    void ping() {cout << "ping(from server)" << endl;}

    void upload(const string&, const string&, const string&);


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

ComposeTweetHandler::ComposeTweetHandler(const int n_tweet_store, const int n_timeline, const int n_graph,
        const int* tweet_store_ports, const int* timeline_ports, const int* graph_ports) {

    
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

ComposeTweetHandler::~ComposeTweetHandler() {   
    delete[] mongo_tweet_storage_socket;
    delete[] mongo_tweet_storage_transport;
    delete[] mongo_tweet_storage_protocol;
    delete[] mongo_tweet_storage_client;
    delete[] write_graph_socket;
    delete[] write_graph_transport;
    delete[] write_graph_protocol;
    delete[] write_graph_client;
    delete[] write_timeline_socket;
    delete[] write_timeline_transport;
    delete[] write_timeline_protocol;
    delete[] write_timeline_client;


}






void ComposeTweetHandler::upload(const string& req_id, const string& type, const string& data){
    if (IF_TRACE)
        logger(req_id, "ComposeTweet", "compose_" + type,  "begin");

//    thread_mx.lock();
    // cout<<req_id<<" "<<type<<" "<<data<<endl;
    auto it = std::find_if (tweet_buf.begin(), tweet_buf.end(), [&req_id](Tweet& t){return t.req_id == req_id;});

    if (it != tweet_buf.end()){
        if (type == "text") {
            it->text = data;
        }

        else if (type == "image") {
            it->image = data;
        }

        else if (type == "video"){
            it->video = data;
//            cout<<"video received"<<endl;
        }

        else if (type == "tag") {
            it->tags = data;
        }

        else if (type == "url")
            it->urls = data;
        else if (type == "unique_id")
            it->unique_id = data;
        // else if (type == "unique_id"){
        //     if (it->user_id == "@user_0")
        //         it->unique_id = "0";
        //     else if (it->user_id == "@user_1")
        //         it->unique_id = "1";
        //     else if (it->user_id == "@user_2")
        //         it->unique_id = "2";
        //     else if (it->user_id == "@user_3")
        //         it->unique_id = "3";
        //     else if (it->user_id == "@user_4")
        //         it->unique_id = "4";
        // }


        if (it->text != "" && it->image != "" && it->video != "" && it->unique_id != "" && it->tags != ""
            && it->urls != "" && it->user_id != "") {

            cout<<req_id<<" "<<"complete!!"<<endl;


            try {

                int tweet_store_index;
                int graph_index;
                int timeline_index;

                tweet_store_index = rand() % n_tweet_store;
                mongo_tweet_storage_transport[tweet_store_index]->open();
                mongo_tweet_storage_client[tweet_store_index]->store_tweet(*it.base(), req_id);
//                cout<<it.base()->image;
                mongo_tweet_storage_transport[tweet_store_index]->close();

                

                timeline_index = rand() % n_timeline;
                write_timeline_transport[timeline_index]->open();
                write_timeline_client[timeline_index]->write_timeline(it->user_id, it->unique_id, it->req_id, "TWEET");
                write_timeline_transport[timeline_index]->close();

                istringstream iss(it->tags);
                string line;
                while (std::getline(iss, line))
                {
                    vector<string> results;
                    boost::split(results, line, [](char c){return c == ' ';});
                    if (results.front()!= "NULL" ) {
                        bool if_blocked;
                        block_transport->open();

                        if_blocked = block_client->if_blocked(results.front(), it->user_id, req_id);
                        block_transport->close();
                        if (!if_blocked) {
                            timeline_index = rand() % n_timeline;
                            write_timeline_transport[timeline_index]->open();
                            write_timeline_client[timeline_index]->write_timeline(results.front(), it->unique_id, it->req_id, "TWEET_TAGGED");
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
        // else if (type == "unique_id"){
        //     if (new_tweet.user_id == "@user_0")
        //         new_tweet.unique_id = "0";
        //     else if (new_tweet.user_id == "@user_1")
        //         new_tweet.unique_id = "1";
        //     else if (new_tweet.user_id == "@user_2")
        //         new_tweet.unique_id = "2";
        //     else if (new_tweet.user_id == "@user_3")
        //         new_tweet.unique_id = "3";
        //     else if (new_tweet.user_id == "@user_4")
        //         new_tweet.unique_id = "4";
        // }
            

        new_tweet.tweet_type = "TWEET";
        tweet_buf.push_back(new_tweet);

    }

    // thread_mx.unlock();
    if (IF_TRACE)
        logger(req_id, "ComposeTweet", "compose_" + type, "end");


}

class ComposeTweetCloneFactory: virtual public  ComposeTweetIfFactory {
public:
    ComposeTweetCloneFactory(const int n_tweet_store, const int n_timeline, const int n_graph,
            const int* tweet_store_ports, const int* timeline_ports, const int* graph_ports) {
        this->n_tweet_store = n_tweet_store;
        this->n_timeline = n_timeline;
        this->n_graph = n_graph;
        this->tweet_store_ports = tweet_store_ports;
        this->timeline_ports = timeline_ports;
        this->graph_ports = graph_ports;
    };
    virtual ~ComposeTweetCloneFactory() {}
    virtual ComposeTweetIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo)
    {
        boost::shared_ptr<TSocket> sock = boost::dynamic_pointer_cast<TSocket>(connInfo.transport);
        return new ComposeTweetHandler(n_tweet_store, n_timeline, n_graph, tweet_store_ports, timeline_ports, graph_ports);
    }
    virtual void releaseHandler(ComposeTweetIf* handler) {
        delete handler;
    }
private:
    int n_tweet_store;
    int n_timeline;
    int n_graph;
    const int *tweet_store_ports;
    const int *timeline_ports;
    const int *graph_ports;

};

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
    srand((int) time(0));

    TSimpleServer server(
    boost::make_shared<ComposeTweetProcessor>(boost::make_shared<ComposeTweetHandler>(n_tweet_store, n_timeline, n_graph, tweet_store_ports, timeline_ports, graph_ports)),
    boost::make_shared<TServerSocket>(stoi(argv[1])),
    boost::make_shared<TBufferedTransportFactory>(),
    boost::make_shared<TBinaryProtocolFactory>()
    );

    // const int workerCount = 2;
  
    // boost::shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(workerCount);
    // threadManager->threadFactory(boost::make_shared<PlatformThreadFactory>());
    // threadManager->start();

    // TThreadPoolServer server(
    // boost::make_shared<ComposeTweetProcessorFactory>(boost::make_shared<ComposeTweetCloneFactory>(n_tweet_store, n_timeline, n_graph, tweet_store_ports, timeline_ports, graph_ports)),
    // boost::make_shared<TServerSocket>(stoi(argv[1])),
    // boost::make_shared<TBufferedTransportFactory>(),
    // boost::make_shared<TBinaryProtocolFactory>(),
    // threadManager);

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}








