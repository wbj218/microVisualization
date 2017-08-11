import requests
import random

data = {}
data["user_id"] = "user_0"
data["movie_id"] = "movie_4"
data["req_id"] = str(random.randint(0, 0xffffffff))

r = requests.get('http://128.253.128.64:32800/hls/user_0/' + data["req_id"] + '/movie_4.mp4')
print (r.content)