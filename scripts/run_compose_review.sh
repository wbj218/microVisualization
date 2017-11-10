#!/bin/bash

taskset -c 0 ../bin/AssignRatingServer &
taskset -c 2 ../bin/ProcessMovieIDServer &
taskset -c 4 ../bin/ProcessUniqueIDServer &
taskset -c 6 ../bin/ProcessTextServer &

taskset -c 8 ../bin/ComposeReviewServer 0 0 0 &

taskset -c 10 ../bin/ReviewStorageServer 0 &
taskset -c 12 ../bin/MovieReviewDBServer 0 &
taskset -c 14 ../bin/UserReviewDBServer 0 &

python3 ../generator/GeneratorServer.py 0 &
python3 ../generator/GeneratorServer.py 1 &
python3 ../generator/GeneratorServer.py 2 &
python3 ../generator/GeneratorServer.py 3 &


# sudo renice -n -20 -u yg397 




