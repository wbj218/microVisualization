from pymongo import MongoClient
import pymemcache.client.base

IP_ADDR = '192.168.99.100'

MONGO_MOVIE_INFO_PORT = 32022
MMC_MOVIE_INFO_PORT = 32023

MONGO_CAST_INFO_PORT = 32024
MMC_CAST_INFO_PORT = 32025

MONGO_PHOTO_PORT = 32026
MMC_PHOTO_PORT = 32027

MONGO_VIDEO_PORT = 32028
MMC_VIDEO_PORT = 32029

NUM_MOVIES = 5

mmc_movie_info_client = pymemcache.client.base.Client((IP_ADDR, MMC_MOVIE_INFO_PORT))
mmc_cast_info_client = pymemcache.client.base.Client((IP_ADDR, MMC_CAST_INFO_PORT))
mmc_photo_client = pymemcache.client.base.Client((IP_ADDR, MMC_PHOTO_PORT))
mmc_video_client = pymemcache.client.base.Client((IP_ADDR, MMC_VIDEO_PORT))

mongo_movie_info_db = MongoClient(IP_ADDR, MONGO_MOVIE_INFO_PORT).movie_info
mongo_cast_info_db = MongoClient(IP_ADDR, MONGO_CAST_INFO_PORT).cast_info
mongo_photo_db = MongoClient(IP_ADDR, MONGO_PHOTO_PORT).photo
mongo_video_db = MongoClient(IP_ADDR, MONGO_VIDEO_PORT).video

for i in range(NUM_MOVIES):
    mmc_movie_info_client.set('movie_' + str(i) + "_cast", "cast_" + str(i))
    mmc_movie_info_client.set('movie_' + str(i) + "_plot", "This is a plot for movie_" + str(i))
    mmc_movie_info_client.set('movie_' + str(i) + "_thumbnail", "This is a thumbnail for movie_" + str(i))
    mmc_movie_info_client.set('movie_' + str(i) + "_rating", "5")

    mmc_photo_client.set('movie_' + str(i), "This is a photo for movie_" + str(i))
    mmc_video_client.set('movie_' + str(i), "This is a video for movie_" + str(i))
    mmc_cast_info_client.set('cast_' + str(i), "This is the info for cast_" + str(i))

    mongo_movie_info_db.cast.insert_one({'movie_id' :'movie_' + str(i), 'cast': 'cast_' + str(i)})
    mongo_movie_info_db.plot.insert_one({'movie_id' :'movie_' + str(i), 'plot': "This is a plot for movie_" + str(i)})
    mongo_movie_info_db.thumbnail.insert_one({'movie_id' :'movie_' + str(i), 'thumbnail': "This is a thumbnail for movie_" + str(i)})
    mongo_movie_info_db.rating.insert_one({'movie_id' :'movie_' + str(i), 'rating': '5'})

    mongo_photo_db.photo.insert_one({'movie_id' :'movie_' + str(i), 'photo': "This is a photo for movie_" + str(i)})
    mongo_video_db.photo.insert_one({'movie_id' :'movie_' + str(i), 'video': "This is a video for movie_" + str(i)})

    mongo_cast_info_db.cast_info.insert_one({'cast_id': 'cast_' + str(i), 'info': "This is the info for cast_" + str(i)})





