#!/bin/zsh

taskset -c 0 ../bin/AssignRatingServer &
taskset -c 2 ../bin/ProcessMovieIDServer &
taskset -c 4 ../bin/ProcessUniqueIDServer &
taskset -c 6 ../bin/ProcessTextServer &

taskset -c 8 ../bin/ComposeReviewServer 1 1 1&

taskset -c 10 ../bin/ReviewStorageServer 1 &
taskset -c 12 ../bin/MovieReviewDBServer 1 &
taskset -c 14 ../bin/UserReviewDBServer 1 &







