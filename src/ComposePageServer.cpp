//
// Created by Yu Gan on 8/8/17.
//


#include "netflix_microservices.h"
#include "../gen-cpp/GetMovieReview.h"
#include "../gen-cpp/ComposePage.h"
#include "../gen-cpp/GetCastInfo.h"
#include "../gen-cpp/GetPhoto.h"
#include "../gen-cpp/GetThumbnail.h"
#include "../gen-cpp/GetPlot.h"
#include "../gen-cpp/GetRating.h"
#include "../gen-cpp/GetVideo.h"
#include "../gen-cpp/GetWatchNext.h"
#include "../gen-cpp/NetflixMicroservices_types.h"
#include <map>

#define PLOT_PORT 10040
#define THUMBNAIL_PORT 10041
#define MOVIE_REVIEW_PORT 10046
#define RATING_PORT 10042
#define VIDEO_PORT 10045
#define PHOTO_PORT 10044
#define CAST_INFO_PORT 10043
#define WATCH_NEXT_PORT 10047

#define SERVER_PORT_START 10050

#define  NUM_COMPONENTS 8


using namespace NetflixMicroservices;

map<const string, Mutex> thread_mutexes;
map<const string, MoviePage> pages;
map<const string, Monitor*> thread_monitors;
map<const string, int> finished_counter;

Mutex shared_obj_lock;

json logs;

bool IF_TRACE;
string LOG_PATH;

void logger(const string &log_id, const string &service, const string &stage, const string &state) {
    shared_obj_lock.lock();
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long time_in_us = tv.tv_sec * 1000000 + tv.tv_usec;
    logs[log_id][service][stage][state] = time_in_us;
    shared_obj_lock.unlock();
}

void exit_handler(int sig) {
    ofstream log_file;
    log_file.open(LOG_PATH);
    log_file << logs;
    log_file.close();
}

class ComposePageHandler: public ComposePageIf {
public:
    ComposePageHandler();
    ~ComposePageHandler();
    void ping() { cout << "ping(from server)" << endl; }
    void compose_page(MoviePage& _return, const string& req_id, const string& movie_id, const string& user_id);
    void upload_plot(const string& req_id, const string& movie_id, const string& plot);
    void upload_rating(const string& req_id, const string& movie_id, const string& rating);
    void upload_thumbnail(const string& req_id, const string& movie_id, const string& thumbnail);
    void upload_cast_info(const string& req_id, const string& movie_id, const vector<CastInfo> & cast_info);
    void upload_photo(const string& req_id, const string& movie_id, const string& photo);
    void upload_video(const string& req_id, const string& movie_id, const string& video);
    void upload_movie_review(const string& req_id, const string& movie_id, const vector<Review> & reviews);
    void upload_watch_next(const string& req_id, const string& user_id, const vector<string> & watch_next);

private:
    boost::shared_ptr<TTransport> plot_socket;
    boost::shared_ptr<TTransport> plot_transport;
    boost::shared_ptr<TProtocol> plot_protocol;
    boost::shared_ptr<GetPlotClient> plot_client;

    boost::shared_ptr<TTransport> thumbnail_socket;
    boost::shared_ptr<TTransport> thumbnail_transport;
    boost::shared_ptr<TProtocol> thumbnail_protocol;
    boost::shared_ptr<GetThumbnailClient> thumbnail_client;

    boost::shared_ptr<TTransport> rating_socket;
    boost::shared_ptr<TTransport> rating_transport;
    boost::shared_ptr<TProtocol> rating_protocol;
    boost::shared_ptr<GetRatingClient> rating_client;

    boost::shared_ptr<TTransport> photo_socket;
    boost::shared_ptr<TTransport> photo_transport;
    boost::shared_ptr<TProtocol> photo_protocol;
    boost::shared_ptr<GetPhotoClient> photo_client;

    boost::shared_ptr<TTransport> video_socket;
    boost::shared_ptr<TTransport> video_transport;
    boost::shared_ptr<TProtocol> video_protocol;
    boost::shared_ptr<GetVideoClient> video_client;

