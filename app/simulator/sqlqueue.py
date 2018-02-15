import threading
import Queue
import stamp as st
import random

# note: similar to networkqueue, extensive comments are excluded
class SQLQueueContainer(threading.Thread):
    def __init__(self, index, children, caches, network_graph, first_process_time, second_process_time, forward_link, backward_link, rate, inded_source_dict, index_target_dict, logger):
        threading.Thread.__init__(self)

        self.job_queue = Queue.Queue()
        self.daemon = True
        self.index = index
        self.server = SQLQueue(index, children, caches, network_graph, first_process_time, second_process_time, forward_link, backward_link, self.job_queue, rate, index_source_dict, index_target_dict, logger)

    def add_job(self, job):
        if self.index not in job.stamp_dict:
            stamp = st.Stamp()
            job.stamp_dict[self.index] = stamp
            job.path.append(self.index)

            stamp.enqueue_time = job.time

        self.job_queue.put(job)

    def run(self):
        self.server.start()

class SQLQueue(threading.Thread):
    def __init__(self, index, children, caches, network_graph, first_process_time, second_process_time, forward_link, backward_link, job_queue, rate, inde_source_dict, index_target_dict, logger):
        threading.Thread.__init__(self)

        self.children = children
        self.first_process_time = first_process_time
        self.second_process_time = second_process_time
        self.forward_link = forward_link
        self.backward_link = backward_link
        self.job_queue = job_queue
        self.network_graph = network_graph
        self.index = index
        self.daemon = True
        self.rate = rate
        self.next_available = 0
        self.logger = logger
        self.caches = caches

    def hits_cache(self):
        if random.randint(0,9) < 4:
            return True
        else:
            return False

    def hits_sql(self):
        if random.randint(0,3) == 0:
            return True
        else:
            return False

    def run(self):
        while True:
            job = self.job_queue.get()
            stamp = job.stamp_dict[self.index]

            self.logger.logArrival(self.index, job.time)

            if not stamp.visited():
                # waits until server is ready
                if job.time < self.next_available:
                    self.logger.logDelayed(self.index, job.time, self.next_available)
                    job.time = self.next_available

                stamp.picked_time = job.time

                current_process_time = self.first_process_time()
                stamp.processed_time = job.time + current_process_time
                self.next_available = self.next_available + max(self.rate, current_process_time)
                job.time = stamp.processed_time

                self.logger.logFinishedFirstProcess(self.index, job.time)

            if stamp.sql_result == None:
                stamp.sql_result = self.hits_sql()
                self.logger.logSQLHit(self.index, stamp.sql_result)

            if not stamp.checked_cache and not stamp.sql_result:
                # checks caches
                cont_flag = False
                for child in self.caches:
                    if not child in stamp.values:
                        stamp.cache_send_time = stamp.processed_time + self.forward_link()
                        job.time = stamp.cache_send_time
                        self.logger.logSendCache(self.index, child, stamp.processed_time)
                        self.network_graph[child].add_job(job)
                        cont_flag = True
                        break

                if cont_flag:
                    continue

                stamp.checked_cache = True

                if len(self.caches) == 0:
                    stamp.cache_result = False

                if stamp.cache_result == None:
                    stamp.cache_result = self.hits_cache()
                    self.logger.logCacheHit(self.index, stamp.cache_result)

            if not stamp.cache_result and not stamp.sql_result:
                # finds time server sends to children
                if len(self.caches) == 0:
                    prior_to_send = stamp.processed_time
                    stamp.send_time = stamp.processed_time + self.forward_link()
                else:
                    values = []
                    for child in self.caches:
                        values.append(stamp.values[child])
                    max_value = 0 if not values else max(values)

                    stamp.send_time = stamp.cache_send_time + max_value + self.forward_link()
                    prior_to_send = stamp.cache_send_time + max_value

                # finds the next child to send to
                cont_flag = False
                for child in self.children:
                    if not child in stamp.values:
                        job.time = stamp.send_time
                        self.logger.logSendChild(self.index, child, prior_to_send)
                        self.network_graph[child].add_job(job)
                        cont_flag = True
                        break

                if cont_flag:
                    continue

            if stamp.sql_result:
                stamp.final_time = stamp.processed_time
            else:
                # all children have value at this point
                values = []
                if not stamp.cache_result and len(self.children) > 0:
                    send_time = stamp.send_time
                    for child in self.children:
                        values.append(stamp.values[child])
                else:
                    send_time = stamp.cache_send_time
                    for child in self.caches:
                        values.append(stamp.values[child])

                max_value = 0 if not values else max(values)
                
                if max_value == 0:
                    stamp.final_time = stamp.processed_time
                else:
                    stamp.final_time = send_time + max_value

                # if we ever sent to children or caches, we need to add another round of process time
                if not (len(self.children) == 0 and len(self.caches) == 0):
                    if stamp.final_time < self.next_available:
                        self.logger.logDelayed(self.index, stamp.final_time, self.next_available)
                        stamp.final_time = self.next_available
                    
                    current_process_time = self.second_process_time()
                    stamp.final_time = stamp.final_time + current_process_time
                    self.next_available = self.next_available + max(self.rate, current_process_time)
                    self.logger.logFinishedSecondProcess(self.index, stamp.final_time)

            stamp.delta_time = stamp.final_time - stamp.enqueue_time
            job.time = stamp.delta_time

            job.path = job.path[0:len(job.path)-1]

            if len(job.path) != 0:
                temp = stamp.final_time
                stamp.final_time = stamp.final_time + self.backward_link()
                stamp.delta_time = stamp.final_time - stamp.enqueue_time
                job.time = stamp.delta_time
                parent = job.path[len(job.path)-1]
                self.logger.logSendParent(self.index, parent, temp)
                job.stamp_dict[parent].values[self.index] = stamp.delta_time
                del job.stamp_dict[self.index]
                self.network_graph[parent].add_job(job)
