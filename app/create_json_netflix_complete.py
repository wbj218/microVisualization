##!/usr/bin/env python
"""
Example of creating a radar chart (a.k.a. a spider or star chart) [1]_.

Although this example allows a frame of either 'circle' or 'polygon', polygon
frames don't have proper gridlines (the lines are circles instead of polygons).
It's possible to get a polygon grid by setting GRIDLINE_INTERPOLATION_STEPS in
matplotlib.axis to the desired number of vertices, but the orientation of the
polygon is not aligned with the radial axes.

.. [1] http://en.wikipedia.org/wiki/Radar_chart
"""
from __future__ import with_statement
import numpy as np
import sys
import Image


f = open('inputs/'+sys.argv[1], 'w')

f.write('{\n    "arch":"netflix_tracing",\n    "date":"2016-04-17T13:46:56.330297532-07:00",\n    "graph":[\n	    {"node":"netflix.tracing.zone0.client","package":"webserver","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"netflix.tracing.zone0.load_balancer","package":"logic","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n ')

for j in xrange(0,3):
    #f.write('{"node":"netflix.tracing.zone'+str(j)+'.ZipkinCollector","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
    for i in xrange(0,int(int(sys.argv[2])/int(10))):
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.nginx'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.php-fpm'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.ReviewText'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.AssignRating'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.MovieID'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.UniqueID'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.MoviePlot'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.MovieThumbnail'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.MovieRating'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.CastInfo'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.WatchNext'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.MovieReviews'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.Photos'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.Videos'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.Favorite'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.Ads'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.AdsUtilityMatrix'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.memcached_ads_recommendations'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.WatchList'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.MovieRecommendations'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.memcached_movies_recommendations'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.xapian'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.MovieUtilityMatrix'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.UserAccountInfo'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.FavoriteStorage'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.memcached_favorite'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.mongodb_favorite'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.VideosNFS'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.RentMovie'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.VideoPlayer'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.ComposeReview'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.Payment'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.AccountLogin'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.ComposePage'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.ReadMovieInfo'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.UpdateMovieInfo'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.MoviesNFS'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.ReviewStorage'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.ReviewsNFS'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.MovieReviewDB'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.UserReviewDB'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.memcached_castDB'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.mongodb_castDB'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.memcached_photosDB'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.mongodb_photosDB'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.memcached_videoPathDB'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.mongodb_videoPathDB'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        #f.write('{"node":"netflix.tracing.zone'+str(j)+'.Cassandra'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        #f.write('{"node":"netflix.tracing.zone'+str(j)+'.QueryEngine'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        #f.write('{"node":"netflix.tracing.zone'+str(j)+'.WebUI'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')

for j in xrange(0,3):
    for i in xrange(0,int(sys.argv[2])):
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.memcached_movieInfo'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.mongodb_movieInfo'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.memcached_movieDB'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.mongodb_movieDB'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.memcached_movieReview'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.mongodb_movieReview'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.memcached_userReview'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.mongodb_userReview'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')

"""
for j in xrange(0,3):
    for i in xrange(0,int(9.6*int(sys.argv[2]))):
        f.write('{"node":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.ZipkinCollector","target":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
"""
f.write('{"edge":"e1","source":"netflix.tracing.zone0.client","target":"netflix.tracing.zone0.load_balancer","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')

for j in xrange(0,3):
    for i in xrange(0,int(int(sys.argv[2])/int(10))):
        f.write('{"edge":"e2","source":"netflix.tracing.zone0.load_balancer","target":"netflix.tracing.zone'+str(j)+'.nginx'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"netflix.tracing.zone'+str(j)+'.nginx'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.php-fpm'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e4","source":"netflix.tracing.zone'+str(j)+'.php-fpm'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.ReviewText'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e5","source":"netflix.tracing.zone'+str(j)+'.php-fpm'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.AssignRating'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e6","source":"netflix.tracing.zone'+str(j)+'.php-fpm'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.UniqueID'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e7","source":"netflix.tracing.zone'+str(j)+'.php-fpm'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.MovieID'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e8","source":"netflix.tracing.zone'+str(j)+'.php-fpm'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.MoviePlot'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e9","source":"netflix.tracing.zone'+str(j)+'.php-fpm'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.MovieThumbnail'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e9","source":"netflix.tracing.zone'+str(j)+'.php-fpm'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.MovieRating'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e9","source":"netflix.tracing.zone'+str(j)+'.php-fpm'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.CastInfo'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e9","source":"netflix.tracing.zone'+str(j)+'.php-fpm'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.WatchNext'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e9","source":"netflix.tracing.zone'+str(j)+'.php-fpm'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.MovieReviews'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e9","source":"netflix.tracing.zone'+str(j)+'.php-fpm'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.Photos'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e9","source":"netflix.tracing.zone'+str(j)+'.php-fpm'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.Videos'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e9","source":"netflix.tracing.zone'+str(j)+'.php-fpm'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.xapian'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e9","source":"netflix.tracing.zone'+str(j)+'.php-fpm'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.MovieRecommendations'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')


        f.write('{"edge":"e9","source":"netflix.tracing.zone'+str(j)+'.php-fpm'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.Ads'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.ReviewText'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.ComposeReview'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.AssignRating'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.ComposeReview'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.UniqueID'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.ComposeReview'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.MovieID'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.ComposeReview'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.ComposeReview'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.AccountLogin'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.AccountLogin'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.UserAccountInfo'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')


        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.ComposeReview'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.UserReviewDB'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.ComposeReview'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.MovieReviewDB'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.ComposeReview'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.ReviewStorage'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.ReviewStorage'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.ReviewsNFS'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.ComposePage'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.ReadMovieInfo'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.ComposePage'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.RentMovie'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.RentMovie'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.Payment'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.RentMovie'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.VideoPlayer'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.VideoPlayer'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.MoviesNFS'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')


        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.MoviePlot'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.ComposePage'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.MovieThumbnail'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.ComposePage'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.MovieRating'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.ComposePage'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.CastInfo'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.ComposePage'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.WatchNext'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.ComposePage'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.MovieReviews'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.ComposePage'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.Photos'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.ComposePage'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.Videos'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.ComposePage'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.Videos'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.VideosNFS'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')

        f.write('{"edge":"e65","source":"netflix.tracing.zone'+str(j)+'.MovieRecommendations'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.MovieUtilityMatrix'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e65","source":"netflix.tracing.zone'+str(j)+'.MovieRecommendations'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.memcached_movies_recommendations'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')


        f.write('{"edge":"e65","source":"netflix.tracing.zone'+str(j)+'.Ads'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.UserAccountInfo'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e65","source":"netflix.tracing.zone'+str(j)+'.Ads'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.AdsUtilityMatrix'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e65","source":"netflix.tracing.zone'+str(j)+'.Ads'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.memcached_ads_recommendations'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e65","source":"netflix.tracing.zone'+str(j)+'.php-fpm'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.Favorite'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e65","source":"netflix.tracing.zone'+str(j)+'.Favorite'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.WatchList'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e65","source":"netflix.tracing.zone'+str(j)+'.Favorite'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.FavoriteStorage'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.FavoriteStorage'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.memcached_favorite'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.FavoriteStorage'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.mongodb_favorite'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e65","source":"netflix.tracing.zone'+str(j)+'.Photos'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.memcached_photosDB'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e65","source":"netflix.tracing.zone'+str(j)+'.Photos'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.mongodb_photosDB'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e65","source":"netflix.tracing.zone'+str(j)+'.Videos'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.memcached_videoPathDB'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e65","source":"netflix.tracing.zone'+str(j)+'.Videos'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.mongodb_videoPathDB'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.CastInfo'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.memcached_castDB'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.ComposeReview'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.UpdateMovieInfo'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.CastInfo'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.mongodb_castDB'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        #f.write('{"edge":"e40","source":"netflix.tracing.zone'+str(j)+'.ZipkinCollector","target":"netflix.tracing.zone'+str(j)+'.Cassandra'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        #f.write('{"edge":"e40","source":"netflix.tracing.zone'+str(j)+'.Cassandra'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.QueryEngine'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        #f.write('{"edge":"e40","source":"netflix.tracing.zone'+str(j)+'.QueryEngine'+str(i)+'","target":"netflix.tracing.zone'+str(j)+'.WebUI'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')

for j in xrange(0,3):
    for i in xrange(0,int(sys.argv[2])):
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.UpdateMovieInfo'+str(int(i/int(10)))+'","target":"netflix.tracing.zone'+str(j)+'.memcached_movieInfo'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.UpdateMovieInfo'+str(int(i/int(10)))+'","target":"netflix.tracing.zone'+str(j)+'.mongodb_movieInfo'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.UserReviewDB'+str(int(i/int(10)))+'","target":"netflix.tracing.zone'+str(j)+'.memcached_userReview'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.UserReviewDB'+str(int(i/int(10)))+'","target":"netflix.tracing.zone'+str(j)+'.mongodb_userReview'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.VideoPlayer'+str(int(i/int(10)))+'","target":"netflix.tracing.zone'+str(j)+'.memcached_movieDB'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.VideoPlayer'+str(int(i/int(10)))+'","target":"netflix.tracing.zone'+str(j)+'.mongodb_movieDB'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.MovieReviewDB'+str(int(i/int(10)))+'","target":"netflix.tracing.zone'+str(j)+'.memcached_movieReview'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.MovieReviewDB'+str(int(i/int(10)))+'","target":"netflix.tracing.zone'+str(j)+'.mongodb_movieReview'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.ReadMovieInfo'+str(int(i/int(10)))+'","target":"netflix.tracing.zone'+str(j)+'.memcached_movieInfo'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"netflix.tracing.zone'+str(j)+'.ReadMovieInfo'+str(int(i/int(10)))+'","target":"netflix.tracing.zone'+str(j)+'.mongodb_movieInfo'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')


"""
k=0
for j in xrange(0,3):
    k=0
    for i in xrange(0,int(sys.argv[2])):
        f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(k)+'","target":"netflix.tracing.zone'+str(j)+'.memcached_user'+str(j)+'_'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(k+1)+'","target":"netflix.tracing.zone'+str(j)+'.mongodb_user'+str(j)+'_'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(k+2*int(sys.argv[2]))+'","target":"netflix.tracing.zone'+str(j)+'.memcached_follower'+str(j)+'_'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(k+1+2*int(sys.argv[2]))+'","target":"netflix.tracing.zone'+str(j)+'.mongodb_follower'+str(j)+'_'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(k+4*int(sys.argv[2]))+'","target":"netflix.tracing.zone'+str(j)+'.memcached_DMRecipient'+str(j)+'_'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(k+1+4*int(sys.argv[2]))+'","target":"netflix.tracing.zone'+str(j)+'.mongodb_DMRecipient'+str(j)+'_'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        k = k + 2
"""	    

"""
#k = 600
for j in xrange(0,3):
    for i in xrange(0,int(int(sys.argv[2])/int(10))):
            f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(6*int(sys.argv[2])+i)+'","target":"netflix.tracing.zone'+str(j)+'.nginx'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
	    f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(int(6.10*int(sys.argv[2]))+i)+'","target":"netflix.tracing.zone'+str(j)+'.php-fpm'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
	    f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(int(6.20*int(sys.argv[2]))+i)+'","target":"netflix.tracing.zone'+str(j)+'.memcached_follow_recommendations'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
	    f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(int(6.30*int(sys.argv[2]))+i)+'","target":"netflix.tracing.zone'+str(j)+'.memcached_ads_recommendations'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
	    f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(int(6.40*int(sys.argv[2]))+i)+'","target":"netflix.tracing.zone'+str(j)+'.FollowRecommender'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
	    f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(int(6.50*int(sys.argv[2]))+i)+'","target":"netflix.tracing.zone'+str(j)+'.text'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
	    f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(int(6.60*int(sys.argv[2]))+i)+'","target":"netflix.tracing.zone'+str(j)+'.video'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
	    f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(int(6.70*int(sys.argv[2]))+i)+'","target":"netflix.tracing.zone'+str(j)+'.image'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
	    f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(int(6.80*int(sys.argv[2]))+i)+'","target":"netflix.tracing.zone'+str(j)+'.userTag'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
	    f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(int(6.90*int(sys.argv[2]))+i)+'","target":"netflix.tracing.zone'+str(j)+'.urlShorten'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
	    f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(int(7.00*int(sys.argv[2]))+i)+'","target":"netflix.tracing.zone'+str(j)+'.uniqueID'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
	    f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(int(7.10*int(sys.argv[2]))+i)+'","target":"netflix.tracing.zone'+str(j)+'.Ads'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
	    f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(int(7.20*int(sys.argv[2]))+i)+'","target":"netflix.tracing.zone'+str(j)+'.ComposeTweet'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
	    f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(int(7.30*int(sys.argv[2]))+i)+'","target":"netflix.tracing.zone'+str(j)+'.ReadTweet'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
	    f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(int(7.40*int(sys.argv[2]))+i)+'","target":"netflix.tracing.zone'+str(j)+'.Reply'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
	    f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(int(7.50*int(sys.argv[2]))+i)+'","target":"netflix.tracing.zone'+str(j)+'.Favorite'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
	    f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(int(7.60*int(sys.argv[2]))+i)+'","target":"netflix.tracing.zone'+str(j)+'.DM'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
	    f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(int(7.70*int(sys.argv[2]))+i)+'","target":"netflix.tracing.zone'+str(j)+'.RT'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
	    f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(int(7.80*int(sys.argv[2]))+i)+'","target":"netflix.tracing.zone'+str(j)+'.ChangeFollow'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
	    f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(int(7.90*int(sys.argv[2]))+i)+'","target":"netflix.tracing.zone'+str(j)+'.UserStats'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
	    f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(int(8.00*int(sys.argv[2]))+i)+'","target":"netflix.tracing.zone'+str(j)+'.UserGraph'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
	    f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(int(8.10*int(sys.argv[2]))+i)+'","target":"netflix.tracing.zone'+str(j)+'.BlockUser'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
	    f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(int(8.20*int(sys.argv[2]))+i)+'","target":"netflix.tracing.zone'+str(j)+'.BlockedStats'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
	    f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(int(8.30*int(sys.argv[2]))+i)+'","target":"netflix.tracing.zone'+str(j)+'.UserStats'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
	    f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(int(8.40*int(sys.argv[2]))+i)+'","target":"netflix.tracing.zone'+str(j)+'.xapian'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
	    f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(int(8.50*int(sys.argv[2]))+i)+'","target":"netflix.tracing.zone'+str(j)+'.ReadTimeline'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
	    f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(int(8.60*int(sys.argv[2]))+i)+'","target":"netflix.tracing.zone'+str(j)+'.WriteTimeline'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
	    f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(int(8.70*int(sys.argv[2]))+i)+'","target":"netflix.tracing.zone'+str(j)+'.WriteGraphTimelines'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
	    f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(int(8.80*int(sys.argv[2]))+i)+'","target":"netflix.tracing.zone'+str(j)+'.WriteReplyTimeline'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
	    f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(int(8.90*int(sys.argv[2]))+i)+'","target":"netflix.tracing.zone'+str(j)+'.WriteDMRecipient'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
	    f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(int(9.00*int(sys.argv[2]))+i)+'","target":"netflix.tracing.zone'+str(j)+'.TweetStorage'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
	    f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(int(9.10*int(sys.argv[2]))+i)+'","target":"netflix.tracing.zone'+str(j)+'.DMStorage'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
	    f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(int(9.20*int(sys.argv[2]))+i)+'","target":"netflix.tracing.zone'+str(j)+'.memcached_tweetStorage'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
	    f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(int(9.30*int(sys.argv[2]))+i)+'","target":"netflix.tracing.zone'+str(j)+'.mongodb_tweetStorage'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
	    f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(int(9.40*int(sys.argv[2]))+i)+'","target":"netflix.tracing.zone'+str(j)+'.memcached_DMStorage'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
	    f.write('{"edge":"e1","source":"netflix.tracing.zone'+str(j)+'.zipkinClient_'+str(int(9.50*int(sys.argv[2]))+i)+'","target":"netflix.tracing.zone'+str(j)+'.mongodb_DMStorage'+str(i)+'","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
"""

f.write(']\n}\n')

f.close() 
	    

	    
