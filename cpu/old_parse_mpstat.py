#!/usr/bin/env python
import operator
import sys

#input format: python parse_mpstat.py [input_file] [nginx_core] [mmc_core] [mongod_core] 

#open the docker mpstats file
print str(sys.argv[1])
with open('../cpu/' + sys.argv[1], "r") as input:
    with open("parsed_" + str(sys.argv[1]), "wb") as output:
        for i in range(3):
	    input.seek(0)
	    print(str(i))
	    core = int(sys.argv[2 + i])
	    output.write("Total CPU utilization for core #" + str(core) + ":" + '\n')
	    line_num = 0
	    for line in input:
	        line_num += 1
		if ("all" not in line) & ("CPU" not in line) & (line_num % 43 != 2):
		#if ("all" not in line) & (line_num % 43 != 2):
		    print("in if and i is " + str(i))
		    line_core = line[14:16]
#		    print(line_core)
		    if line_core != '':
			line_core = int(line_core)
#			print("Line_core is " + str(line_core) + "and core is " + str(core))
                        if (core == line_core) & ("Average" not in line):
                            total_cpu = 100 - float(line[90:95])
		            output.write(str(total_cpu) + '\n')
	    i += 1
