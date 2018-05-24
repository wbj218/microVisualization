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
N = 5

ind = (0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287, 288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299, 300, 301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 315, 316, 317, 318, 319, 320, 321, 322, 323, 324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335, 336, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347, 348, 349, 350)  # the x locations for the groups
#width = 0.05       # the width of the bars

latencyMergedUnsorted = np.loadtxt('../stripped/2016_11_29_netflix_10000.out', delimiter='\n')

print len(latencyMergedUnsorted)

latencyWWWUnsorted = np.zeros(10000)
j = 0

for i in xrange(len(latencyMergedUnsorted)):
    if (i%2 == 0):
        latencyWWWUnsorted[i/2] = latencyMergedUnsorted[i]
        j = j + 1

print len(latencyWWWUnsorted)

ind = np.linspace(0,10000,10000)

latencySorted = np.sort(latencyWWWUnsorted)

print len(latencySorted)

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

plot(ind, latencySorted, '-', color='#660000', linewidth=3.5)

#indx = np.linspace(20,20,2)
#indy = np.linspace(0,2000,2)
#plot(indx,indy,'--', color='#990000', linewidth=1.5)

ax=plt.axis()

#xlim(0, 1200)
#ylim(0, 1.06)
plt.xlabel('Requests')
plt.ylabel('Latency')
fig.set_figheight(5) #2 for label
fig.set_figwidth(9) #4 for label
fig.subplots_adjust(left=0.16)
#fig.subplots_adjust(bottom=0.12)
fig.subplots_adjust(bottom=0.148)
#fig.subplots_adjust(right=0.92)
grid(True, color='#999999')

plt.subplot(111)
#plt.legend(('Bolt', 'Naive'), loc='upper left', ncol=1)

fig.savefig(sys.argv[1])
plt.show()

