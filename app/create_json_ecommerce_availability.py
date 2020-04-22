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
f.write('{\n"arch":"e-commerce",\n"date":"2016-04-17T13:46:56.330297532-07:00",\n    "graph":[\n	    {"node":"e-commerce.complete.zone0.client","package":"webserver","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"e-commerce.complete.zone0.load_balancer","package":"logic","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')

for j in xrange(0,3):
    for i in xrange(0,int(sys.argv[2])):
        f.write('{"node":"e-commerce.complete.zone'+str(j)+'.frontend'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"e-commerce.complete.zone'+str(j)+'.orders'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"e-commerce.complete.zone'+str(j)+'.ComposeLandingPage'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"e-commerce.complete.zone'+str(j)+'.search'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"e-commerce.complete.zone'+str(j)+'.discounts'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"e-commerce.complete.zone'+str(j)+'.media'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"e-commerce.complete.zone'+str(j)+'.catalogue'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"e-commerce.complete.zone'+str(j)+'.memcached_inventory'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"e-commerce.complete.zone'+str(j)+'.mongodb_inventory'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"e-commerce.complete.zone'+str(j)+'.login'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"e-commerce.complete.zone'+str(j)+'.sessionID'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"e-commerce.complete.zone'+str(j)+'.AccountInfo'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"e-commerce.complete.zone'+str(j)+'.AccountInfoDB'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"e-commerce.complete.zone'+str(j)+'.Shipping'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"e-commerce.complete.zone'+str(j)+'.mongodb_delivery'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"e-commerce.complete.zone'+str(j)+'.OrderQueue'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"e-commerce.complete.zone'+str(j)+'.QueueMaster'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"e-commerce.complete.zone'+str(j)+'.memcached_orders'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"e-commerce.complete.zone'+str(j)+'.mongodb_orders'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"e-commerce.complete.zone'+str(j)+'.payment'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"e-commerce.complete.zone'+str(j)+'.transactionID'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"e-commerce.complete.zone'+str(j)+'.Authorization'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"e-commerce.complete.zone'+str(j)+'.Invoicing'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"e-commerce.complete.zone'+str(j)+'.recommendations'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"e-commerce.complete.zone'+str(j)+'.utilityMatrix'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"e-commerce.complete.zone'+str(j)+'.memcached_recommendations'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"e-commerce.complete.zone'+str(j)+'.wishlist'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"e-commerce.complete.zone'+str(j)+'.cart'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"e-commerce.complete.zone'+str(j)+'.mongodb_cart'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')


f.write('{"edge":"e1","source":"e-commerce.complete.zone0.client","target":"e-commerce.complete.zone0.load_balancer","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')


for j in xrange(0,3):
    for i in xrange(0,int(sys.argv[2])):
        f.write('{"edge":"e2","source":"e-commerce.complete.zone0.load_balancer","target":"e-commerce.complete.zone'+str(j)+'.frontend'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e2","source":"e-commerce.complete.zone'+str(j)+'.frontend'+str(i)+'","target":"e-commerce.complete.zone'+str(j)+'.ComposeLandingPage'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e2","source":"e-commerce.complete.zone'+str(j)+'.frontend'+str(i)+'","target":"e-commerce.complete.zone'+str(j)+'.recommendations'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e2","source":"e-commerce.complete.zone'+str(j)+'.frontend'+str(i)+'","target":"e-commerce.complete.zone'+str(j)+'.search'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e2","source":"e-commerce.complete.zone'+str(j)+'.frontend'+str(i)+'","target":"e-commerce.complete.zone'+str(j)+'.login'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e2","source":"e-commerce.complete.zone'+str(j)+'.frontend'+str(i)+'","target":"e-commerce.complete.zone'+str(j)+'.orders'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e2","source":"e-commerce.complete.zone'+str(j)+'.frontend'+str(i)+'","target":"e-commerce.complete.zone'+str(j)+'.cart'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e2","source":"e-commerce.complete.zone'+str(j)+'.recommendations'+str(i)+'","target":"e-commerce.complete.zone'+str(j)+'.utilityMatrix'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e2","source":"e-commerce.complete.zone'+str(j)+'.recommendations'+str(i)+'","target":"e-commerce.complete.zone'+str(j)+'.memcached_recommendations'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e2","source":"e-commerce.complete.zone'+str(j)+'.ComposeLandingPage'+str(i)+'","target":"e-commerce.complete.zone'+str(j)+'.search'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e2","source":"e-commerce.complete.zone'+str(j)+'.ComposeLandingPage'+str(i)+'","target":"e-commerce.complete.zone'+str(j)+'.discounts'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e2","source":"e-commerce.complete.zone'+str(j)+'.ComposeLandingPage'+str(i)+'","target":"e-commerce.complete.zone'+str(j)+'.media'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e2","source":"e-commerce.complete.zone'+str(j)+'.ComposeLandingPage'+str(i)+'","target":"e-commerce.complete.zone'+str(j)+'.catalogue'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e2","source":"e-commerce.complete.zone'+str(j)+'.catalogue'+str(i)+'","target":"e-commerce.complete.zone'+str(j)+'.memcached_inventory'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e2","source":"e-commerce.complete.zone'+str(j)+'.catalogue'+str(i)+'","target":"e-commerce.complete.zone'+str(j)+'.mongodb_inventory'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e2","source":"e-commerce.complete.zone'+str(j)+'.catalogue'+str(i)+'","target":"e-commerce.complete.zone'+str(j)+'.search'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e2","source":"e-commerce.complete.zone'+str(j)+'.login'+str(i)+'","target":"e-commerce.complete.zone'+str(j)+'.sessionID'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e2","source":"e-commerce.complete.zone'+str(j)+'.login'+str(i)+'","target":"e-commerce.complete.zone'+str(j)+'.AccountInfo'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e2","source":"e-commerce.complete.zone'+str(j)+'.AccountInfo'+str(i)+'","target":"e-commerce.complete.zone'+str(j)+'.AccountInfoDB'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e2","source":"e-commerce.complete.zone'+str(j)+'.orders'+str(i)+'","target":"e-commerce.complete.zone'+str(j)+'.Shipping'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e2","source":"e-commerce.complete.zone'+str(j)+'.orders'+str(i)+'","target":"e-commerce.complete.zone'+str(j)+'.QueueMaster'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e2","source":"e-commerce.complete.zone'+str(j)+'.orders'+str(i)+'","target":"e-commerce.complete.zone'+str(j)+'.memcached_orders'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e2","source":"e-commerce.complete.zone'+str(j)+'.orders'+str(i)+'","target":"e-commerce.complete.zone'+str(j)+'.mongodb_orders'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e2","source":"e-commerce.complete.zone'+str(j)+'.Shipping'+str(i)+'","target":"e-commerce.complete.zone'+str(j)+'.OrderQueue'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e2","source":"e-commerce.complete.zone'+str(j)+'.Shipping'+str(i)+'","target":"e-commerce.complete.zone'+str(j)+'.mongodb_delivery'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e2","source":"e-commerce.complete.zone'+str(j)+'.QueueMaster'+str(i)+'","target":"e-commerce.complete.zone'+str(j)+'.OrderQueue'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e2","source":"e-commerce.complete.zone'+str(j)+'.orders'+str(i)+'","target":"e-commerce.complete.zone'+str(j)+'.payment'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e2","source":"e-commerce.complete.zone'+str(j)+'.orders'+str(i)+'","target":"e-commerce.complete.zone'+str(j)+'.cart'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e2","source":"e-commerce.complete.zone'+str(j)+'.payment'+str(i)+'","target":"e-commerce.complete.zone'+str(j)+'.Invoicing'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e2","source":"e-commerce.complete.zone'+str(j)+'.payment'+str(i)+'","target":"e-commerce.complete.zone'+str(j)+'.transactionID'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e2","source":"e-commerce.complete.zone'+str(j)+'.payment'+str(i)+'","target":"e-commerce.complete.zone'+str(j)+'.Authorization'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e2","source":"e-commerce.complete.zone'+str(j)+'.cart'+str(i)+'","target":"e-commerce.complete.zone'+str(j)+'.wishlist'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e2","source":"e-commerce.complete.zone'+str(j)+'.cart'+str(i)+'","target":"e-commerce.complete.zone'+str(j)+'.mongodb_cart'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        
f.write(']\n}\n')

f.close() 
