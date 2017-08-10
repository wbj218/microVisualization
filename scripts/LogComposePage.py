import json
import operator


with open('../logs/Client_0.log', 'r') as file:
    client_json = json.load(file)

with open('../logs/Client_1.log', 'r') as file:
    client_json.update(json.load(file))

# with open('../logs/Client_2.log', 'r') as file:
#     client_json.update(json.load(file))

# with open('../logs/Client_3.log', 'r') as file:
#     client_json.update(json.load(file))

# with open('../logs/Client_4.log', 'r') as file:
#     client_json.update(json.load(file))

# with open('../logs/Client_5.log', 'r') as file:
#     client_json.update(json.load(file))

# with open('../logs/Client_6.log', 'r') as file:
#     client_json.update(json.load(file))

# with open('../logs/Client_7.log', 'r') as file:
#     client_json.update(json.load(file))




with open('../logs/ComposePage_0.log', 'r') as file:
    compose_page_json = json.load(file)

# with open('../logs/ComposePage_1.log', 'r') as file:
#     compose_page_json.update(json.load(file))

# with open('../logs/ComposePage_3.log', 'r') as file:
#     compose_page_json.update(json.load(file))

# with open('../logs/ComposePage_3.log', 'r') as file:
#     compose_page_json.update(json.load(file))


with open('../logs/GetCastInfo.log', 'r') as file:
    get_cast_json = json.load(file)

with open('../logs/GetPhoto.log', 'r') as file:
    get_photo_json = json.load(file)

with open('../logs/GetPlot.log', 'r') as file:
    get_plot_json = json.load(file)

with open('../logs/GetVideo.log', 'r') as file:
    get_video_json = json.load(file)

with open('../logs/GetThumbnail.log', 'r') as file:
    get_thumbnail_json = json.load(file)

with open('../logs/GetWatchNext.log', 'r') as file:
    get_watch_next_json = json.load(file)

with open('../logs/GetMovieReview.log', 'r') as file:
    get_review_json = json.load(file)

with open('../logs/GetRating.log', 'r') as file:
    get_rating_json = json.load(file)



with open('../logs/MovieInfoStorage_0.log', 'r') as file:
    movie_info_json = json.load(file)

# with open('../logs/MovieInfoStorage_1.log', 'r') as file:
#     movie_info_json.update(json.load(file))

with open('../logs/MovieReviewDB_0.log', 'r') as file:
    movie_review_db_json = json.load(file)

with open('../logs/ReviewStorage_0.log', 'r') as file:
    review_store_json = json.load(file)

with open('../logs/ProcessMovieID.log', 'r') as file:
    movie_id_json = json.load(file)


total_latency = {}
ngx_latency = {}
compose_page_latency = {}
get_cast_latency = {}
get_plot_latency = {}
get_photo_latency = {}
get_video_latency = {}
get_review_latency = {}
get_rating_latency = {}
get_watch_next_latency = {}
get_thumbnail_latency = {}
movie_info_latency = {}
movie_id_latency = {}
review_store_latency = {}
movie_review_db_latency = {}


for key, value in client_json.items():    
    ngx_latency[key] = movie_id_json[key]["ProcessMovieID"]["get_movie_id"]["begin"] - client_json[key]["begin"]

for key, value in movie_id_json.items():    
    movie_id_latency[key] = movie_id_json[key]["ProcessMovieID"]["get_movie_id"]["end"] -  movie_id_json[key]["ProcessMovieID"]["get_movie_id"]["begin"]

for key, value in compose_page_json.items():
    if "end" in compose_page_json[key]["ComposePage"]["compose_page"]:
        compose_page_latency[key] = compose_page_json[key]["ComposePage"]["compose_page"]["end"] - compose_page_json[key]["ComposePage"]["compose_page"]["begin"]

for key, value in get_cast_json.items():
    get_cast_latency[key] = get_cast_json[key]["GetCastInfo"]["get_cast_info"]["end"] - get_cast_json[key]["GetCastInfo"]["get_cast_info"]["begin"] 

for key, value in get_plot_json.items():
    get_plot_latency[key] = get_plot_json[key]["GetPlot"]["get_plot"]["end"] - get_plot_json[key]["GetPlot"]["get_plot"]["begin"]

for key, value in get_thumbnail_json.items():
    get_thumbnail_latency[key] = get_thumbnail_json[key]["GetThumbnail"]["get_thumbnail"]["end"] - get_thumbnail_json[key]["GetThumbnail"]["get_thumbnail"]["begin"]

for key, value in get_photo_json.items():
    get_photo_latency[key] = get_photo_json[key]["GetPhoto"]["get_photo"]["end"] - get_photo_json[key]["GetPhoto"]["get_photo"]["begin"]

for key, value in get_video_json.items():
    get_video_latency[key] = get_video_json[key]["GetVideo"]["get_video"]["end"] - get_video_json[key]["GetVideo"]["get_video"]["begin"]

