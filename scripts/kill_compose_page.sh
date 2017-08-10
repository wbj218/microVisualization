killall -INT GetPlotServer
killall -INT GetThumbnailServer
killall -INT GetWatchNextServer
killall -INT GetPhotoServer
killall -INT GetVideoServer
killall -INT GetRatingServer
killall -INT GetMovieReviewServer
killall -INT GetCastInfoServer
killall -INT ProcessMovieIDServer
killall -INT MovieInfoStorageServer
killall -INT MovieReviewDBServer
killall -INT ReviewStorageServer
killall -INT ComposePageServer

sleep 1

killall -KILL GetPlotServer
killall -KILL GetThumbnailServer
killall -KILL GetWatchNextServer
killall -KILL GetPhotoServer
killall -KILL GetVideoServer
killall -KILL GetRatingServer
killall -KILL GetMovieReviewServer
killall -KILL GetCastInfoServer
killall -KILL ProcessMovieIDServer
killall -KILL MovieInfoStorageServer
killall -KILL MovieReviewDBServer
killall -KILL ReviewStorageServer
killall -KILL ComposePageServer

python3 LogComposePage.py
