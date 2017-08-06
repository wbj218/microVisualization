import subprocess as sp

NUM_USERS = 5
NUM_MOVIES = 5

MONGO_MOVIE_DB_PORT = 32000
MMC_MOVIE_DB_PORT = 32005

MONGO_USER_DB_PORT = 32010
MMC_USER_DB_PORT = 32015

MONGO_REVIEW_STORAGE_PORT = 32020
MMC_REVIEW_STORAGE_PORT = 32021

for i in range(NUM_MOVIES):
    sp.call("docker stop mongo-movie_db_" + str(i), shell = True)
    sp.call("docker stop mmc-movie_db_" + str(i), shell = True)

    sp.call("docker rm mongo-movie_db_" + str(i), shell = True)
    sp.call("docker rm mmc-movie_db_" + str(i), shell = True)


for i in range(NUM_USERS):
    sp.call("docker stop mongo-user_db_" + str(i), shell = True)
    sp.call("docker stop mmc-user_db_" + str(i), shell = True)

    sp.call("docker rm mongo-user_db_" + str(i), shell = True)
    sp.call("docker rm mmc-user_db_" + str(i), shell = True)


sp.call("docker stop mongo-review_storage", shell = True)
sp.call("docker stop mmc-review_storage", shell = True)

sp.call("docker rm mongo-review_storage", shell = True)
sp.call("docker rm mmc-review_storage", shell = True)

sp.call ("docker volume rm $(docker volume ls -f dangling=true -q)", shell = True)

for i in range(NUM_MOVIES):
    sp.call ("docker run -d -p " + str(MONGO_MOVIE_DB_PORT + i) + ":27017 --name mongo-movie_db_" + str(i) + " mongo",
             shell = True)

for i in range(NUM_MOVIES):
    sp.call ("docker run -d -p " + str(MMC_MOVIE_DB_PORT + i) + ":11211 --name mmc-movie_db_" + str(i) + " memcached",
             shell = True)

for i in range(NUM_USERS):
    sp.call ("docker run -d -p " + str(MONGO_USER_DB_PORT + i) + ":27017 --name mongo-user_db_" + str(i) + " mongo",
             shell = True)

for i in range(NUM_USERS):
    sp.call ("docker run -d -p " + str(MMC_USER_DB_PORT + i) + ":11211 --name mmc-user_db_" + str(i) + " memcached",
             shell = True)


sp.call ("docker run -d -p " + str(MONGO_REVIEW_STORAGE_PORT) + ":27017 --name mongo-review_storage" + " mongo", shell = True)
sp.call ("docker run -d -p " + str(MMC_REVIEW_STORAGE_PORT) + ":11211 --name mmc-review_storage" + " memcached", shell = True)