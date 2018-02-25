#!/usr/bin/env python
import operator
import sys

#input format: python parse_mpstat.py [input_file] [nginx_core] [mmc_core] [mongod_core] 

#open the docker mpstats file
print str(sys.argv[1])

#get the cores to monitor
nginx_core = int(sys.argv[2])
mmc_core = int(sys.argv[3])
mongod_core = int(sys.argv[4])

with open('../cpu/' + sys.argv[1], "r") as input:
    with open("parsed_" + str(sys.argv[1]), "wb") as output:
	
	#store CPU utilizations in arrays
	nginx = []
	mmc = []
	mongod = []
	
	line_num = 0
	for line in input:
	    line_num += 1
	    if ("all" not in line) & ("Average" not in line) & ("CPU" not in line) & (line_num % 43 != 2):
		line_core = line[14:16]
		if line_core != '':
		    line_core = int(line_core)
                    total_cpu = 100 - float(line[90:95])
		    if (nginx_core == line_core):
			nginx.append(total_cpu)
		    if (mmc_core == line_core):
			mmc.append(total_cpu)
		    if (mongod_core == line_core):
			mongod.append(total_cpu)

	curr = 0
	end = len(nginx) - 1	
	while curr != end:
	    if (nginx[curr] < 6.0):
		nginx.pop(curr)
		mmc.pop(curr)
		mongod.pop(curr)
		end -= 1
	    else:
	        curr += 1

	output.write("Total CPU utilization for nginx core #" + str(nginx_core) + ":" + '\n')
	for i in range(len(nginx)):
       	    output.write(str(nginx[i]) + '\n')

	output.write("Total CPU utilization for mmc core #" + str(mmc_core) + ":" + '\n')
	for i in range(len(mmc)):
	    output.write(str(mmc[i]) + '\n')

        output.write("Total CPU utilization for mongod core #" + str(mongod_core) + ":" + '\n')	
	for i in range(len(mongod)):
	    output.write(str(mongod[i]) + '\n')	
