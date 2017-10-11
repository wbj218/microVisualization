//
// Created by Yu Gan on 8/10/17.
//

#include "netflix_microservices.h"
#include "libmemcached/memcached.h"
#include <libmongoc-1.0/mongoc.h>
#include "../gen-cpp/UserAccount.h"
#include <mutex>
#include <vector>
#include <map>
#include <boost/algorithm/string.hpp>

#define MONGO_PORT 32030
#define MMC_PORT 32031

using namespace NetflixMicroservices;

json logs;
bool IF_TRACE;
string LOG_PATH;

map<string, int> movie_price;

std::mutex thread_mutex;

void logger(const string &log_id, const string &service, const string &stage, const string &state) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long time_in_us = tv.tv_sec * 1000000 + tv.tv_usec;
    thread_mutex.lock();
    logs[log_id][service][stage][state] = time_in_us;
    thread_mutex.unlock();
}



void exit_handler(int sig) {
    ofstream log_file;
    log_file.open(LOG_PATH);
    log_file << logs;
    log_file.close();
}

class UserAccountHandler: public UserAccountIf {
public:
    UserAccountHandler();
    ~UserAccountHandler();
    void ping() { cout << "ping(from server)" << endl; }
    bool if_purchased(const std::string& req_id, const std::string& user_id, const std::string& movie_id);
    bool purchase(const std::string& req_id, const std::string& user_id, const std::string& movie_id);
    void add_account(const string& req_id, const std::string& user_id, const int amount);


private:
    mongoc_client_t *mongo_client;
    mongoc_collection_t *collection;
    memcached_st *mmc;

};

UserAccountHandler::UserAccountHandler() {
    string mmc_configs;
    mongo_client = mongoc_client_new (("mongodb://" + to_string(DOCKER_IP_ADDR) + ":" + to_string(MONGO_PORT) +
                                       "/?appname=user_account").c_str());
    assert(mongo_client);
    collection = mongoc_client_get_collection (mongo_client, "user_account", "user_account");
    assert(collection);
    mmc_configs = "--SERVER=" + to_string(DOCKER_IP_ADDR) + ":" + to_string(MMC_PORT);
    mmc = memcached(mmc_configs.c_str(), mmc_configs.length());
    assert(mmc);
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_NO_BLOCK, 1);
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_TCP_NODELAY, 1);
//    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_NOREPLY, 1);
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_TCP_KEEPALIVE, 1);
    memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_BINARY_PROTOCOL, 1);
}

UserAccountHandler::~UserAccountHandler() {
    mongoc_client_destroy (mongo_client);
    memcached_free(mmc);
}

bool UserAccountHandler::if_purchased(const std::string& req_id, const std::string& user_id, const std::string& movie_id) {
    if (IF_TRACE)
        logger(req_id, "UserAccount", "if_purchased", "begin");
    bool _return = false;
    char * mmc_value_char;
    memcached_return_t mmc_rc;
    uint32_t mmc_flags;
    size_t mmc_value_length;
    string mmc_key = user_id + "_purchased_list";
    string purchased_str;
    mmc_value_char = memcached_get(mmc, mmc_key.c_str(), mmc_key.length(), &mmc_value_length, &mmc_flags, &mmc_rc);
    if (mmc_value_char) {
        vector<string> purchased_list;
        boost::split(purchased_list, mmc_value_char, [](char c){return c == '\n';});
        _return = std::find(purchased_list.begin(), purchased_list.end(), movie_id) != purchased_list.end();
    } else {
        const bson_t *doc;
        bson_t *query;
        mongoc_cursor_t *cursor;
        json doc_json;
        query = bson_new();
        BSON_APPEND_UTF8(query, "user_id", user_id.c_str());
        cursor = mongoc_collection_find_with_opts (collection, query, NULL, NULL);
        if (mongoc_cursor_next(cursor, &doc)) {
            doc_json = json::parse(bson_as_json (doc, NULL));
            purchased_str = doc_json["purchased_list"];
            vector<string> purchased_list;
            boost::split(purchased_list, purchased_str, [](char c){return c == '\n';});
            _return = std::find(purchased_list.begin(), purchased_list.end(), movie_id) != purchased_list.end();
        } else {
            _return = false;
        }
        bson_destroy(query);
        mongoc_cursor_destroy (cursor);
    }

    free(mmc_value_char);

    if (IF_TRACE)
        logger(req_id, "UserAccount", "if_purchased", "end");
    return _return;

}


