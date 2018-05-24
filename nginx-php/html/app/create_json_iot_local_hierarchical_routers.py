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
f.write('{\n"arch":"iot_local",\n"date":"2016-04-17T13:46:56.330297532-07:00",\n')
f.write('"graph":[\n	    {"node":"iot.local.zone0.client","package":"webserver","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    ')
f.write('{"node":"iot.local.zone0.load_balancer","package":"logic","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	  ')
for i in xrange(0,3):
    f.write('{"node":"iot.local.zone'+str(i)+'.nginx","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
    f.write('{"node":"iot.local.zone'+str(i)+'.cloudController","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	    ')
    f.write('{"node":"iot.local.zone'+str(i)+'.constructRoute","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
    f.write('{"node":"iot.local.zone'+str(i)+'.targetDB","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	')
    f.write('{"node":"iot.local.zone'+str(i)+'.imageDB","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	')
    f.write('{"node":"iot.local.zone'+str(i)+'.videoDB","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	')
    f.write('{"node":"iot.local.zone'+str(i)+'.locationDB","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	 ')
    f.write('{"node":"iot.local.zone'+str(i)+'.orientationDB","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')	
    f.write('{"node":"iot.local.zone'+str(i)+'.speedDB","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	')
    f.write('{"node":"iot.local.zone'+str(i)+'.luminosityDB","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')	    
    f.write('{"node":"iot.local.zone'+str(i)+'.motionController","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	 ')
    f.write('{"node":"iot.local.zone'+str(i)+'.imageRecognition","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	 ')
    f.write('{"node":"iot.local.zone'+str(i)+'.obstacleAvoidance","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	 ')
    f.write('{"node":"iot.local.zone'+str(i)+'.stockImageDB","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')

for j in xrange(0,3):
    for i in xrange(0,int(sys.argv[2])/10):
        f.write('{"node":"iot.local.zone'+str(j)+'.wifiRouter'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"iot.local.zone'+str(j)+'.masterDrone'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"iot.local.zone'+str(j)+'.constructRoute'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"iot.local.zone'+str(j)+'.targetDB'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	')
        f.write('{"node":"iot.local.zone'+str(j)+'.imageDB'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	')
        f.write('{"node":"iot.local.zone'+str(j)+'.videoDB'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	')
        f.write('{"node":"iot.local.zone'+str(j)+'.locationDB'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	 ')
        f.write('{"node":"iot.local.zone'+str(j)+'.orientationDB'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')	
        f.write('{"node":"iot.local.zone'+str(j)+'.speedDB'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	')
        f.write('{"node":"iot.local.zone'+str(j)+'.luminosityDB'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')	    
        f.write('{"node":"iot.local.zone'+str(j)+'.masterMotionController'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	 ')
        f.write('{"node":"iot.local.zone'+str(j)+'.masterImageRecognition'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	 ')
        f.write('{"node":"iot.local.zone'+str(j)+'.masterObstacleAvoidance'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n	 ')
        f.write('{"node":"iot.local.zone'+str(j)+'.stockImageDB'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')

    
for j in xrange(0,3):
    for i in xrange(0,int(sys.argv[2])):
        f.write('{"node":"iot.local.zone'+str(j)+'.droneController'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"iot.local.zone'+str(j)+'.sensorCameraImage'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"iot.local.zone'+str(j)+'.sensorCameraVideo'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"iot.local.zone'+str(j)+'.sensorLocation'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"iot.local.zone'+str(j)+'.sensorSpeed'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"iot.local.zone'+str(j)+'.sensorOrientation'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"iot.local.zone'+str(j)+'.sensorLuminosity'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"iot.local.zone'+str(j)+'.logJS'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')
        f.write('{"node":"iot.local.zone'+str(j)+'.logDB'+str(i)+'","package":"db","timestamp":"2016-04-17T13:46:56.329529972-07:00","metadata":"IP/54.198.0.1"},\n')

f.write('{"edge":"e1","source":"iot.local.zone0.client","target":"iot.local.zone0.load_balancer","bidirectional":"no","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
for i in xrange(0,3):
    f.write('{"edge":"e2","source":"iot.local.zone0.load_balancer","target":"iot.local.zone'+str(i)+'.nginx","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.zone'+str(i)+'.nginx","target":"iot.local.zone'+str(i)+'.cloudController","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.zone'+str(i)+'.cloudController","target":"iot.local.zone'+str(i)+'.constructRoute","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.zone'+str(i)+'.constructRoute","target":"iot.local.zone'+str(i)+'.targetDB","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.zone'+str(i)+'.cloudController","target":"iot.local.zone'+str(i)+'.imageDB","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.zone'+str(i)+'.cloudController","target":"iot.local.zone'+str(i)+'.videoDB","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.zone'+str(i)+'.cloudController","target":"iot.local.zone'+str(i)+'.locationDB","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.zone'+str(i)+'.cloudController","target":"iot.local.zone'+str(i)+'.speedDB","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.zone'+str(i)+'.cloudController","target":"iot.local.zone'+str(i)+'.orientationDB","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.zone'+str(i)+'.cloudController","target":"iot.local.zone'+str(i)+'.luminosityDB","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.zone'+str(i)+'.motionController","target":"iot.local.zone'+str(i)+'.imageRecognition","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.zone'+str(i)+'.cloudController","target":"iot.local.zone'+str(i)+'.motionController","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.zone'+str(i)+'.imageRecognition","target":"iot.local.zone'+str(i)+'.obstacleAvoidance","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.zone'+str(i)+'.imageRecognition","target":"iot.local.zone'+str(i)+'.stockImageDB","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.zone'+str(i)+'.obstacleAvoidance","target":"iot.local.zone'+str(i)+'.constructRoute","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.zone'+str(i)+'.imageDB","target":"iot.local.zone'+str(i)+'.imageRecognition","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.zone'+str(i)+'.videoDB","target":"iot.local.zone'+str(i)+'.imageRecognition","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.zone'+str(i)+'.locationDB","target":"iot.local.zone'+str(i)+'.imageRecognition","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.zone'+str(i)+'.imageDB","target":"iot.local.zone'+str(i)+'.obstacleAvoidance","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.zone'+str(i)+'.videoDB","target":"iot.local.zone'+str(i)+'.obstacleAvoidance","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.zone'+str(i)+'.locationDB","target":"iot.local.zone'+str(i)+'.obstacleAvoidance","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.zone'+str(i)+'.speedDB","target":"iot.local.zone'+str(i)+'.obstacleAvoidance","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.zone'+str(i)+'.orientationDB","target":"iot.local.zone'+str(i)+'.obstacleAvoidance","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
    f.write('{"edge":"e3","source":"iot.local.zone'+str(i)+'.luminosityDB","target":"iot.local.zone'+str(i)+'.obstacleAvoidance","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')

for j in xrange(0,3):
    for i in xrange(0,int(sys.argv[2])/10):
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.cloudController","target":"iot.local.zone'+str(j)+'.wifiRouter'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.wifiRouter'+str(i)+'","target":"iot.local.zone'+str(j)+'.masterDrone'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.masterDrone'+str(i)+'","target":"iot.local.zone'+str(j)+'.constructRoute'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.constructRoute'+str(i)+'","target":"iot.local.zone'+str(j)+'.targetDB'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.masterDrone'+str(i)+'","target":"iot.local.zone'+str(j)+'.imageDB'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.masterDrone'+str(i)+'","target":"iot.local.zone'+str(j)+'.videoDB'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.masterDrone'+str(i)+'","target":"iot.local.zone'+str(j)+'.locationDB'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.masterDrone'+str(i)+'","target":"iot.local.zone'+str(j)+'.speedDB'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.masterDrone'+str(i)+'","target":"iot.local.zone'+str(j)+'.orientationDB'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.masterDrone'+str(i)+'","target":"iot.local.zone'+str(j)+'.luminosityDB'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.masterMotionController'+str(i)+'","target":"iot.local.zone'+str(j)+'.masterImageRecognition'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.masterDrone'+str(i)+'","target":"iot.local.zone'+str(j)+'.masterMotionController'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.masterImageRecognition'+str(i)+'","target":"iot.local.zone'+str(j)+'.masterObstacleAvoidance'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.masterImageRecognition'+str(i)+'","target":"iot.local.zone'+str(j)+'.stockImageDB'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.masterObstacleAvoidance'+str(i)+'","target":"iot.local.zone'+str(j)+'.constructRoute'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.imageDB'+str(i)+'","target":"iot.local.zone'+str(j)+'.masterImageRecognition'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.videoDB'+str(i)+'","target":"iot.local.zone'+str(j)+'.masterImageRecognition'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.locationDB'+str(i)+'","target":"iot.local.zone'+str(j)+'.masterImageRecognition'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.imageDB'+str(i)+'","target":"iot.local.zone'+str(j)+'.masterObstacleAvoidance'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.videoDB'+str(i)+'","target":"iot.local.zone'+str(j)+'.masterObstacleAvoidance'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.locationDB'+str(i)+'","target":"iot.local.zone'+str(j)+'.masterObstacleAvoidance'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.speedDB'+str(i)+'","target":"iot.local.zone'+str(j)+'.masterObstacleAvoidance'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.orientationDB'+str(i)+'","target":"iot.local.zone'+str(j)+'.masterObstacleAvoidance'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.luminosityDB'+str(i)+'","target":"iot.local.zone'+str(j)+'.masterObstacleAvoidance'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')

for j in xrange(0,3):
    for i in xrange(0,int(sys.argv[2])):
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.masterDrone'+str(int(int(i)/10))+'","target":"iot.local.zone'+str(j)+'.droneController'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.droneController'+str(i)+'","target":"iot.local.zone'+str(j)+'.logJS'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.droneController'+str(i)+'","target":"iot.local.zone'+str(j)+'.sensorCameraImage'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.droneController'+str(i)+'","target":"iot.local.zone'+str(j)+'.sensorCameraVideo'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.droneController'+str(i)+'","target":"iot.local.zone'+str(j)+'.sensorLocation'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.droneController'+str(i)+'","target":"iot.local.zone'+str(j)+'.sensorSpeed'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.droneController'+str(i)+'","target":"iot.local.zone'+str(j)+'.sensorOrientation'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.droneController'+str(i)+'","target":"iot.local.zone'+str(j)+'.sensorLuminosity'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.sensorCameraImage'+str(i)+'","target":"iot.local.zone'+str(j)+'.logJS'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.sensorCameraVideo'+str(i)+'","target":"iot.local.zone'+str(j)+'.logJS'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.sensorLocation'+str(i)+'","target":"iot.local.zone'+str(j)+'.logJS'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.sensorSpeed'+str(i)+'","target":"iot.local.zone'+str(j)+'.logJS'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.sensorOrientation'+str(i)+'","target":"iot.local.zone'+str(j)+'.logJS'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.sensorLuminosity'+str(i)+'","target":"iot.local.zone'+str(j)+'.logJS'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')
        f.write('{"edge":"e3","source":"iot.local.zone'+str(j)+'.logJS'+str(i)+'","target":"iot.local.zone'+str(j)+'.logDB'+str(i)+'","bidirectional":"yes","timestamp":"2016-04-17T13:46:56.330250819-07:00"},\n')

f.write(']\n}\n')

f.close() 
