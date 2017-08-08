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


using namespace NetflixMicroservices;

map<string, Mutex> thread_mutexes;
map<string, MoviePage> pages;
map<string, Monitor> thread_monitors;

json logs;

bool IF_TRACE;
string LOG_PATH;

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
    
}
void ComposePageHandler::upload_plot(const string& req_id, const string& movie_id, const string& plot) {
    
}
void ComposePageHandler::upload_rating(const string& req_id, const string& movie_id, const string& rating) {

}
void ComposePageHandler::upload_thumbnail(const string& req_id, const string& movie_id, const string& thumbnail){

}
void ComposePageHandler::upload_cast_info(const string& req_id, const string& movie_id, const vector<CastInfo> & cast_info) {

}
void ComposePageHandler::upload_photo(const string& req_id, const string& movie_id, const string& photo) {

}
void ComposePageHandler::upload_video(const string& req_id, const string& movie_id, const string& video) {

}
void ComposePageHandler::upload_movie_review(const string& req_id, const string& movie_id, const vector<Review> & reviews) {

}
void ComposePageHandler::upload_watch_next(const string& req_id, const string& movie_id, const vector<string> & watch_next) {

}

int main (int argc, char *argv[]) {
    IF_TRACE = true;
    LOG_PATH = "../logs/ComposePage.log";

    TThreadedServer server(
            boost::make_shared<ComposePageProcessor>(boost::make_shared<ComposePageHandler>()),
            boost::make_shared<TServerSocket>(SERVER_PORT_START + stoi(argv[1])),
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;

}



