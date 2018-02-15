##!/usr/bin/env python
"""

You can use decreasing axes by flipping the normal order of the axis
limits

"""
#!/usr/bin/python
from __future__ import with_statement
import matplotlib as mpl
mpl.use('Agg')
import numpy as np
import matplotlib.pyplot as plt
import sys
from pylab import *
import time
import mmap
import random
import os
from collections import defaultdict

def bufcount(filename):
    f = open(filename)                  
    lines = 0
    buf_size = 1024 * 1024
    read_f = f.read # loop optimization

    buf = read_f(buf_size)
    while buf:
        lines += buf.count('\n')
        buf = read_f(buf_size)
    return lines

fig=plt.figure()

#load = np.array([2,5,10,50,100,200,300,400,500,600,700,800,900,1000,1500,2000,2500,3000,3500,4000,4500,5000,5500,6000,6500,7000,7500,8000,8500,9000,9500,10000])
load = np.linspace(0.02,0.98,49)
#load = np.array([0.02,0.04,0.06,0.08,0.1,0.12,0.14,0.16,0.18,0.2,0.22,0.24,0.26,0.28,0.3,0.32,0.34,0.36,0.38,0.4,0.42,0.44,0.46,0.48,0.5,0.52,0.54,0.56,0.580,0.60])

tailLatency = np.zeros(len(load))
medianLatency = np.zeros(len(load))
avgLatency = np.zeros(len(load))

dirName = sys.argv[1]
appName = dirName.split('/',5)
appDate = appName[3]
print appName[4]

for j in xrange(len(load)):
    #latencyMergedUnsorted = np.loadtxt('../logs/2016_11_29_basic_' + str(load[j]) + '.out', delimiter='\n')
    #latencyMergedUnsorted = np.loadtxt('/filer-01/qsim/logs/2016_11_29_netflix_' + str(load[j]) + '.out', delimiter='\n')
    #latencyMergedUnsorted = np.loadtxt('/filer-01/qsim/logs/2016_11_29_19_44_cassandra_' + str(load[j]) + '.out', delimiter='\n')
    #latencyMergedUnsorted = np.loadtxt('/filer-01/qsim/logs/2016_12_02_19_04_basic_' + str(load[j]) + '.out', delimiter='\n')
    confLoad = int(100*load[j])
    if (int(100*load[j])== 57):
        confLoad = 58
    if (int(100*load[j])== 5):
        load[j] = 0.06
    if (int(100*load[j])== 13):
        load[j] = 0.14
        confLoad = 14
    latencyMergedUnsorted = np.loadtxt(sys.argv[1] + '_' + str(confLoad) + '_100.out', delimiter='\n')
    #print 'unsorted[' + str(load[j]) + ']: ', latencyMergedUnsorted
    #print len(latencyMergedUnsorted)
    latencySorted = np.sort(latencyMergedUnsorted)
    for i in xrange(len(latencySorted)):
        avgLatency[j] += latencySorted[i]
    avgLatency[j] = float(avgLatency[j]/float(len(latencySorted)))
    print 'load', load[j], 'avg latency', avgLatency[j]
    #print 'sorted: ', latencySorted
    #print len(latencySorted)
    tailIndex = np.rint(0.99*len(latencySorted)) - 1
    print 'load', load[j], 'tailIndex: ', tailIndex, 'tail latency', latencySorted[tailIndex]
    #print 'tail latency', latencySorted[tailIndex]
    tailLatency[j] = latencySorted[tailIndex]
    medianIndex = np.rint(0.5*len(latencySorted)) - 1
    medianLatency[j] = latencySorted[medianIndex]
    print 'load', load[j], 'medianIndex: ', medianIndex, 'median latency', latencySorted[medianIndex]

params = {#'backend': 'ps',
          'axes.labelsize': 30,
          'text.fontsize': 20,
#          'legend.fontsize': 14,
          'legend.fontsize': 26, #28
          'xtick.labelsize': 22,
          'ytick.labelsize': 24}#,
#          'text.usetex': True,
#          'figure.figsize': fig_size}

#matplotlib.rcParams.update({'font.size': 18})

plt.rcParams.update(params)
plt.rcParams['ps.useafm'] = True
plt.rcParams['pdf.use14corefonts'] = True
plt.rcParams['text.usetex'] = True

plot(load, tailLatency, '-', color='#990000', linewidth=3.5)
plot(load, avgLatency, '-', color='#339933', linewidth=3.5)
plot(load, medianLatency, '--', color='#330000', linewidth=3.5)
#plt.semilogy(load, tailLatency, '-', color='#990000', linewidth=3.5)

#indx = np.linspace(20,20,2)
#indy = np.linspace(0,2000,2)
#plot(indx,indy,'--', color='#990000', linewidth=1.5)

plt.legend(('tail', 'avg', 'median'), ncol=1, loc='upper left')

ax=plt.axis()

#xlim(0, 1200)
#ylim(0, 1.06)
#ylim(0, 3)
plt.xlabel('Load')
plt.ylabel('99th pctl Latency (usec)')
fig.set_figheight(5) #2 for label
fig.set_figwidth(9) #4 for label
fig.subplots_adjust(left=0.16)
#fig.subplots_adjust(bottom=0.12)
fig.subplots_adjust(bottom=0.148)
#fig.subplots_adjust(right=0.92)
grid(True, color='#999999')

plt.subplot(111)
#plt.legend(('Bolt', 'Naive'), loc='upper left', ncol=1)

fig.savefig('scripts/figures/'+appName[4]+'.pdf')
plt.show()

