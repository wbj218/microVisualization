namespace cpp NetflixMicroservices
namespace php NetflixMicroservices
namespace py NetflixMicroservices

struct Review {
    1: string unique_id
    2: string user_id
    3: string text
    4: string rating
    5: string movie_id
    6: string req_id
}

service ProcessUniqueID {
    void ping (),
    oneway void get_unique_id(1:string req_id)
}

service ProcessMovieID {
    void ping (),
    oneway void get_movie_id(1:string req_id, 2:string url)
}

service ProcessText {
    void ping(),
    oneway void process_text(1:string req_id, 2:string text_data)
}

service AssignRating {
    void ping(),
    oneway void assign_rating(1:string req_id, 2:string rating)
}

service ComposeReview {
    void ping(),
    oneway void upload(1:string req_id, 2:string type, 3:string data)

}

service MovieReviewDB {
    void ping(),
    oneway void write_movie_review(1:string req_id, 2:string movie_id, 3:string user_id, 4:string unique_id, 5:string rating)
    string get_movie_review(1:string req_id, 2:string movie_id, 3:i32 begin_no, 4:i32 num)
}

service UserReviewDB {
    void ping(),
    oneway void write_user_review(1:string req_id, 2:string movie_id, 3:string user_id, 4:string unique_id)

}

service ReviewStorage {
    void ping(),
    oneway void review_storage(1:string req_id, 2:Review review)
    Review get_review(1:string req_id, 2:string movie_id, 3:string unique_id)

}

service MovieInfoStorage {
    void ping(),
    string get_info(1:string req_id, 2:string movie_id, 3:string type)

}

service GetPlot {
    void ping(),
    string get_plot(1:string req_id, 2:string movie_id)

}

service GetThumbnail {
    void ping(),
    string get_thumbnail(1:string req_id, 2:string movie_id)
}

service GetRating {
    void ping(),
    string get_rating(1:string req_id, 2:string movie_id)
}

service GetCastInfo {
    void ping(),
    string get_cast_info(1:string req_id, 2:string movie_id)
}

service GetPhoto {
    void ping(),
    string get_photo(1:string req_id, 2:string movie_id)
}

service GetVideo {
    void ping(),
    string get_video(1:string req_id, 2:string movie_id)
}

service GetMovieReview {
    void ping(),
    list<Review> get_movie_review(1:string req_id, 2:string movie_id, 3:i32 begin_no, 4:i32 num)
}

service GetWatchNext {
    void ping(),
    list<string> get_watch_next(1:string req_id, 2:string user_id)
}