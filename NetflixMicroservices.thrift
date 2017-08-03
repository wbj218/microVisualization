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

service ProcessRating {
    void ping(),
    oneway void process_text(1:string req_id, 2:string text_data)
}

service ComposeReview {
    void ping(),
    oneway void upload(1:string req_id, 2:string type, 3:string data)

}