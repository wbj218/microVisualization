sudo killall -9 AssignRatingServer
sudo killall -9 ProcessMovieIDServer
sudo killall -9 ProcessUniqueIDServer
sudo killall -9 ProcessTextServer
sudo killall -9 ComposeReviewServer
sudo killall -9 ReviewStorageServer
sudo killall -9 MovieReviewDBServer
sudo killall -9 UserReviewDBServer

sleep 1

sudo killall -u sh2442 python3