    boost::shared_ptr<TTransport> movie_review_socket;
    boost::shared_ptr<TTransport> movie_review_transport;
    boost::shared_ptr<TProtocol> movie_review_protocol;
    boost::shared_ptr<GetMovieReviewClient> movie_review_client;

    boost::shared_ptr<TTransport> cast_info_socket;
    boost::shared_ptr<TTransport> cast_info_transport;
    boost::shared_ptr<TProtocol> cast_info_protocol;
    boost::shared_ptr<GetCastInfoClient> cast_info_client;

    boost::shared_ptr<TTransport> watch_next_socket;
    boost::shared_ptr<TTransport> watch_next_transport;
    boost::shared_ptr<TProtocol> watch_next_protocol;
    boost::shared_ptr<GetWatchNextClient> watch_next_client;

};

ComposePageHandler::ComposePageHandler() {
    plot_socket = (boost::shared_ptr<TTransport>) new TSocket("localhost", PLOT_PORT);
    plot_transport = (boost::shared_ptr<TTransport>) new TBufferedTransport(plot_socket);
    plot_protocol = (boost::shared_ptr<TProtocol>) new TBinaryProtocol(plot_transport);
    plot_client = (boost::shared_ptr<GetPlotClient>) new GetPlotClient(plot_protocol);

    thumbnail_socket = (boost::shared_ptr<TTransport>) new TSocket("localhost", THUMBNAIL_PORT);
    thumbnail_transport = (boost::shared_ptr<TTransport>) new TBufferedTransport(thumbnail_socket);
    thumbnail_protocol = (boost::shared_ptr<TProtocol>) new TBinaryProtocol(thumbnail_transport);
    thumbnail_client = (boost::shared_ptr<GetThumbnailClient>) new GetThumbnailClient(thumbnail_protocol);

    movie_review_socket = (boost::shared_ptr<TTransport>) new TSocket("localhost", MOVIE_REVIEW_PORT);
    movie_review_transport = (boost::shared_ptr<TTransport>) new TBufferedTransport(movie_review_socket);
    movie_review_protocol = (boost::shared_ptr<TProtocol>) new TBinaryProtocol(movie_review_transport);
    movie_review_client = (boost::shared_ptr<GetMovieReviewClient>) new GetMovieReviewClient(movie_review_protocol);

    cast_info_socket = (boost::shared_ptr<TTransport>) new TSocket("localhost", CAST_INFO_PORT);
    cast_info_transport = (boost::shared_ptr<TTransport>) new TBufferedTransport(cast_info_socket);
    cast_info_protocol = (boost::shared_ptr<TProtocol>) new TBinaryProtocol(cast_info_transport);
    cast_info_client = (boost::shared_ptr<GetCastInfoClient>) new GetCastInfoClient(cast_info_protocol);

    photo_socket = (boost::shared_ptr<TTransport>) new TSocket("localhost", PHOTO_PORT);
    photo_transport = (boost::shared_ptr<TTransport>) new TBufferedTransport(photo_socket);
    photo_protocol = (boost::shared_ptr<TProtocol>) new TBinaryProtocol(photo_transport);
    photo_client = (boost::shared_ptr<GetPhotoClient>) new GetPhotoClient(photo_protocol);

    video_socket = (boost::shared_ptr<TTransport>) new TSocket("localhost", VIDEO_PORT);
    video_transport = (boost::shared_ptr<TTransport>) new TBufferedTransport(video_socket);
    video_protocol = (boost::shared_ptr<TProtocol>) new TBinaryProtocol(video_transport);
    video_client = (boost::shared_ptr<GetVideoClient>) new GetVideoClient(video_protocol);

    rating_socket = (boost::shared_ptr<TTransport>) new TSocket("localhost", RATING_PORT);
    rating_transport = (boost::shared_ptr<TTransport>) new TBufferedTransport(rating_socket);
    rating_protocol = (boost::shared_ptr<TProtocol>) new TBinaryProtocol(rating_transport);
    rating_client = (boost::shared_ptr<GetRatingClient>) new GetRatingClient(rating_protocol);

    watch_next_socket = (boost::shared_ptr<TTransport>) new TSocket("localhost", WATCH_NEXT_PORT);
    watch_next_transport = (boost::shared_ptr<TTransport>) new TBufferedTransport(watch_next_socket);
    watch_next_protocol = (boost::shared_ptr<TProtocol>) new TBinaryProtocol(watch_next_transport);
    watch_next_client = (boost::shared_ptr<GetWatchNextClient>) new GetWatchNextClient(watch_next_protocol);
}

