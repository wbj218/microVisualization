import subprocess as sp

NUM_USERS = 5
NUM_MOVIES = 5

MONGO_MOVIE_DB_PORT = 42000
MMC_MOVIE_DB_PORT = 42005

MONGO_USER_DB_PORT = 42010
MMC_USER_DB_PORT = 42015

MONGO_REVIEW_STORAGE_PORT = 42020
MMC_REVIEW_STORAGE_PORT = 42021

NGINX_PORT = 42800

# MONGO_MOVIE_INFO_PORT = 32022
# MMC_MOVIE_INFO_PORT = 32023

# MONGO_CAST_INFO_PORT = 32024
# MMC_CAST_INFO_PORT = 32025

# MONGO_PHOTO_PORT = 32026
# MMC_PHOTO_PORT = 32027

# MONGO_VIDEO_PORT = 32028
# MMC_VIDEO_PORT = 32029

for i in range(NUM_MOVIES):
    sp.call("docker stop sh2442_mongo-movie_db_" + str(i), shell = True)
    sp.call("docker stop sh2442_mmc-movie_db_" + str(i), shell = True)

    sp.call("docker rm sh2442_mongo-movie_db_" + str(i), shell = True)
    sp.call("docker rm sh2442_mmc-movie_db_" + str(i), shell = True)


for i in range(NUM_USERS):
    sp.call("docker stop sh2442_mongo-user_db_" + str(i), shell = True)
    sp.call("docker stop sh2442_mmc-user_db_" + str(i), shell = True)

    sp.call("docker rm sh2442_mongo-user_db_" + str(i), shell = True)
    sp.call("docker rm sh2442_mmc-user_db_" + str(i), shell = True)


sp.call("docker stop sh2442_mongo-review_storage", shell = True)
sp.call("docker stop sh2442_mmc-review_storage", shell = True)


# sp.call("docker stop mongo-movie_info", shell = True)
# sp.call("docker stop mmc-movie_info", shell = True)

# sp.call("docker stop mongo-cast_info", shell = True)
# sp.call("docker stop mmc-cast_info", shell = True)

# sp.call("docker stop mongo-photo", shell = True)
# sp.call("docker stop mmc-photo", shell = True)

# sp.call("docker stop mongo-video", shell = True)
# sp.call("docker stop mmc-video", shell = True)

sp.call("docker rm sh2442_mongo-review_storage", shell = True)
sp.call("docker rm sh2442_mmc-review_storage", shell = True)

# sp.call("docker rm mongo-movie_info", shell = True)
# sp.call("docker rm mmc-movie_info", shell = True)

# sp.call("docker rm mongo-cast_info", shell = True)
# sp.call("docker rm mmc-cast_info", shell = True)

# sp.call("docker rm mongo-photo", shell = True)
# sp.call("docker rm mmc-photo", shell = True)

# sp.call("docker rm mongo-video", shell = True)
# sp.call("docker rm mmc-video", shell = True)
sp.call("docker stop sh2442_nginx-php-fpm", shell = True)
sp.call("docker rm sh2442_nginx-php-fpm", shell = True)

sp.call ("docker volume rm $(docker volume ls -f dangling=true -q)", shell = True)

for i in range(NUM_MOVIES):
    sp.call ("docker run -d -p " + str(MONGO_MOVIE_DB_PORT + i) + ":27017 --name sh2442_mongo-movie_db_" + str(i) + " yg397/sail_mongodb",
             shell = True)

for i in range(NUM_MOVIES):
    sp.call ("docker run -d -p " + str(MMC_MOVIE_DB_PORT + i) + ":11211 --name sh2442_mmc-movie_db_" + str(i) + " yg397/sail_memcached",
             shell = True)

for i in range(NUM_USERS):
    sp.call ("docker run -d -p " + str(MONGO_USER_DB_PORT + i) + ":27017 --name sh2442_mongo-user_db_" + str(i) + " yg397/sail_mongodb",
             shell = True)

for i in range(NUM_USERS):
    sp.call ("docker run -d -p " + str(MMC_USER_DB_PORT + i) + ":11211 --name sh2442_mmc-user_db_" + str(i) + " yg397/sail_memcached",
             shell = True)

sp.call ("docker run -d -p " + str(MONGO_REVIEW_STORAGE_PORT) + ":27017 --name sh2442_mongo-review_storage" + " yg397/sail_mongodb", shell = True)
sp.call ("docker run -d -p " + str(MMC_REVIEW_STORAGE_PORT) + ":11211 --name sh2442_mmc-review_storage" + " yg397/sail_memcached", shell = True)

# sp.call ("docker run -d -p " + str(MONGO_MOVIE_INFO_PORT) + ":27017 --name mongo-movie_info" + " mongo", shell = True)
# sp.call ("docker run -d -p " + str(MMC_MOVIE_INFO_PORT) + ":11211 --name mmc-movie_info" + " memcached", shell = True)

# sp.call ("docker run -d -p " + str(MONGO_CAST_INFO_PORT) + ":27017 --name mongo-cast_info" + " mongo", shell = True)
# sp.call ("docker run -d -p " + str(MMC_CAST_INFO_PORT) + ":11211 --name mmc-cast_info" + " memcached", shell = True)

# sp.call ("docker run -d -p " + str(MONGO_PHOTO_PORT) + ":27017 --name mongo-photo" + " mongo", shell = True)
# sp.call ("docker run -d -p " + str(MMC_PHOTO_PORT) + ":11211 --name mmc-photo" + " memcached", shell = True)

# sp.call ("docker run -d -p " + str(MONGO_VIDEO_PORT) + ":27017 --name mongo-video" + " mongo", shell = True)
# sp.call ("docker run -d -p " + str(MMC_VIDEO_PORT) + ":11211 --name mmc-video" + " memcached", shell = True)

sp.call ("docker run --cpuset-cpus=19 -d -p"  + str(NGINX_PORT) + ":80 --name sh2442_nginx-php-fpm -v $PWD/../nginx-php/html:/var/www/html -v $PWD/../nginx-php/conf:/etc/nginx/sites-enabled yg397/sail_nginx-php-fpm:latest", shell = True)

