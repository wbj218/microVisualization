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

service WriteMovieDB {
    void ping(),
    oneway void write_movie_db(1:string req_id, 2:string movie_id, 3:string user_id, 4:string unique_id)

}

service WriteUserDB {
    void ping(),
    oneway void write_user_db(1:string req_id, 2:string movie_id, 3:string user_id, 4:string unique_id)

}