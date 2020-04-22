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
f.write('{\n"arch":"iot_local",\n"date":"2016-04-17T13:46:56.330297532-07:00",\n    "graph":[\n	    {"node":"iot.local.client","package":"webserver","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.local.load_balancer","package":"logic","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.local.nginx","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.local.cloudController","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.local.constructRoute","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.local.targetDB","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.local.imageDB","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.local.videoDB","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.local.locationDB","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.local.orientationDB","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.local.speedDB","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.local.luminosityDB","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.local.motionController","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.local.imageRecognition","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.local.obstacleAvoidance","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.local.stockImageDB","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.local.wifiRouter","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')

for i in xrange(0,int(sys.argv[2])):
    f.write('{"node":"iot.local.droneController'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
    f.write('{"node":"iot.local.sensorCameraImage'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
    f.write('{"node":"iot.local.sensorCameraVideo'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
    f.write('{"node":"iot.local.sensorLocation'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
    f.write('{"node":"iot.local.sensorSpeed'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
    f.write('{"node":"iot.local.sensorOrientation'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
    f.write('{"node":"iot.local.sensorLuminosity'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
    f.write('{"node":"iot.local.logJS'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
    f.write('{"node":"iot.local.logDB'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')

f.write('{"edge":"e1","source":"iot.local.client","target":"iot.local.load_balancer","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('{"edge":"e2","source":"iot.local.load_balancer","target":"iot.local.nginx","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('{"edge":"e3","source":"iot.local.nginx","target":"iot.local.cloudController","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('{"edge":"e3","source":"iot.local.cloudController","target":"iot.local.constructRoute","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('{"edge":"e3","source":"iot.local.constructRoute","target":"iot.local.targetDB","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('{"edge":"e3","source":"iot.local.cloudController","target":"iot.local.wifiRouter","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('{"edge":"e3","source":"iot.local.cloudController","target":"iot.local.imageDB","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('{"edge":"e3","source":"iot.local.cloudController","target":"iot.local.videoDB","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('{"edge":"e3","source":"iot.local.cloudController","target":"iot.local.locationDB","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('{"edge":"e3","source":"iot.local.cloudController","target":"iot.local.speedDB","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('{"edge":"e3","source":"iot.local.cloudController","target":"iot.local.orientationDB","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('{"edge":"e3","source":"iot.local.cloudController","target":"iot.local.luminosityDB","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('{"edge":"e3","source":"iot.local.motionController","target":"iot.local.imageRecognition","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('{"edge":"e3","source":"iot.local.cloudController","target":"iot.local.motionController","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('{"edge":"e3","source":"iot.local.imageRecognition","target":"iot.local.obstacleAvoidance","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('{"edge":"e3","source":"iot.local.imageRecognition","target":"iot.local.stockImageDB","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('{"edge":"e3","source":"iot.local.obstacleAvoidance","target":"iot.local.constructRoute","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('{"edge":"e3","source":"iot.local.imageDB","target":"iot.local.imageRecognition","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('{"edge":"e3","source":"iot.local.videoDB","target":"iot.local.imageRecognition","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('{"edge":"e3","source":"iot.local.locationDB","target":"iot.local.imageRecognition","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('{"edge":"e3","source":"iot.local.imageDB","target":"iot.local.obstacleAvoidance","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('{"edge":"e3","source":"iot.local.videoDB","target":"iot.local.obstacleAvoidance","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('{"edge":"e3","source":"iot.local.locationDB","target":"iot.local.obstacleAvoidance","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('{"edge":"e3","source":"iot.local.speedDB","target":"iot.local.obstacleAvoidance","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('{"edge":"e3","source":"iot.local.orientationDB","target":"iot.local.obstacleAvoidance","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('{"edge":"e3","source":"iot.local.luminosityDB","target":"iot.local.obstacleAvoidance","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')

for i in xrange(0,int(sys.argv[2])):
    f.write('{"edge":"e3","source":"iot.local.wifiRouter","target":"iot.local.droneController'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.droneController'+str(i)+'","target":"iot.local.logJS'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.droneController'+str(i)+'","target":"iot.local.sensorCameraImage'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.droneController'+str(i)+'","target":"iot.local.sensorCameraVideo'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.droneController'+str(i)+'","target":"iot.local.sensorLocation'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.droneController'+str(i)+'","target":"iot.local.sensorSpeed'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.droneController'+str(i)+'","target":"iot.local.sensorOrientation'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.droneController'+str(i)+'","target":"iot.local.sensorLuminosity'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.sensorCameraImage'+str(i)+'","target":"iot.local.logJS'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.sensorCameraVideo'+str(i)+'","target":"iot.local.logJS'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.sensorLocation'+str(i)+'","target":"iot.local.logJS'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.sensorSpeed'+str(i)+'","target":"iot.local.logJS'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.sensorOrientation'+str(i)+'","target":"iot.local.logJS'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.sensorLuminosity'+str(i)+'","target":"iot.local.logJS'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.logJS'+str(i)+'","target":"iot.local.logDB'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')

f.write(']\n}\n')

f.close() 
