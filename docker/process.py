
with open("memcached.log", "r") as f:
	lines = f.readlines()
	for line in lines:
		words = line.split(", ")
		if "opcode: 7\n" not in words:
			print line

