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

for i in range(NUM_MOVIES):
    sp.call("sudo tcpdump -i em1 -A -tt -nn port" +  str(MMC_MOVIE_DB_PORT + i)
            + " > ./logs/mmc_movie_db_" + str(i) + ".log &", shell = True)
    sp.call("sudo tcpdump -i em1 -A -tt -nn port" +  str(MONGO_MOVIE_DB_PORT + i)
            + " > ./logs/mongo_movie_db_" + str(i) + ".log &", shell = True)

for i in range(NUM_USERS):
    sp.call("sudo tcpdump -i em1 -A -tt -nn port" +  str(MMC_USER_DB_PORT + i)
            + " > ./logs/mmc_user_db_" + str(i) + ".log &", shell = True)
    sp.call("sudo tcpdump -i em1 -A -tt -nn port" +  str(MONGO_USER_DB_PORT + i)
            + " > ./logs/mongo_user_db_" + str(i) + ".log &", shell = True)

sp.call("sudo tcpdump -i em1 -A -tt -nn port" +  str(MMC_REVIEW_STORAGE_PORT)
        + " > ./logs/mmc_review_storage.log &", shell = True)
sp.call("sudo tcpdump -i em1 -A -tt -nn port" +  str(MONGO_REVIEW_STORAGE_PORT)
        + " > ./logs/mongo_review_storage.log &", shell = True)
