class DummyLogger:
	
        def logInitEnqueue(self, jid, time):
		pass

        def logChild(self, child, childName):
		pass

        def logForwardPathTime(self, finalTime, enqueueTime, deltaTime):
		pass

        def logPath(self, jid, path):
		pass

        def logBackwardPathTime(self, finalTime, enqueueTime, backwardLink, deltaTime):
		pass

        def logTrace(self, jid, trace, trace_length):
		pass

        def logTracePath(self, jid, trace_path, trace_path_length):
		pass
	
        def logArrival(self, index, time):
		pass

	def logFinishedFirstProcess(self, index, time):
		pass

	def logSendCache(self, parent, child, time):
		pass

	def logCacheHit(self, index, result):
		pass

	def logSendChild(self, parent, child, time):
		pass

	def logFinishedSecondProcess(self, index, time):
		pass

	def logSendParent(self, child, parent, time):
		pass

	def logSQLHit(self, index, result):
		pass

	def logDelayed(self, index, time1, time2):
		pass

class CmdLineQLogger:
	def __init__(self, index_name_dict):
		self.index_to_name = index_name_dict

        def logInitEnqueue(self, jid, time):
            print "[log]: initial enqueue time for job: ", jid, "is:    ", time

        def logChild(self, child, childName):
            print "[log] child is: ", child, "and the name of source of child is:", childName

        def logForwardPathTime(self, finalTime, enqueueTime, deltaTime):
            print "[log] [forward_path]: final time:    ", finalTime, "enqueue time:    ", enqueueTime, "response time:     ", deltaTime

        def logPath(self, jid, path):
            print "[log] path for job: ", jid, "is: ", path

        def logBackwardPathTime(self, finalTime, enqueueTime, backwardLink, deltaTime):
            print "[log] [backward_path]: final time:   ", finalTime, "enqueue time:    ", enqueueTime, "backward link time:    ", backwardLink, "and response time:    ", deltaTime

        def logTrace(self, jid, trace, trace_length):
            print "[log] job.trace for job:", jid, "is: ", trace, "length of timeline:", trace_length

        def logTracePath(self, jid, trace_path, trace_path_length):
            print "[log] job.trace_path for job:", jid, "is: ", trace_path, "length of trace:", trace_path_length

	def logArrival(self, index, time):
		pass

	def logFinishedFirstProcess(self, index, time):
		pass

	def logSendCache(self, parent, child, time):
		pass

	def logCacheHit(self, index, result):
		pass

	def logSendChild(self, parent, child, time):
		pass

	def logFinishedSecondProcess(self, index, time):
		pass

	def logSendParent(self, child, parent, time):
		pass

	def logSQLHit(self, index, result):
		pass

	def logDelayed(self, index, time1, time2):
		pass

class CmdLineLogger:
	def __init__(self, index_name_dict):
		self.index_to_name = index_name_dict

	def logArrival(self, index, time):
		print "Queue "+str(self.index_to_name[index])+", "+ str(index)+", got job, time is at " + str(time)

	def logFinishedFirstProcess(self, index, time):
		print "Queue "+str(self.index_to_name[index])+", "+ str(index)+", finished first round processing, time is at "+str(time)

	def logSendCache(self, parent, child, time):
		print "Queue "+str(self.index_to_name[parent])+", "+str(parent)+", sending to cache queue "+str(self.index_to_name[child])+", "+str(child)+", time is at "+str(time)

	def logCacheHit(self, index, result):
		print "Queue "+str(self.index_to_name[index])+", "+ str(index)+", hits cache: "+str(result)

	def logSendChild(self, parent, child, time):
		print "Queue "+str(self.index_to_name[parent])+", "+str(parent)+", sending to child queue "+str(self.index_to_name[child])+", "+str(child)+", time is at "+str(time)

	def logFinishedSecondProcess(self, index, time):
		print "Queue "+str(self.index_to_name[index])+", "+ str(index)+", finished second round processing, time is at "+str(time)

	def logSendParent(self, child, parent, time):
		print "Queue "+str(self.index_to_name[child])+", "+str(child)+", sending to parent queue "+str(self.index_to_name[parent])+", "+str(parent)+", time is at "+str(time)

	def logSQLHit(self, index, result):
		print "Queue "+str(self.index_to_name[index])+", "+ str(index)+", hits SQL: "+str(result)

	def logDelayed(self, index, time1, time2):
		print "Queue "+str(self.index_to_name[index])+", "+ str(index)+", delayed from time "+str(time1)+" to time "+str(time2)
