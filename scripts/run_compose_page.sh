#!/bin/zsh

taskset -c 0 ../bin/GetPlotServer 1 2 &
taskset -c 2 ../bin/GetThumbnailServer 1 2 &
taskset -c 4 ../bin/GetWatchNextServer 2 &
taskset -c 6 ../bin/GetPhotoServer 2 &
taskset -c 8 ../bin/GetVideoServer 2 &
taskset -c 10 ../bin/GetRatingServer 1 2 &
taskset -c 12 ../bin/GetMovieReviewServer 1 1 2 &
taskset -c 14 ../bin/GetCastInfoServer 1 2 &

taskset -c 16 ../bin/ProcessMovieIDServer &

taskset -c 17 ../bin/MovieInfoStorageServer 1 &
# taskset -c 17,19 ../bin/MovieInfoStorageServer 1 &
taskset -c 1 ../bin/MovieReviewDBServer 1 &
taskset -c 3 ../bin/ReviewStorageServer 1 &

taskset -c 9 ../bin/ComposePageServer 0 &
taskset -c 11 ../bin/ComposePageServer 1 &

python3 ../generator/GeneratorServer.py 0 &
python3 ../generator/GeneratorServer.py 1 &
python3 ../generator/GeneratorServer.py 2 &
python3 ../generator/GeneratorServer.py 3 &
python3 ../generator/GeneratorServer.py 4 &
python3 ../generator/GeneratorServer.py 5 &
python3 ../generator/GeneratorServer.py 6 &
python3 ../generator/GeneratorServer.py 7 &

sudo renice -n -20 -u yg397 s