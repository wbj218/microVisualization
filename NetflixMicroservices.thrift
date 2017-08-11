namespace cpp NetflixMicroservices
namespace php NetflixMicroservices
namespace py NetflixMicroservices

struct Review {
    1: string unique_id;
    2: string user_id;
    3: string text;
    4: string rating;
    5: string movie_id;
    6: string req_id;
}

struct CastInfo {
    1: string cast_id;
    2: string info;
}

struct MoviePage {
    1: string movie_id;
    2: string user_id;
    3: string req_id;
    4: list<CastInfo> cast_info;
    5: string plot;
    6: string thumbnail;
    7: string rating;
    8: list<string> watch_next;
    9: list<Review> reviews;
    10: string photo;
    11: string video;
}

service ProcessUniqueID {
    void ping (),
    oneway void get_unique_id(1:string req_id)
}

service ProcessMovieID {
    void ping (),
    oneway void process_movie_id(1:string req_id, 2:string url)
    string get_movie_id(1:string req_id, 2:string url)
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
    oneway void write_movie_review(1:string req_id, 2:string movie_id, 3:string user_id, 4:string unique_id, 5:string rating),
    string get_movie_review(1:string req_id, 2:string movie_id, 3:i32 begin_no, 4:i32 num)
}

service UserReviewDB {
    void ping(),
    oneway void write_user_review(1:string req_id, 2:string movie_id, 3:string user_id, 4:string unique_id)

}

service ReviewStorage {
    void ping(),
    oneway void review_storage(1:string req_id, 2:Review review),
    Review get_review(1:string req_id, 2:string movie_id, 3:string unique_id)

}

service MovieInfoStorage {
    void ping(),
    string get_info(1:string req_id, 2:string movie_id, 3:string type)

}

service GetPlot {
    void ping(),
    oneway void get_plot(1:string req_id, 2:string movie_id, 3:i32 server_no)

}

service GetThumbnail {
    void ping(),
    oneway void get_thumbnail(1:string req_id, 2:string movie_id, 3:i32 server_no)
}

service GetRating {
    void ping(),
    oneway void get_rating(1:string req_id, 2:string movie_id, 3:i32 server_no)
}

service GetCastInfo {
    void ping(),
    oneway void get_cast_info(1:string req_id, 2:string movie_id, 3:i32 server_no)
}

service GetPhoto {
    void ping(),
    oneway void get_photo(1:string req_id, 2:string movie_id, 3:i32 server_no)
}

service GetVideo {
    void ping(),
    oneway void get_video(1:string req_id, 2:string movie_id, 3:i32 server_no)
}

service GetMovieReview {
    void ping(),
    oneway void get_movie_review(1:string req_id, 2:string movie_id, 3:i32 begin_no, 4:i32 num, 5:i32 server_no)
}

service GetWatchNext {
    void ping(),
    oneway void get_watch_next(1:string req_id, 2:string user_id, 3:i32 server_no)
}

service ComposePage {
    void ping(),
    MoviePage compose_page (1:string req_id, 2:string movie_id, 3:string user_id),
    oneway void upload_plot (1:string req_id, 2:string movie_id, 3:string plot),
    oneway void upload_rating (1:string req_id, 2:string movie_id, 3:string rating),
    oneway void upload_thumbnail (1:string req_id, 2:string movie_id, 3:string thumbnail),
    oneway void upload_cast_info (1:string req_id, 2:string movie_id, 3:list<CastInfo> cast_info),
    oneway void upload_photo (1:string req_id, 2:string movie_id, 3:string photo),
    oneway void upload_video (1:string req_id, 2:string movie_id, 3:string video),
    oneway void upload_movie_review (1:string req_id, 2:string movie_id, 3:list<Review> reviews),
    oneway void upload_watch_next (1:string req_id, 2:string user_id, 3:list<string> watch_next)
}

service UserAccount {
    void ping(),
    bool if_purchased(1:string req_id, 2:string user_id, 3:string movie_id),
    bool purchase(1:string req_id, 2:string user_id, 3:string movie_id),
    void add_account(1:string req_id, 2:string user_id, 3:i32 amount)

}



service Generator {
    void ping(),
    oneway void run(),
    oneway void shutdown()
}