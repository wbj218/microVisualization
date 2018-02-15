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
f.write('{\n    "arch":"iot_remote",\n    "date":"2016-04-17T13:46:56.330297532-07:00",\n    "graph":[\n	    {"node":"iot.remote.zone0.client","package":"webserver","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.remote.zone0.load_balancer","package":"logic","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.remote.zone0.nginx","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.remote.zone0.cloudController","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.remote.zone0.constructRoute","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.remote.zone0.targetDB","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.remote.zone0.imageDB","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.remote.zone0.videoDB","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.remote.zone0.locationDB","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.remote.zone0.orientationDB","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.remote.zone0.speedDB","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    {"node":"iot.remote.zone0.luminosityDB","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')


for j in xrange(0,3):
    for i in xrange(0,(int(sys.argv[2])/10)):
        f.write('{"node":"iot.remote.zone'+str(j)+'.wifiRouter'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('	{"node":"iot.remote.zone'+str(j)+'.masterDrone'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('	{"node":"iot.remote.zone'+str(j)+'.masterMotionController'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('	{"node":"iot.remote.zone'+str(j)+'.masterImageRecognition'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('	{"node":"iot.remote.zone'+str(j)+'.masterObstacleAvoidance'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('	{"node":"iot.remote.zone'+str(j)+'.logDB'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('	{"node":"iot.remote.zone'+str(j)+'.stockImageDB'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
    
    
i=0
for j in xrange(0,3):
    for i in xrange(0,int(sys.argv[2])):
        f.write('	{"node":"iot.remote.zone'+str(j)+'.droneController'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('	{"node":"iot.remote.zone'+str(j)+'.sensorCameraImage'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('	{"node":"iot.remote.zone'+str(j)+'.sensorCameraVideo'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('	{"node":"iot.remote.zone'+str(j)+'.sensorLocation'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('	{"node":"iot.remote.zone'+str(j)+'.sensorSpeed'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('	{"node":"iot.remote.zone'+str(j)+'.sensorOrientation'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('	{"node":"iot.remote.zone'+str(j)+'.sensorLuminosity'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('	{"node":"iot.remote.zone'+str(j)+'.logJS'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')

f.write('	{"edge":"e1","source":"iot.remote.zone0.client","target":"iot.remote.zone0.load_balancer","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('	{"edge":"e2","source":"iot.remote.zone0.load_balancer","target":"iot.remote.zone0.nginx","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('	{"edge":"e3","source":"iot.remote.zone0.nginx","target":"iot.remote.zone0.cloudController","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('	{"edge":"e3","source":"iot.remote.zone0.cloudController","target":"iot.remote.zone0.constructRoute","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('	{"edge":"e3","source":"iot.remote.zone0.constructRoute","target":"iot.remote.zone0.targetDB","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('	{"edge":"e3","source":"iot.remote.zone0.cloudController","target":"iot.remote.zone0.imageDB","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('	{"edge":"e3","source":"iot.remote.zone0.cloudController","target":"iot.remote.zone0.videoDB","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('	{"edge":"e3","source":"iot.remote.zone0.cloudController","target":"iot.remote.zone0.locationDB","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('	{"edge":"e3","source":"iot.remote.zone0.cloudController","target":"iot.remote.zone0.speedDB","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('	{"edge":"e3","source":"iot.remote.zone0.cloudController","target":"iot.remote.zone0.orientationDB","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('	{"edge":"e3","source":"iot.remote.zone0.cloudController","target":"iot.remote.zone0.luminosityDB","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')

i=0
for j in xrange(0,3):
    for i in xrange(0,(int(sys.argv[2])/10)):
        f.write('	{"edge":"e3","source":"iot.remote.zone0.cloudController","target":"iot.remote.zone'+str(j)+'.wifiRouter'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('	{"edge":"e3","source":"iot.remote.zone'+str(j)+'.wifiRouter'+str(i)+'","target":"iot.remote.zone'+str(j)+'.masterDrone'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('	{"edge":"e3","source":"iot.remote.zone'+str(j)+'.masterDrone'+str(i)+'","target":"iot.remote.zone'+str(j)+'.masterMotionController'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('	{"edge":"e3","source":"iot.remote.zone'+str(j)+'.masterDrone'+str(i)+'","target":"iot.remote.zone'+str(j)+'.masterImageRecognition'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('	{"edge":"e3","source":"iot.remote.zone'+str(j)+'.masterDrone'+str(i)+'","target":"iot.remote.zone'+str(j)+'.masterObstacleAvoidance'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('	{"edge":"e3","source":"iot.remote.zone'+str(j)+'.masterMotionController'+str(i)+'","target":"iot.remote.zone'+str(j)+'.masterImageRecognition'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('	{"edge":"e3","source":"iot.remote.zone'+str(j)+'.masterImageRecognition'+str(i)+'","target":"iot.remote.zone'+str(j)+'.masterObstacleAvoidance'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('	{"edge":"e3","source":"iot.remote.zone'+str(j)+'.masterImageRecognition'+str(i)+'","target":"iot.remote.zone'+str(j)+'.stockImageDB'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')

i=0
for j in xrange(0,3):
    for i in xrange(0,int(sys.argv[2])):
        f.write('	{"edge":"e3","source":"iot.remote.zone'+str(j)+'.masterDrone'+str(int(int(i)/10))+'","target":"iot.remote.zone'+str(j)+'.droneController'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('	{"edge":"e3","source":"iot.remote.zone'+str(j)+'.droneController'+str(i)+'","target":"iot.remote.zone'+str(j)+'.logJS'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('	{"edge":"e3","source":"iot.remote.zone'+str(j)+'.droneController'+str(i)+'","target":"iot.remote.zone'+str(j)+'.sensorCameraImage'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('	{"edge":"e3","source":"iot.remote.zone'+str(j)+'.droneController'+str(i)+'","target":"iot.remote.zone'+str(j)+'.sensorCameraVideo'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('	{"edge":"e3","source":"iot.remote.zone'+str(j)+'.droneController'+str(i)+'","target":"iot.remote.zone'+str(j)+'.sensorLocation'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('	{"edge":"e3","source":"iot.remote.zone'+str(j)+'.droneController'+str(i)+'","target":"iot.remote.zone'+str(j)+'.sensorSpeed'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('	{"edge":"e3","source":"iot.remote.zone'+str(j)+'.droneController'+str(i)+'","target":"iot.remote.zone'+str(j)+'.sensorOrientation'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('	{"edge":"e3","source":"iot.remote.zone'+str(j)+'.droneController'+str(i)+'","target":"iot.remote.zone'+str(j)+'.sensorLuminosity'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('	{"edge":"e3","source":"iot.remote.zone'+str(j)+'.sensorCameraImage'+str(i)+'","target":"iot.remote.zone'+str(j)+'.logJS'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('	{"edge":"e3","source":"iot.remote.zone'+str(j)+'.sensorCameraVideo'+str(i)+'","target":"iot.remote.zone'+str(j)+'.logJS'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('	{"edge":"e3","source":"iot.remote.zone'+str(j)+'.sensorLocation'+str(i)+'","target":"iot.remote.zone'+str(j)+'.logJS'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('	{"edge":"e3","source":"iot.remote.zone'+str(j)+'.sensorSpeed'+str(i)+'","target":"iot.remote.zone'+str(j)+'.logJS'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('	{"edge":"e3","source":"iot.remote.zone'+str(j)+'.sensorOrientation'+str(i)+'","target":"iot.remote.zone'+str(j)+'.logJS'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('	{"edge":"e3","source":"iot.remote.zone'+str(j)+'.sensorLuminosity'+str(i)+'","target":"iot.remote.zone'+str(j)+'.logJS'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('	{"edge":"e3","source":"iot.remote.zone'+str(j)+'.sensorCameraImage'+str(i)+'","target":"iot.remote.zone'+str(j)+'.masterImageRecognition'+str(int(int(i)/10))+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('	{"edge":"e3","source":"iot.remote.zone'+str(j)+'.sensorCameraVideo'+str(i)+'","target":"iot.remote.zone'+str(j)+'.masterImageRecognition'+str(int(int(i)/10))+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('	{"edge":"e3","source":"iot.remote.zone'+str(j)+'.sensorLocation'+str(i)+'","target":"iot.remote.zone'+str(j)+'.masterImageRecognition'+str(int(int(i)/10))+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('	{"edge":"e3","source":"iot.remote.zone'+str(j)+'.sensorCameraImage'+str(i)+'","target":"iot.remote.zone'+str(j)+'.masterObstacleAvoidance'+str(int(int(i)/10))+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('	{"edge":"e3","source":"iot.remote.zone'+str(j)+'.sensorCameraVideo'+str(i)+'","target":"iot.remote.zone'+str(j)+'.masterObstacleAvoidance'+str(int(int(i)/10))+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('	{"edge":"e3","source":"iot.remote.zone'+str(j)+'.sensorLocation'+str(i)+'","target":"iot.remote.zone'+str(j)+'.masterObstacleAvoidance'+str(int(int(i)/10))+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('	{"edge":"e3","source":"iot.remote.zone'+str(j)+'.sensorSpeed'+str(i)+'","target":"iot.remote.zone'+str(j)+'.masterObstacleAvoidance'+str(int(int(i)/10))+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('	{"edge":"e3","source":"iot.remote.zone'+str(j)+'.sensorOrientation'+str(i)+'","target":"iot.remote.zone'+str(j)+'.masterObstacleAvoidance'+str(int(int(i)/10))+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('	{"edge":"e3","source":"iot.remote.zone'+str(j)+'.sensorLuminosity'+str(i)+'","target":"iot.remote.zone'+str(j)+'.masterObstacleAvoidance'+str(int(int(i)/10))+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('	{"edge":"e3","source":"iot.remote.zone'+str(j)+'.logJS'+str(i)+'","target":"iot.remote.zone'+str(j)+'.logDB'+str(int(int(i)/10))+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
f.write('	]\n}\n')
f.close() 

