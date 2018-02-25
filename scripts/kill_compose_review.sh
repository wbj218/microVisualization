killall -KILL AssignRatingServer
killall -KILL ProcessMovieIDServer
killall -KILL ProcessUniqueIDServer
killall -KILL ProcessTextServer
killall -KILL ComposeReviewServer
killall -KILL ReviewStorageServer
killall -KILL MovieReviewDBServer
killall -KILL UserReviewDBServer
python3 LogComposeReview.py
killall python3
