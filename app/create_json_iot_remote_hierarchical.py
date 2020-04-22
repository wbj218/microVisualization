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
f.write('{\n    "arch":"iot_remote",\n    "date":"2016-04-17T13:46:56.330297532-07:00",\n    "graph":[\n	    {"node":"iot.remote.client","package":"webserver","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.remote.load_balancer","package":"logic","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.remote.nginx","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.remote.cloudController","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.remote.constructRoute","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.remote.targetDB","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.remote.imageDB","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.remote.videoDB","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.remote.locationDB","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.remote.orientationDB","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.remote.speedDB","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.remote.luminosityDB","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.remote.wifiRouter","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')

for i in xrange(0,(int(sys.argv[2])/25)):
    f.write('	{"node":"iot.remote.masterDrone'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
    
    
i=0
for i in xrange(0,int(sys.argv[2])):
    f.write('	{"node":"iot.remote.droneController'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
    f.write('	{"node":"iot.remote.motionController'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
    f.write('	{"node":"iot.remote.imageRecognition'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
    f.write('	{"node":"iot.remote.obstacleAvoidance'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
    f.write('	{"node":"iot.remote.sensorCameraImage'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
    f.write('	{"node":"iot.remote.sensorCameraVideo'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
    f.write('	{"node":"iot.remote.sensorLocation'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
    f.write('	{"node":"iot.remote.sensorSpeed'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
    f.write('	{"node":"iot.remote.sensorOrientation'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
    f.write('	{"node":"iot.remote.sensorLuminosity'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
    f.write('	{"node":"iot.remote.logJS'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
    f.write('	{"node":"iot.remote.logDB'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
    f.write('	{"node":"iot.remote.stockImageDB'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')

f.write('	{"edge":"e1","source":"iot.remote.client","target":"iot.remote.load_balancer","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('	{"edge":"e2","source":"iot.remote.load_balancer","target":"iot.remote.nginx","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('	{"edge":"e3","source":"iot.remote.nginx","target":"iot.remote.cloudController","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('	{"edge":"e3","source":"iot.remote.cloudController","target":"iot.remote.constructRoute","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('	{"edge":"e3","source":"iot.remote.constructRoute","target":"iot.remote.targetDB","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('	{"edge":"e3","source":"iot.remote.cloudController","target":"iot.remote.wifiRouter","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('	{"edge":"e3","source":"iot.remote.cloudController","target":"iot.remote.imageDB","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('	{"edge":"e3","source":"iot.remote.cloudController","target":"iot.remote.videoDB","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('	{"edge":"e3","source":"iot.remote.cloudController","target":"iot.remote.locationDB","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('	{"edge":"e3","source":"iot.remote.cloudController","target":"iot.remote.speedDB","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('	{"edge":"e3","source":"iot.remote.cloudController","target":"iot.remote.orientationDB","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('	{"edge":"e3","source":"iot.remote.cloudController","target":"iot.remote.luminosityDB","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')

i=0
for i in xrange(0,(int(sys.argv[2])/25)):
    f.write('	{"edge":"e3","source":"iot.remote.wifiRouter","target":"iot.remote.masterDrone'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')

i=0
for j in xrange(0,int(sys.argv[2])):
    f.write('	{"edge":"e3","source":"iot.remote.masterDrone'+str(int(int(j)/25))+'","target":"iot.remote.droneController'+str(j)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('	{"edge":"e3","source":"iot.remote.droneController'+str(j)+'","target":"iot.remote.logJS'+str(j)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('	{"edge":"e3","source":"iot.remote.droneController'+str(j)+'","target":"iot.remote.motionController'+str(j)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('	{"edge":"e3","source":"iot.remote.droneController'+str(j)+'","target":"iot.remote.sensorCameraImage'+str(j)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('	{"edge":"e3","source":"iot.remote.droneController'+str(j)+'","target":"iot.remote.sensorCameraVideo'+str(j)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('	{"edge":"e3","source":"iot.remote.droneController'+str(j)+'","target":"iot.remote.sensorLocation'+str(j)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('	{"edge":"e3","source":"iot.remote.droneController'+str(j)+'","target":"iot.remote.sensorSpeed'+str(j)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('	{"edge":"e3","source":"iot.remote.droneController'+str(j)+'","target":"iot.remote.sensorOrientation'+str(j)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('	{"edge":"e3","source":"iot.remote.droneController'+str(j)+'","target":"iot.remote.sensorLuminosity'+str(j)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('	{"edge":"e3","source":"iot.remote.sensorCameraImage'+str(j)+'","target":"iot.remote.logJS'+str(j)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('	{"edge":"e3","source":"iot.remote.sensorCameraVideo'+str(j)+'","target":"iot.remote.logJS'+str(j)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('	{"edge":"e3","source":"iot.remote.sensorLocation'+str(j)+'","target":"iot.remote.logJS'+str(j)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('	{"edge":"e3","source":"iot.remote.sensorSpeed'+str(j)+'","target":"iot.remote.logJS'+str(j)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('	{"edge":"e3","source":"iot.remote.sensorOrientation'+str(j)+'","target":"iot.remote.logJS'+str(j)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('	{"edge":"e3","source":"iot.remote.sensorLuminosity'+str(j)+'","target":"iot.remote.logJS'+str(j)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('	{"edge":"e3","source":"iot.remote.motionController'+str(j)+'","target":"iot.remote.imageRecognition'+str(j)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('	{"edge":"e3","source":"iot.remote.imageRecognition'+str(j)+'","target":"iot.remote.obstacleAvoidance'+str(j)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('	{"edge":"e3","source":"iot.remote.imageRecognition'+str(j)+'","target":"iot.remote.stockImageDB'+str(j)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('	{"edge":"e3","source":"iot.remote.sensorCameraImage'+str(j)+'","target":"iot.remote.imageRecognition'+str(j)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('	{"edge":"e3","source":"iot.remote.sensorCameraVideo'+str(j)+'","target":"iot.remote.imageRecognition'+str(j)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('	{"edge":"e3","source":"iot.remote.sensorLocation'+str(j)+'","target":"iot.remote.imageRecognition'+str(j)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('	{"edge":"e3","source":"iot.remote.sensorCameraImage'+str(j)+'","target":"iot.remote.obstacleAvoidance'+str(j)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('	{"edge":"e3","source":"iot.remote.sensorCameraVideo'+str(j)+'","target":"iot.remote.obstacleAvoidance'+str(j)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('	{"edge":"e3","source":"iot.remote.sensorLocation'+str(j)+'","target":"iot.remote.obstacleAvoidance'+str(j)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('	{"edge":"e3","source":"iot.remote.sensorSpeed'+str(j)+'","target":"iot.remote.obstacleAvoidance'+str(j)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('	{"edge":"e3","source":"iot.remote.sensorOrientation'+str(j)+'","target":"iot.remote.obstacleAvoidance'+str(j)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('	{"edge":"e3","source":"iot.remote.sensorLuminosity'+str(j)+'","target":"iot.remote.obstacleAvoidance'+str(j)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('	{"edge":"e3","source":"iot.remote.logJS'+str(j)+'","target":"iot.remote.logDB'+str(j)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('	]\n}\n')
f.close() 

