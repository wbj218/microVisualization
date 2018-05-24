class Stamp:
	def __init__(self):
		self.enqueue_time = -1
		self.picked_time = -1
		self.processed_time = -1
		self.send_time = -1
		self.cache_send_time = -1
		self.cache_get_back_time = -1
		self.final_time = -1
		self.delta_time = -1
		self.values = {}
		self.checked_cache = False
		self.cache_result = None
		self.sql_result = None

	def visited(self):
		return self.picked_time != -1