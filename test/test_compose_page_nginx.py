import requests
import random

data = {}
data["user_id"] = "user_0"
data["url"] = "http://www.imdb.com/title/movie_0"
data["req_id"] = data["user_id"] + " " +str(random.randint(0, 0xffffffff))

r = requests.get('http://192.168.99.100:32800/GetPage.php', params=data)
print (r.content)