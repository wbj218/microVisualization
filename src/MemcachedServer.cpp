//
// Created by Yu Gan on 7/11/17.
//

#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/TToString.h>

#include <boost/make_shared.hpp>
#include <iostream>
#include <libmongoc-1.0/mongoc.h>
#include "../gen-cpp/Memcached.h"
#include <libmemcached/memcached.h>

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;
using namespace TwitterMicroservices;

class MemcachedHandler: public MemcachedIf {
public:
    MemcachedHandler();
    ~MemcachedHandler();
    void ping() {cout << "ping()" << endl;}
    void get_data(string&, const string&);
    int32_t set_data(const string&, const string&);
    bool if_exist(const string&);
    int32_t delete_data(const string&);


private:
    string configs;
    memcached_st *mmc;
    memcached_return_t rc;
    uint32_t flags;
    size_t value_length;
    string mmc_addr = "192.168.99.100";
    string mmc_port = "32771";

};

MemcachedHandler::MemcachedHandler(){

    configs = "--SERVER=" + mmc_addr + ":" + mmc_port;
    mmc = memcached(configs.c_str(), configs.length());
}

MemcachedHandler::~MemcachedHandler(){
    memcached_free(mmc);
}

void MemcachedHandler::get_data(string& _return, const string& key) {

    char* value = memcached_get(mmc, key.c_str(), key.length(), &value_length, &flags, &rc);
    assert(rc == MEMCACHED_SUCCESS);
//    cout<<value<<endl;
//    string s(value);
//    _return = s;
    _return = value;
    free(value);
}

int32_t MemcachedHandler::set_data(const string& key, const string& value) {
    rc = memcached_set(mmc, key.c_str(), key.length(), value.c_str(), value.length(),
                                         (time_t)0, (uint32_t)0);
//    cout<<"key: "<<key.c_str()<<", value: "<<value<<"success"<<endl;
    assert(rc == MEMCACHED_SUCCESS);
    return 0;
}

bool MemcachedHandler::if_exist(const string& key) {
    rc = memcached_exist(mmc, key.c_str(), key.length());
    assert(rc == MEMCACHED_SUCCESS || rc == MEMCACHED_NOTFOUND);
    return rc == MEMCACHED_SUCCESS ? true: false;
}

int32_t MemcachedHandler::delete_data(const string& key) {
    rc = memcached_delete(mmc, key.c_str(), key.length(), (time_t)0);
    assert(rc == MEMCACHED_SUCCESS);
    return 0;
}

class MemcachedCloneFactory : virtual public MemcachedIfFactory {
public:
    virtual ~MemcachedCloneFactory() {}
    virtual MemcachedIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo)
    {
        boost::shared_ptr<TSocket> sock = boost::dynamic_pointer_cast<TSocket>(connInfo.transport);
//        cout << "Incoming connection\n";
//        cout << "\tSocketInfo: "  << sock->getSocketInfo() << "\n";
//        cout << "\tPeerHost: "    << sock->getPeerHost() << "\n";
//        cout << "\tPeerAddress: " << sock->getPeerAddress() << "\n";
//        cout << "\tPeerPort: "    << sock->getPeerPort() << "\n";
        return new MemcachedHandler;
    }
    virtual void releaseHandler(MemcachedIf* handler) {
        delete handler;
    }
};



int main() {
    TThreadedServer server(
            boost::make_shared<MemcachedProcessorFactory>(boost::make_shared<MemcachedCloneFactory>()),
            boost::make_shared<TServerSocket>(9097), //port
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());
    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}

