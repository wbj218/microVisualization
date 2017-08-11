import requests
import random

data = {}
data["user_id"] = "user_0"
data["movie_id"] = "movie_0 "
data["req_id"] = str(random.randint(0, 0xffffffff))

r = requests.get('http://192.168.99.100:32800/hls/user_0/' + data["req_id"] + '/movie_0.mp4')
print (r.content)