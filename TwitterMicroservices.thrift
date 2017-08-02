namespace cpp TwitterMicroservices
namespace php TwitterMicroservices
namespace py TwitterMicroservices




struct Tweet {
    1: string unique_id
    2: string user_id
    3: string text
    4: string image
    5: string video
    6: string tags
    7: string urls
    8: string tweet_type
    9:string recipient_id
    10:string orig_tweet_id
    11:string retweet_text
    12:string retweet_tags
    13:string retweet_urls
    14:string orig_user_id
    15:string req_id

}

service ProcessUniqueID {
    void ping (),
    oneway void get_unique_id(1:string req_id, 2:string tweet_type)
}

service ProcessURL {
    void ping(),
    oneway void process_url(1:string req_id, 2:string text_data, 3:string type)
}

service ProcessTag {
    void ping(),
    oneway void process_tag(1:string req_id, 2:string text_data, 3:string type)
}

service ProcessText {
    void ping(),
    oneway void process_text(1:string req_id, 2:string text_data, 3:string type)
}

service ProcessImage {
    void ping(),
    oneway void process_image(1:string req_id, 2:string image_data, 3:string type)
}

service ProcessVideo {
    void ping(),
    oneway void process_video(1:string req_id, 2:string video_data, 3:string type)
}

service ComposeTweet {
    void ping(),
    oneway void upload(1:string req_id, 2:string type, 3:string data)

}

service ComposeReply {
    void ping(),
    oneway void upload(1:string req_id, 2:string type, 3:string data)

}

service ComposeRetweet {
    void ping(),
    oneway void upload(1:string req_id, 2:string type, 3:string data)
    oneway void upload_tweet(1:Tweet tweet, 2: string req_id)

}

service ComposeDM {
    void ping(),
    oneway void upload(1:string req_id, 2:string type, 3:string data)


}



service MongoTweetStorage {
    void    ping(),
    oneway void    store_tweet(1:Tweet tweet, 2:string req_id)
    string  get_tweet(1:string user_id, 2:string unique_id, 3:string req_id)
}

service MongoDMStorage {
    void    ping(),
    oneway void store_dm(1:Tweet tweet, 2:string req_id)
}

service WriteUserGraph {
    void    ping(),
    oneway void write_graph(1:string user_id, 2:string unique_id, 3:string req_id)
}

service WriteTimeline {
    void    ping(),
    oneway void write_timeline(1:string user_id, 2:string unique_id, 3:string req_id, 4:string type)
}

service WriteFavorite {
    void    ping(),
    oneway void write_favorite(1:string user_id, 2:string unique_id, 3:string author_id, 4:string req_id)
}

service WriteDM {
     void    ping(),
     oneway void write_dm(1:string user_id, 2:string unique_id, 3:string req_id)
}

service ReadTweet {
     void    ping(),
     Tweet read_tweet(1:string user_id, 2:string unique_id, 3:string type, 4:string req_id)

}

service ReadTimeline {
    void    ping(),
    list<string> read_timeline(1:string user_id, 2:i32 start_no, 3:i32 n_tweets, 4:string type, 5:string req_id)

}

service BlockStats {
    void    ping(),
    bool if_blocked(1:string user1_id, 2:string user2_id, 3:string req_id),
    oneway void add_block(1:string user1_id, 2:string user2_id),
    oneway void remove_block(1:string user1_id, 2:string user2_id),


}



service Xapian {
    void ping(),
    string search(1:string key, 2:string req_id)
}

service Generator {
    void ping(),
    oneway void post(),
    oneway void response(1:string req_id, 2:string stage),
    oneway void shutdown()

}



service ServerControl {
    void ping(),
    oneway void shundown()
}



