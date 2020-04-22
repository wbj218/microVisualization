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

f.write('{\n    "arch":"twitter_compose",\n    "date":"2016-04-17T13:46:56.330297532-07:00",\n    "graph":[\n	    {"node":"twitter.compose.zone0.client","package":"webserver","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"twitter.compose.zone0.load_balancer","package":"logic","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')

for j in xrange(0,3):
    for i in xrange(0,int(int(sys.argv[2])/int(10))):
        f.write('{"node":"twitter.compose.zone'+str(j)+'.nginx'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"twitter.compose.zone'+str(j)+'.php-fpm'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"twitter.compose.zone'+str(j)+'.text'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"twitter.compose.zone'+str(j)+'.image'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"twitter.compose.zone'+str(j)+'.video'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"twitter.compose.zone'+str(j)+'.userTag'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"twitter.compose.zone'+str(j)+'.urlShorten'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"twitter.compose.zone'+str(j)+'.uniqueID'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"twitter.compose.zone'+str(j)+'.ComposeTweet'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"twitter.compose.zone'+str(j)+'.TweetStorage'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"twitter.compose.zone'+str(j)+'.memcached_tweetStorage'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"twitter.compose.zone'+str(j)+'.mongodb_tweetStorage'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"twitter.compose.zone'+str(j)+'.WriteTimeline'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"twitter.compose.zone'+str(j)+'.UserGraph'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"twitter.compose.zone'+str(j)+'.WriteGraphTimelines'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')

for j in xrange(0,3):
    for i in xrange(0,int(sys.argv[2])):
        f.write('{"node":"twitter.compose.zone'+str(j)+'.memcached_user'+str(j)+'_'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"twitter.compose.zone'+str(j)+'.mongodb_user'+str(j)+'_'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')    
        f.write('{"node":"twitter.compose.zone'+str(j)+'.memcached_follower'+str(j)+'_'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"twitter.compose.zone'+str(j)+'.mongodb_follower'+str(j)+'_'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')

f.write('{"edge":"e1","source":"twitter.compose.zone0.client","target":"twitter.compose.zone0.load_balancer","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')

for j in xrange(0,3):
    for i in xrange(0,int(int(sys.argv[2])/int(10))):
        f.write('{"edge":"e2","source":"twitter.compose.zone0.load_balancer","target":"twitter.compose.zone'+str(j)+'.nginx'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"twitter.compose.zone'+str(j)+'.nginx'+str(i)+'","target":"twitter.compose.zone'+str(j)+'.php-fpm'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e4","source":"twitter.compose.zone'+str(j)+'.php-fpm'+str(i)+'","target":"twitter.compose.zone'+str(j)+'.text'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e5","source":"twitter.compose.zone'+str(j)+'.php-fpm'+str(i)+'","target":"twitter.compose.zone'+str(j)+'.image'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e6","source":"twitter.compose.zone'+str(j)+'.php-fpm'+str(i)+'","target":"twitter.compose.zone'+str(j)+'.video'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e7","source":"twitter.compose.zone'+str(j)+'.php-fpm'+str(i)+'","target":"twitter.compose.zone'+str(j)+'.userTag'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e8","source":"twitter.compose.zone'+str(j)+'.php-fpm'+str(i)+'","target":"twitter.compose.zone'+str(j)+'.urlShorten'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e9","source":"twitter.compose.zone'+str(j)+'.php-fpm'+str(i)+'","target":"twitter.compose.zone'+str(j)+'.uniqueID'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e10","source":"twitter.compose.zone'+str(j)+'.text'+str(i)+'","target":"twitter.compose.zone'+str(j)+'.ComposeTweet'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e11","source":"twitter.compose.zone'+str(j)+'.image'+str(i)+'","target":"twitter.compose.zone'+str(j)+'.ComposeTweet'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e12","source":"twitter.compose.zone'+str(j)+'.video'+str(i)+'","target":"twitter.compose.zone'+str(j)+'.ComposeTweet'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e13","source":"twitter.compose.zone'+str(j)+'.userTag'+str(i)+'","target":"twitter.compose.zone'+str(j)+'.ComposeTweet'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e14","source":"twitter.compose.zone'+str(j)+'.urlShorten'+str(i)+'","target":"twitter.compose.zone'+str(j)+'.ComposeTweet'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e15","source":"twitter.compose.zone'+str(j)+'.uniqueID'+str(i)+'","target":"twitter.compose.zone'+str(j)+'.ComposeTweet'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e16","source":"twitter.compose.zone'+str(j)+'.ComposeTweet'+str(i)+'","target":"twitter.compose.zone'+str(j)+'.TweetStorage'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e17","source":"twitter.compose.zone'+str(j)+'.ComposeTweet'+str(i)+'","target":"twitter.compose.zone'+str(j)+'.WriteTimeline'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e18","source":"twitter.compose.zone'+str(j)+'.ComposeTweet'+str(i)+'","target":"twitter.compose.zone'+str(j)+'.WriteGraphTimelines'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e19","source":"twitter.compose.zone'+str(j)+'.TweetStorage'+str(i)+'","target":"twitter.compose.zone'+str(j)+'.memcached_tweetStorage'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e20","source":"twitter.compose.zone'+str(j)+'.TweetStorage'+str(i)+'","target":"twitter.compose.zone'+str(j)+'.mongodb_tweetStorage'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e65","source":"twitter.compose.zone'+str(j)+'.WriteGraphTimelines'+str(i)+'","target":"twitter.compose.zone'+str(j)+'.UserGraph'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    
    
for j in xrange(0,3):
    for i in xrange(0,int(sys.argv[2])):
        f.write('{"edge":"e21","source":"twitter.compose.zone'+str(j)+'.WriteTimeline'+str((i/int(10)))+'","target":"twitter.compose.zone'+str(j)+'.memcached_user'+str(j)+'_'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e22","source":"twitter.compose.zone'+str(j)+'.WriteTimeline'+str((i/int(10)))+'","target":"twitter.compose.zone'+str(j)+'.mongodb_user'+str(j)+'_'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e43","source":"twitter.compose.zone'+str(j)+'.WriteGraphTimelines'+str((i/int(10)))+'","target":"twitter.compose.zone'+str(j)+'.memcached_follower'+str(j)+'_'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e44","source":"twitter.compose.zone'+str(j)+'.WriteGraphTimelines'+str((i/int(10)))+'","target":"twitter.compose.zone'+str(j)+'.mongodb_follower'+str(j)+'_'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    
f.write(']\n}\n')

f.close() 
	    