bool UserAccountHandler::purchase(const std::string& req_id, const std::string& user_id, const std::string& movie_id) {
    if (IF_TRACE)
        logger(req_id, "UserAccount", "purchase", "begin");

    bool _return = false;
    char * mmc_value_char;
    memcached_return_t mmc_rc;
    uint32_t mmc_flags;
    size_t mmc_value_length;
    string mmc_key = user_id + "_purchased_list";
    vector<string> purchased_vector;
    string purchased_str;

    mmc_value_char = memcached_get(mmc, mmc_key.c_str(), mmc_key.length(), &mmc_value_length, &mmc_flags, &mmc_rc);
    if (mmc_value_char) {
        purchased_str = mmc_value_char;
        boost::split(purchased_vector, mmc_value_char, [](char c){return c == '\n';});
        _return = std::find(purchased_vector.begin(), purchased_vector.end(), movie_id) != purchased_vector.end();
    } else {
        const bson_t *doc;
        bson_t *query;
        mongoc_cursor_t *cursor;
        json doc_json;
        query = bson_new();
        BSON_APPEND_UTF8(query, "user_id", user_id.c_str());
        cursor = mongoc_collection_find_with_opts (collection, query, NULL, NULL);
        if (mongoc_cursor_next(cursor, &doc)) {
            doc_json = json::parse(bson_as_json (doc, NULL));
            purchased_str = doc_json["purchased_list"];
            boost::split(purchased_vector, purchased_str, [](char c){return c == '\n';});
            _return = std::find(purchased_vector.begin(), purchased_vector.end(), movie_id) != purchased_vector.end();
        } else {
            _return = false;
        }
    }

    if (!_return) {
        mmc_key = user_id + "_balence";
        mmc_value_char = memcached_get(mmc, mmc_key.c_str(), mmc_key.length(), &mmc_value_length, &mmc_flags, &mmc_rc);
        if (mmc_value_char) {
            int balence = stoi(mmc_value_char);
            
            int price = movie_price[movie_id];
            
            if (balence >= price) {
                balence -= price;
                purchased_str += movie_id + '\n';
                memcached_set(mmc, mmc_key.c_str(), mmc_key.length(), to_string(balence).c_str(), to_string(balence).length(),
                              (time_t) 0, (uint32_t) 0);
                mmc_key = user_id + "_purchased_list";
                memcached_set(mmc, mmc_key.c_str(), mmc_key.length(), purchased_str.c_str(), purchased_str.length(),
                              (time_t) 0, (uint32_t) 0);

                bson_t *query;
                bson_t *update;
                bson_error_t bson_error;
                query = bson_new();
                BSON_APPEND_UTF8(query, "user_id", user_id.c_str());

                BSON_APPEND_UTF8(update, "user_id", user_id.c_str());
                BSON_APPEND_UTF8(update, "balence", to_string(balence).c_str());
                BSON_APPEND_UTF8(update, "purchased_list", purchased_str.c_str());

                bool rc = mongoc_collection_update(collection, MONGOC_UPDATE_NONE, query, update, NULL, &bson_error);
                assert(rc);
                bson_destroy(query);
                bson_destroy(update);
                
                _return = true;
            }
            
        }  else {
            const bson_t *doc;
            bson_t *query;
            mongoc_cursor_t *cursor;
            json doc_json;
            query = bson_new();
            BSON_APPEND_UTF8(query, "user_id", user_id.c_str());
            cursor = mongoc_collection_find_with_opts (collection, query, NULL, NULL);
            if (mongoc_cursor_next(cursor, &doc)) {
                doc_json = json::parse(bson_as_json (doc, NULL));
                string balence_str = doc_json["balence"];
                int balence = stoi(balence_str);
                int price = movie_price[movie_id];
                if (balence >= price) {
                    balence -= price;
                    purchased_str += movie_id + '\n';
                    memcached_set(mmc, mmc_key.c_str(), mmc_key.length(), to_string(balence).c_str(), to_string(balence).length(),
                                  (time_t) 0, (uint32_t) 0);
                    mmc_key = user_id + "_purchased_list";
                    memcached_set(mmc, mmc_key.c_str(), mmc_key.length(), purchased_str.c_str(), purchased_str.length(),
                                  (time_t) 0, (uint32_t) 0);
                    
                    bson_t *update;
                    bson_error_t bson_error;
                    update = bson_new();
                    BSON_APPEND_UTF8(update, "user_id", user_id.c_str());
                    BSON_APPEND_UTF8(update, "balence", to_string(balence).c_str());
                    BSON_APPEND_UTF8(update, "purchased_list", purchased_str.c_str());

                    bool rc = mongoc_collection_update(collection, MONGOC_UPDATE_NONE, query, update, NULL, &bson_error);

                    assert(rc);
                    bson_destroy(query);
                    bson_destroy(update);

                    _return = true;
                }
                
            }
            mongoc_cursor_destroy(cursor);
        }
    }
    
    


    if (IF_TRACE)
        logger(req_id, "UserAccount", "purchase", "end");
    
    return _return;
}

void UserAccountHandler::add_account(const string& req_id, const std::string& user_id, const int amount) {
    bson_t *doc;
    doc = bson_new();
    bson_error_t bson_error;
    BSON_APPEND_UTF8(doc, "user_id", user_id.c_str());
    BSON_APPEND_UTF8(doc, "balence", to_string(amount).c_str());
    BSON_APPEND_UTF8(doc, "purchased_list", "");

    bool rc = mongoc_collection_insert(collection, MONGOC_INSERT_NONE, doc, NULL, &bson_error);
    assert(rc);
    bson_destroy(doc);


}



class UserAccountCloneFactory: virtual public UserAccountIfFactory {
public:
    virtual ~UserAccountCloneFactory() {}
    virtual UserAccountIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo)
    {
        boost::shared_ptr<TSocket> sock = boost::dynamic_pointer_cast<TSocket>(connInfo.transport);
        return new UserAccountHandler();
    }
    virtual void releaseHandler(UserAccountIf* handler) {
        delete handler;
    }
};

int main (int argc, char *argv[]) {
    IF_TRACE = true;
    LOG_PATH = LOG_DIR_PATH + "UserAccount.log";

    movie_price["movie_0"] = 10;
    movie_price["movie_1"] = 10;
    movie_price["movie_2"] = 10;
    movie_price["movie_3"] = 10;
    movie_price["movie_4"] = 10;

    void (*handler)(int) = &exit_handler;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGKILL, handler);

    TThreadedServer server(
            boost::make_shared<UserAccountProcessorFactory>(boost::make_shared<UserAccountCloneFactory>()),
            boost::make_shared<TServerSocket>(10060),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}
