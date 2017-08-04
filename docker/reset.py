import subprocess as sp

NUM_USERS = 5
NUM_MOVIES = 5

MONGO_MOVIE_DB_PORT = 32000
MMC_MOVIE_DB_PORT = 32005

for i in range(NUM_MOVIES):
    sp.call("docker stop mongo-movie_db_" + str(i), shell = True)
    sp.call("docker stop mmc-movie_db_" + str(i), shell = True)

    sp.call("docker rm mongo-movie_db_" + str(i), shell = True)
    sp.call("docker rm mmc-movie_db_" + str(i), shell = True)

sp.call ("docker volume rm $(docker volume ls -f dangling=true -q)", shell = True)

for i in range(NUM_MOVIES):
    sp.call ("docker run -d -p " + str(MONGO_MOVIE_DB_PORT + i) + ":27017 --name mongo-movie_db_" + str(i) + " mongo",
             shell = True)

for i in range(NUM_MOVIES):
    sp.call ("docker run -d -p " + str(MMC_MOVIE_DB_PORT + i) + ":11211 --name mmc-movie_db_" + str(i) + " memcached",
             shell = True)