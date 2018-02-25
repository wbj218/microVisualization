import subprocess as sp
import json

with open("../config/config.json", "r") as file:
    config = json.load(file)

NUM_USERS = config["num_user"]
NUM_MOVIES = config["num_movie"]

MONGO_MOVIE_DB_PORT = config["docker_mongo_movie_db"]["port"]
MMC_MOVIE_DB_PORT = config["docker_mmc_movie_db"]["port"]

MONGO_USER_DB_PORT = config["docker_mongo_user_db"]["port"]
MMC_USER_DB_PORT = config["docker_mmc_user_db"]["port"]

MONGO_REVIEW_STORAGE_PORT = config["docker_mongo_review_store"]["port"]
MMC_REVIEW_STORAGE_PORT = config["docker_mmc_review_store"]["port"]

NGINX_PORT = config["docker_nginx_php"]["port"]

USERNAME = config["username"]


for i in range(NUM_MOVIES):
    sp.call("docker stop " + USERNAME + "_mongo-movie_db_" + str(i), shell = True)
    sp.call("docker stop " + USERNAME + "_mmc-movie_db_" + str(i), shell = True)
    sp.call("docker rm " + USERNAME + "_mongo-movie_db_" + str(i), shell = True)
    sp.call("docker rm " + USERNAME + "_mmc-movie_db_" + str(i), shell = True)


for i in range(NUM_USERS):
    sp.call("docker stop " + USERNAME + "_mongo-user_db_" + str(i), shell = True)
    sp.call("docker stop " + USERNAME + "_mmc-user_db_" + str(i), shell = True)
    sp.call("docker rm " + USERNAME + "_mongo-user_db_" + str(i), shell = True)
    sp.call("docker rm " + USERNAME + "_mmc-user_db_" + str(i), shell = True)

sp.call("docker stop " + USERNAME + "_mongo-review_storage", shell = True)
sp.call("docker stop " + USERNAME + "_mmc-review_storage", shell = True)

sp.call("docker rm " + USERNAME + "_mongo-review_storage", shell = True)
sp.call("docker rm " + USERNAME + "_mmc-review_storage", shell = True)

sp.call("docker stop " + USERNAME + "_nginx-php-fpm", shell = True)
sp.call("docker rm " + USERNAME + "_nginx-php-fpm", shell = True)

sp.call ("docker volume rm $(docker volume ls -f dangling=true -q)", shell = True)

for i in range(NUM_MOVIES):
    sp.call ("docker run -d -p " + str(MONGO_MOVIE_DB_PORT + i) + ":27017 --name "
             + USERNAME + "_mongo-movie_db_" + str(i) + " yg397/sail_mongodb",
             shell = True)

for i in range(NUM_MOVIES):
    sp.call ("docker run -d -p " + str(MMC_MOVIE_DB_PORT + i) + ":11211 --name "
             + USERNAME + "_mmc-movie_db_" + str(i) + " yg397/sail_memcached -m 512m -I 64m",
             shell = True)

for i in range(NUM_USERS):
    sp.call ("docker run -d -p " + str(MONGO_USER_DB_PORT + i) + ":27017 --name "
             + USERNAME + "_mongo-user_db_" + str(i) + " yg397/sail_mongodb",
             shell = True)

for i in range(NUM_USERS):
    sp.call ("docker run -d -p " + str(MMC_USER_DB_PORT + i) + ":11211 --name "
             + USERNAME + "_mmc-user_db_" + str(i) + " yg397/sail_memcached -m 512m -I 64m",
             shell = True)

sp.call ("docker run -d -p " + str(MONGO_REVIEW_STORAGE_PORT) + ":27017 --name "
         + USERNAME + "_mongo-review_storage" + " yg397/sail_mongodb", shell = True)
sp.call ("docker run -d -p " + str(MMC_REVIEW_STORAGE_PORT)
         + ":11211 --name " + USERNAME + "_mmc-review_storage" + " yg397/sail_memcached -m 512m -I 64m", shell = True)

sp.call ("docker run --cpuset-cpus=19 -d -p "  + str(NGINX_PORT) + ":80 --name " + USERNAME
         + "_nginx-php-fpm -v $PWD/../nginx-php/html:/var/www/html -v $PWD/../nginx-php/conf:/etc/nginx/sites-enabled "
         + "yg397/sail_nginx-php-fpm:latest", shell = True)