ComposePageHandler::~ComposePageHandler() {
    
}

void ComposePageHandler::compose_page(MoviePage& _return, const string& req_id, const string& movie_id, const string& user_id) {
    if (IF_TRACE)
        logger(req_id, "ComposePage", "compose_page", "begin");

    Mutex my_mutex;
    MoviePage my_page;
    shared_obj_lock.lock();
    thread_mutexes.insert(make_pair(req_id, my_mutex));
    Monitor my_monitor(&thread_mutexes[req_id]);
    thread_monitors.insert(make_pair(req_id, &my_monitor));
    pages.insert(make_pair(req_id, my_page));
    finished_counter.insert(make_pair(req_id, 0));
    shared_obj_lock.unlock();
    
    try {
        plot_transport->open();
        plot_client->get_plot(req_id, movie_id);
        plot_transport->close();

        thumbnail_transport->open();
        thumbnail_client->get_thumbnail(req_id, movie_id);
        thumbnail_transport->close();

        rating_transport->open();
        rating_client->get_rating(req_id, movie_id);
        rating_transport->close();

        cast_info_transport->open();
        cast_info_client->get_cast_info(req_id, movie_id);
        cast_info_transport->close();

        movie_review_transport->open();
        movie_review_client->get_movie_review(req_id, movie_id, 0, 10);
        movie_review_transport->close();

        photo_transport->open();
        photo_client->get_photo(req_id, movie_id);
        photo_transport->close();

        video_transport->open();
        video_client->get_video(req_id, movie_id);
        video_transport->close();

        watch_next_transport->open();
        watch_next_client->get_watch_next(req_id, movie_id);
        watch_next_transport->close();
    } catch (TException &tx) {
        cout << "ERROR: " << tx.what() << endl;
    }


    while (finished_counter[req_id] < NUM_COMPONENTS) {
        thread_monitors[req_id]->wait();
    }

    _return = pages[req_id];

    shared_obj_lock.lock();
    thread_monitors.erase(req_id);
    thread_mutexes.erase(req_id);
    pages.erase(req_id);
    finished_counter.erase(req_id);
    shared_obj_lock.unlock();

    if (IF_TRACE)
        logger(req_id, "ComposePage", "compose_page", "end");
}