for key, value in get_rating_json.items():
    get_rating_latency[key] = get_rating_json[key]["GetRating"]["get_rating"]["end"] - get_rating_json[key]["GetRating"]["get_rating"]["begin"]  

for key, value in get_watch_next_json.items():
    get_watch_next_latency[key] = get_watch_next_json[key]["GetWatchNext"]["get_watch_next"]["end"] - get_watch_next_json[key]["GetWatchNext"]["get_watch_next"]["begin"]

for key, value in get_review_json.items():
    get_review_latency[key] = get_review_json[key]["GetMovieReview"]["get_movie_review"]["end"] - get_review_json[key]["GetMovieReview"]["get_movie_review"]["begin"]  

for key, value in movie_info_json.items():
    # if "get_cast" in movie_info_json[key]["MovieInfoStorage"] and "get_plot" in movie_info_json[key]["MovieInfoStorage"] and \
    # "get_thumbnail" in movie_info_json[key]["MovieInfoStorage"] and "get_rating " in movie_info_json[key]["MovieInfoStorage"]:
    movie_info_latency[key] = (movie_info_json[key]["MovieInfoStorage"]["get_cast"]["end"] - movie_info_json[key]["MovieInfoStorage"]["get_cast"]["begin"] + 
                         movie_info_json[key]["MovieInfoStorage"]["get_plot"]["end"] - movie_info_json[key]["MovieInfoStorage"]["get_plot"]["begin"] + 
                         movie_info_json[key]["MovieInfoStorage"]["get_thumbnail"]["end"] - movie_info_json[key]["MovieInfoStorage"]["get_thumbnail"]["begin"] + 
                         movie_info_json[key]["MovieInfoStorage"]["get_rating"]["end"] - movie_info_json[key]["MovieInfoStorage"]["get_rating"]["begin"]) / 4

for key, value in movie_review_db_json.items():
    movie_review_db_latency[key] = movie_review_db_json[key]["MovieReviewDB"]["get_movie_review"]["end"] - movie_review_db_json[key]["MovieReviewDB"]["get_movie_review"]["begin"]
    
for key, value in review_store_json.items():
    review_store_latency[key] = review_store_json[key]["ReviewStorage"]["get_review"]["end"] - review_store_json[key]["ReviewStorage"]["get_review"]["begin"]

for key, value in client_json.items():
    if "end" in client_json[key]:
        total_latency[key] = client_json[key]["end"] - client_json[key]["begin"]


avg_total_latency = 0
avg_ngx_latency = 0
avg_compose_page_latency = 0
avg_get_cast_latency = 0
avg_get_plot_latency = 0
avg_get_photo_latency = 0
avg_get_video_latency = 0
avg_get_review_latency = 0
avg_get_rating_latency = 0
avg_get_watch_next_latency = 0
avg_get_thumbnail_latency = 0
avg_movie_info_latency = 0
avg_movie_id_latency = 0
avg_review_store_latency = 0
avg_movie_review_db_latency = 0

for key, value in total_latency.items():
    avg_total_latency += value

for key, value in ngx_latency.items():
    avg_ngx_latency += value

for key, value in compose_page_latency.items():
    avg_compose_page_latency += value

for key, value in get_plot_latency.items():
    avg_get_plot_latency += value

for key, value in get_thumbnail_latency.items():
    avg_get_thumbnail_latency += value

for key, value in get_cast_latency.items():
    avg_get_cast_latency += value

for key, value in get_photo_latency.items():
    avg_get_photo_latency += value

for key, value in get_video_latency.items():
    avg_get_video_latency += value

for key, value in get_rating_latency.items():
    avg_get_rating_latency += value

for key, value in get_review_latency.items():
    avg_get_review_latency += value

for key, value in get_watch_next_latency.items():
    avg_get_watch_next_latency += value

for key, value in movie_id_latency.items():
    avg_movie_id_latency += value

for key, value in movie_info_latency.items():
    avg_movie_info_latency += value

for key, value in movie_review_db_latency.items():
    avg_movie_review_db_latency += value

for key, value in review_store_latency.items():
    avg_review_store_latency += value

print ("n_reqs = ", len(total_latency))

print (avg_ngx_latency / len(ngx_latency))
print (avg_movie_id_latency / len(movie_id_latency))

print (avg_get_cast_latency / len(get_cast_latency))
print (avg_get_photo_latency / len(get_photo_latency))
print (avg_get_plot_latency / len(get_plot_latency))
print (avg_get_rating_latency / len(get_rating_latency))
print (avg_get_review_latency / len(get_review_latency))
print (avg_get_thumbnail_latency / len(get_thumbnail_latency))
print (avg_get_video_latency / len(get_video_latency))
print (avg_get_watch_next_latency / len(get_watch_next_latency))

print (avg_movie_review_db_latency / len(movie_review_db_latency))
print (avg_movie_info_latency / len(movie_info_latency))
print (avg_review_store_latency / len(review_store_latency))

print (avg_compose_page_latency / len(compose_page_latency))
print (avg_total_latency / len(total_latency))
