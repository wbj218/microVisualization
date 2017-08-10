#!/bin/zsh

taskset -c 0 ../bin/GetPlotServer 1 1 &
taskset -c 2 ../bin/GetThumbnailServer 1 1 &
taskset -c 4 ../bin/GetWatchNextServer 1 &
taskset -c 6 ../bin/GetPhotoServer 1 &
taskset -c 8 ../bin/GetVideoServer 1 &
taskset -c 10 ../bin/GetRatingServer 1 1 &
taskset -c 12 ../bin/GetMovieReviewServer 1 1 1 &
taskset -c 14 ../bin/GetCastInfoServer 1 1 &

taskset -c 16 ../bin/ProcessMovieIDServer &

taskset -c 18 ../bin/MovieInfoStorageServer 1 &
taskset -c 1 ../bin/MovieReviewDBServer 1 &
taskset -c 3 ../bin/ReviewStorageServer 1 &

taskset -c 5,7 ../bin/ComposePageServer 1 &

python3 ../generator/GeneratorServer.py 0 &
python3 ../generator/GeneratorServer.py 1 &
python3 ../generator/GeneratorServer.py 2 &
python3 ../generator/GeneratorServer.py 3 &

sudo renice -n -20 -u yg397 s