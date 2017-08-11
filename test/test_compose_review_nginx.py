import requests
import random

data = {}
data["user_id"] = "user_0"
data["url"] = "http://www.imdb.com/title/movie_0"
data["req_id"] = data["user_id"] + " " +str(random.randint(0, 0xffffffff))
data["rating"] = '5' 
data["text"] = "It is a movie review! It is a movie review! It is a movie review! It is a movie review! It is a movie review! "

r = requests.post('http://128.253.128.64:32800/ComposeReview.php', data=data)
print (r.content)