killall -INT AssignRatingServer
killall -INT ProcessMovieIDServer
killall -INT ProcessUniqueIDServer
killall -INT ProcessTextServer
killall -INT ComposeReviewServer
killall -INT ReviewStorageServer
killall -INT MovieReviewDBServer
killall -INT UserReviewDBServer

sleep 1

killall -KILL AssignRatingServer
killall -KILL ProcessMovieIDServer
killall -KILL ProcessUniqueIDServer
killall -KILL ProcessTextServer
killall -KILL ComposeReviewServer
killall -KILL ReviewStorageServer
killall -KILL MovieReviewDBServer
killall -KILL UserReviewDBServer

