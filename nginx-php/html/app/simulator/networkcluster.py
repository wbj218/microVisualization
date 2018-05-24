import Queue

class NetworkCluster:
	def __init__(self):
		self.clusterQ = Queue.Queue()

	def add_member(self, member):
		self.clusterQ.put(member)

	def add_job(self, job):
		member = self.clusterQ.get()
		member.add_job(job)
		self.clusterQ.put(member)
	