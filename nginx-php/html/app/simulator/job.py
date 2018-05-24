class Job:
	def __init__(self):
		self.stamp_dict = {}
		self.time = 0
                self.jid = 0
                self.source_prev = 0
                self.target_prev = 0
		self.path = []
		self.trace_path = []
		self.trace = []
