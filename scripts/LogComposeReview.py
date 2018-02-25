import json
import operator
import numpy as np


with open('../logs/Client_0.log', 'r') as file:
    client_json = json.load(file)

with open('../logs/Client_1.log', 'r') as file:
    client_json.update(json.load(file))

with open('../logs/Client_2.log', 'r') as file:
    client_json.update(json.load(file))

with open('../logs/Client_3.log', 'r') as file:
    client_json.update(json.load(file))

# with open('../logs/Client_4.log', 'r') as file:
#     client_json.update(json.load(file))

# with open('../logs/Client_5.log', 'r') as file:
#     client_json.update(json.load(file))

# with open('../logs/Client_6.log', 'r') as file:
#     client_json.update(json.load(file))

# with open('../logs/Client_7.log', 'r') as file:
#     client_json.update(json.load(file))




with open('../logs/ComposeReview.log', 'r') as file:
    compose_review_json = json.load(file)
if compose_review_json is None:
    compose_review_json = {}

with open('../logs/ProcessMovieID.log', 'r') as file:
    movie_id_json = json.load(file)
if movie_id_json is None:
    movie_id_json = {}

with open('../logs/AssignRating.log', 'r') as file:
    rating_json = json.load(file)
if rating_json is None:
    rating_json = {}

with open('../logs/ProcessUniqueID.log', 'r') as file:
    unique_id_json = json.load(file)
if unique_id_json is None:
    unique_id_json = {}

with open('../logs/ProcessText.log', 'r') as file:
    text_json = json.load(file)
if text_json is None:
    text_json = {}

with open('../logs/MovieReviewDB_0.log', 'r') as file:
    movie_review_db_json = json.load(file)
if movie_review_db_json is None:
    movie_review_db_json = {}

with open('../logs/UserReviewDB_0.log', 'r') as file:
    user_review_db_json = json.load(file)
if user_review_db_json is None:
    user_review_db_json = {}

with open('../logs/ReviewStorage_0.log', 'r') as file:
    review_store_json = json.load(file)
if review_store_json is None:
    review_store_json = {}



total_latency = {}
ngx_latency = {}

movie_id_latency = {}
rating_latency = {}
text_latency = {}
unique_id_latency = {}

compose_review_latency = {}

review_store_latency = {}
movie_review_db_latency = {}
user_review_db_latency = {}


for key, value in client_json.items():
    if key in movie_id_json:    
        ngx_latency[key] = movie_id_json[key]["ProcessMovieID"]["process_movie_id"]["begin"] - client_json[key]["begin"]
for key, value in movie_id_json.items():    
    movie_id_latency[key] = movie_id_json[key]["ProcessMovieID"]["process_movie_id"]["end"] -  movie_id_json[key]["ProcessMovieID"]["process_movie_id"]["begin"]

for key, value in rating_json.items():    
    rating_latency[key] = rating_json[key]["AssignRating"]["assign_rating"]["end"] -  rating_json[key]["AssignRating"]["assign_rating"]["begin"]

for key, value in text_json.items():    
    text_latency[key] = text_json[key]["ProcessText"]["process_text"]["end"] -  text_json[key]["ProcessText"]["process_text"]["begin"]

for key, value in unique_id_json.items():    
    unique_id_latency[key] = unique_id_json[key]["ProcessUniqueID"]["process_unique_id"]["end"] -  unique_id_json[key]["ProcessUniqueID"]["process_unique_id"]["begin"]


for key, value in compose_review_json.items():    
    compose_review_latency[key] = max(compose_review_json[key]["ComposeReview"]["compose_movie_id"]["end"] - compose_review_json[key]["ComposeReview"]["compose_movie_id"]["begin"], 
                                         compose_review_json[key]["ComposeReview"]["compose_text"]["end"] - compose_review_json[key]["ComposeReview"]["compose_text"]["begin"],
                                         compose_review_json[key]["ComposeReview"]["compose_rating"]["end"] - compose_review_json[key]["ComposeReview"]["compose_rating"]["begin"],
                                         compose_review_json[key]["ComposeReview"]["compose_unique_id"]["end"] - compose_review_json[key]["ComposeReview"]["compose_unique_id"]["begin"]
                                         ) 