void ComposePageHandler::upload_plot(const string& req_id, const string& movie_id, const string& plot) {
    if (IF_TRACE)
        logger(req_id, "ComposePage", "upload_plot", "begin");

    thread_mutexes[req_id].lock();
    pages[req_id].plot = plot;
    finished_counter[req_id]++;
    if (finished_counter[req_id] >= NUM_COMPONENTS)
        thread_monitors[req_id]->notify();
    thread_mutexes[req_id].unlock();

    if (IF_TRACE)
        logger(req_id, "ComposePage", "upload_plot", "end");
}
void ComposePageHandler::upload_rating(const string& req_id, const string& movie_id, const string& rating) {
    if (IF_TRACE)
        logger(req_id, "ComposePage", "upload_rating", "begin");

    thread_mutexes[req_id].lock();
    pages[req_id].rating = rating;
    finished_counter[req_id]++;
    if (finished_counter[req_id] >= NUM_COMPONENTS)
        thread_monitors[req_id]->notify();
    thread_mutexes[req_id].unlock();

    if (IF_TRACE)
        logger(req_id, "ComposePage", "upload_rating", "end");
}
void ComposePageHandler::upload_thumbnail(const string& req_id, const string& movie_id, const string& thumbnail){
    if (IF_TRACE)
        logger(req_id, "ComposePage", "upload_thumbnail", "begin");

    thread_mutexes[req_id].lock();
    pages[req_id].thumbnail = thumbnail;
    finished_counter[req_id]++;
    if (finished_counter[req_id] >= NUM_COMPONENTS)
        thread_monitors[req_id]->notify();
    thread_mutexes[req_id].unlock();

    if (IF_TRACE)
        logger(req_id, "ComposePage", "upload_thumbnail", "end");
}
void ComposePageHandler::upload_cast_info(const string& req_id, const string& movie_id, const vector<CastInfo> & cast_info) {
    if (IF_TRACE)
        logger(req_id, "ComposePage", "upload_cast_info", "begin");

    thread_mutexes[req_id].lock();
    pages[req_id].cast_info = cast_info;
    finished_counter[req_id]++;
    if (finished_counter[req_id] >= NUM_COMPONENTS)
        thread_monitors[req_id]->notify();
    thread_mutexes[req_id].unlock();

    if (IF_TRACE)
        logger(req_id, "ComposePage", "upload_cast_info", "end");
}
void ComposePageHandler::upload_photo(const string& req_id, const string& movie_id, const string& photo) {
    if (IF_TRACE)
        logger(req_id, "ComposePage", "upload_photo", "begin");

    thread_mutexes[req_id].lock();
    pages[req_id].photo = photo;
    finished_counter[req_id]++;
    if (finished_counter[req_id] >= NUM_COMPONENTS)
        thread_monitors[req_id]->notify();
    thread_mutexes[req_id].unlock();

    if (IF_TRACE)
        logger(req_id, "ComposePage", "upload_photo", "end");
}
void ComposePageHandler::upload_video(const string& req_id, const string& movie_id, const string& video) {
    if (IF_TRACE)
        logger(req_id, "ComposePage", "upload_video", "begin");

    thread_mutexes[req_id].lock();
    pages[req_id].video = video;
    finished_counter[req_id]++;
    if (finished_counter[req_id] >= NUM_COMPONENTS)
        thread_monitors[req_id]->notify();
    thread_mutexes[req_id].unlock();

    if (IF_TRACE)
        logger(req_id, "ComposePage", "upload_video", "end");
}
void ComposePageHandler::upload_movie_review(const string& req_id, const string& movie_id, const vector<Review> & reviews) {
    if (IF_TRACE)
        logger(req_id, "ComposePage", "upload_movie_review", "begin");

    thread_mutexes[req_id].lock();
    pages[req_id].reviews = reviews;
    finished_counter[req_id]++;
    if (finished_counter[req_id] >= NUM_COMPONENTS)
        thread_monitors[req_id]->notify();
    thread_mutexes[req_id].unlock();

    if (IF_TRACE)
        logger(req_id, "ComposePage", "upload_movie_review", "end");
}
void ComposePageHandler::upload_watch_next(const string& req_id, const string& movie_id, const vector<string> & watch_next) {
    if (IF_TRACE)
        logger(req_id, "ComposePage", "upload_watch_next", "begin");

    thread_mutexes[req_id].lock();
    pages[req_id].watch_next = watch_next;
    finished_counter[req_id]++;
    if (finished_counter[req_id] >= NUM_COMPONENTS)
        thread_monitors[req_id]->notify();
    thread_mutexes[req_id].unlock();

    if (IF_TRACE)
        logger(req_id, "ComposePage", "upload_watch_next", "end");
}

class ComposePageCloneFactory: virtual public ComposePageIfFactory {
public:
    virtual ~ComposePageCloneFactory() {}
    virtual ComposePageIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo)
    {
        boost::shared_ptr<TSocket> sock = boost::dynamic_pointer_cast<TSocket>(connInfo.transport);
        return new ComposePageHandler;
    }
    virtual void releaseHandler(ComposePageIf* handler) {
        delete handler;
    }
};

int main (int argc, char *argv[]) {
    IF_TRACE = true;
    LOG_PATH = "../logs/ComposePage.log";

    TThreadedServer server(
            boost::make_shared<ComposePageProcessorFactory>(boost::make_shared<ComposePageCloneFactory>()),
            boost::make_shared<TServerSocket>(SERVER_PORT_START + stoi(argv[1])),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;

}