for key, value in movie_review_db_json.items():
    movie_review_db_latency[key] = movie_review_db_json[key]["MovieReviewDB"]["write_movie_review"]["end"] - movie_review_db_json[key]["MovieReviewDB"]["write_movie_review"]["begin"]
    
for key, value in user_review_db_json.items():
    user_review_db_latency[key] = user_review_db_json[key]["UserReviewDB"]["write_user_db"]["end"] - user_review_db_json[key]["UserReviewDB"]["write_user_db"]["begin"]
    

for key, value in review_store_json.items():
    review_store_latency[key] = review_store_json[key]["ReviewStorage"]["review_storage"]["end"] - review_store_json[key]["ReviewStorage"]["review_storage"]["begin"]

for key, value in client_json.items():
    total_latency[key] = {}
    if key in movie_review_db_json and 'end' in movie_review_db_json[key]["MovieReviewDB"]["write_movie_review"]:
        total_latency[key]["movie_db"] = movie_review_db_json[key]["MovieReviewDB"]["write_movie_review"]["end"] - value["begin"]

    if key in user_review_db_json and 'end' in user_review_db_json[key]["UserReviewDB"]["write_user_db"]:
        total_latency[key]["user_db"] = user_review_db_json[key]["UserReviewDB"]["write_user_db"]["end"] - value["begin"]

    if key in movie_review_db_json and 'end' in review_store_json[key]["ReviewStorage"]["review_storage"]:
        total_latency[key]["review_store"] = review_store_json[key]["ReviewStorage"]["review_storage"]["end"] - value["begin"]

        total_latency[key]["max"] =  max([v for v in total_latency[key].values()])
    

avg_total_latency = 0
avg_ngx_latency = 0

avg_rating_latency = 0
avg_text_latency = 0
avg_unique_id_latency = 0
avg_movie_id_latency = 0

avg_compose_review_latency = 0

avg_review_store_latency = 0
avg_movie_review_db_latency = 0
avg_user_review_db_latency = 0

for key, value in total_latency.items():
    if 'max' in value:
        avg_total_latency += value["max"]

for key, value in ngx_latency.items():
    avg_ngx_latency += value


for key, value in rating_latency.items():
    avg_rating_latency += value

for key, value in text_latency.items():
    avg_text_latency += value

for key, value in unique_id_latency.items():
    avg_unique_id_latency += value

for key, value in movie_id_latency.items():
    avg_movie_id_latency += value



for key, value in compose_review_latency.items():
    avg_compose_review_latency += value


for key, value in review_store_latency.items():
    avg_review_store_latency += value

for key, value in movie_review_db_latency.items():
    avg_movie_review_db_latency += value

for key, value in user_review_db_latency.items():
    avg_user_review_db_latency += value




print ("n_reqs =", len(total_latency))

print ("avg_ngx_latency =", avg_ngx_latency / len(ngx_latency))

print ("avg_movie_id_latency =", avg_movie_id_latency / len(movie_id_latency))
print ("avg_rating_latency =", avg_rating_latency / len(rating_latency))
print ("avg_text_latency =", avg_text_latency / len(text_latency))
print ("avg_unique_id_latency =", avg_unique_id_latency / len(unique_id_latency))

print ("avg_compose_review_latency =", avg_compose_review_latency / len(compose_review_latency))

print ("avg_review_store_latency =", avg_review_store_latency / len(review_store_latency))
print ("avg_movie_review_db_latency =", avg_movie_review_db_latency / len(movie_review_db_latency))
print ("avg_user_review_db_latency =", avg_user_review_db_latency / len(user_review_db_latency))


print ("avg_total_latency (us) =", avg_total_latency / len(total_latency))
