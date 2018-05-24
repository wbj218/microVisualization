import threading
import Queue
import stamp as st
import random
from threading import *
screen_lock = Semaphore(value=1)

# the container class that handles adding jobs and initialization
class NetworkQueueContainer(threading.Thread):
    def __init__(self, index, children, caches, backlinks, network_graph, first_process_time, second_process_time, forward_link, backward_link, rate, index_source_dict, index_target_dict, logger):
        threading.Thread.__init__(self)

        self.job_queue = Queue.Queue()
        self.daemon = True
        self.index = index
        self.logger = logger
        self.server = NetworkQueue(index, children, caches, backlinks, network_graph, first_process_time, second_process_time, forward_link, backward_link, self.job_queue, rate, index_source_dict, index_target_dict, logger)

    def add_job(self, job):
        # create new stamp if this is the first time visiting this queue along this path

        if self.index not in job.stamp_dict:
            stamp = st.Stamp()
            job.stamp_dict[self.index] = stamp
            job.path.append(self.index)
            
            screen_lock.acquire()
            #print "[log]: initial enqueue time:        ", job.time
            self.logger.logInitEnqueue(job.jid, job.time)
            screen_lock.release()
            stamp.enqueue_time = job.time
            job.trace.append(job.time)

        self.job_queue.put(job)

    def run(self):
        self.server.start()

# the worker class that constantly gets jobs and processes them
class NetworkQueue(threading.Thread):
    def __init__(self, index, children, caches, backlinks, network_graph, first_process_time, second_process_time, forward_link, backward_link, job_queue, rate, index_source_dict, index_target_dict, logger):
        threading.Thread.__init__(self)

        self.children = children
        self.first_process_time = first_process_time
        self.second_process_time = second_process_time
        self.forward_link = forward_link
        self.backward_link = backward_link
        self.job_queue = job_queue
        self.network_graph = network_graph
        self.backlinks = backlinks
        self.index = index
        self.daemon = True
        self.rate = rate
        self.next_available = 0
        self.index_source_dict = index_source_dict
        self.index_target_dict = index_target_dict
        self.logger = logger
        self.caches = caches

    # whether or not we hit the cache
    def hits_cache(self):
        if random.randint(0,9) < 4:
            return True
        else:
            return False

    # main method
    def run(self):
        # loop forever (constantly process jobs)
        while True:
            # gets a job and access the stamp dictionary to get the stamp for this server
            job = self.job_queue.get()
            stamp = job.stamp_dict[self.index]

            self.logger.logArrival(self.index, job.time)

            # if haven't visited, this is the first visit
            if not stamp.visited():
                # waits until server is ready
                if job.time < self.next_available:
                    self.logger.logDelayed(self.index, job.time, self.next_available)
                    job.time = self.next_available

                #print "log: job.time", job.time
                stamp.picked_time = job.time
                job.trace.append(job.time)

                # first process here
                current_process_time = self.first_process_time()
                stamp.processed_time = job.time + current_process_time
                # self.next_available = self.next_available + max(self.rate, current_process_time)

                #screen_lock.acquire()
                # print "[log]: next_available:              ", self.next_available
                # print "[log]: current_process_time:        ", current_process_time
                # print "[log]: stamp.processed_time:        ", stamp.processed_time
                # print stamp.processed_time
                #screen_lock.release()
                job.time = stamp.processed_time
                job.trace.append(job.time)

                self.logger.logFinishedFirstProcess(self.index, job.time)

            # if we haven't checked all caches, check them
            if not stamp.checked_cache:
                # checks caches
                cont_flag = False
                for child in self.caches:
                    if not child in stamp.values:
                        stamp.cache_send_time = stamp.processed_time + self.forward_link()
                        job.time = stamp.cache_send_time
                        #TODO: fix naming of child source and target
                        #print "[log] child is: ", self.index_source_dict[child]
                        #print "[log] child is: ", self.index_target_dict[child]
                        job.trace.append(job.time)
                        self.logger.logSendCache(self.index, child, stamp.processed_time)
                        self.network_graph[child].add_job(job)
                        cont_flag = True
                        break

                # cont flag will be on as long as we sent to a cache
                if cont_flag:
                    continue

                # if we are here, we have checked all caches
                stamp.checked_cache = True

                # check if we hit cache
                if len(self.caches) == 0:
                    stamp.cache_result = False

                if stamp.cache_result == None:
                    stamp.cache_result = self.hits_cache()
                    self.logger.logCacheHit(self.index, stamp.cache_result)

            # if we did not hit cache, send to children
            if not stamp.cache_result:
                # finds time server sends to children
                if len(self.caches) == 0:
                    prior_to_send = stamp.processed_time
                    stamp.send_time = stamp.processed_time + self.forward_link()
                else:
                    values = []
                    for child in self.caches:
                        values.append(stamp.values[child])
                        #TODO: fix naming of child source and target
                        screen_lock.acquire()
                        self.logger.logChild(child, self.index_source_dict[child-1])
                        #print "source_prev before", job.source_prev
                        job.trace_path.append('queue('+self.index_source_dict[child-1]+')')
                        job.trace_path.append('proc('+self.index_source_dict[child-1]+')')
                        job.source_prev = self.index_source_dict[child-1]
                        #print "source_prev after", job.source_prev
                        screen_lock.release()
                        screen_lock.acquire()
                        self.logger.logChild(child, self.index_target_dict[child-1])
                        job.trace_path.append('queue('+self.index_target_dict[child-1]+')')
                        job.trace_path.append('proc('+self.index_target_dict[child-1]+')')
                        screen_lock.release()
                    max_value = 0 if not values else max(values)

                    stamp.send_time = stamp.cache_send_time + max_value + self.forward_link()
                    prior_to_send = stamp.cache_send_time + max_value

                # finds the next child to send to
                cont_flag = False
                for child in self.children:
                    if not child in stamp.values:
                        job.time = stamp.send_time
                        job.trace.append(job.time)
                        screen_lock.acquire()
                        self.logger.logChild(child, self.index_source_dict[child-1])
                        #print "source_prev before", job.source_prev
                        #print "target_prev before", job.target_prev
                        #if the previous child's parent is different, add the parent to the trace path, otherwise they are consecurive children of the same parent
                        #if ((job.source_prev != self.index_source_dict[child-1]) and (job.target_prev != self.index_source_dict[child-1])):
                        #cdel: always append the parent and the child
                        job.trace_path.append('queue('+self.index_source_dict[child-1]+')')
                        job.trace_path.append('proc('+self.index_source_dict[child-1]+')')
                        job.source_prev = self.index_source_dict[child-1]
                        job.target_prev = self.index_target_dict[child-1]
                        #print "source_prev after", job.source_prev
                        #print "target_prev after", job.target_prev
                        screen_lock.release()
                        screen_lock.acquire()
                        self.logger.logChild(child, self.index_target_dict[child-1])
                        job.trace_path.append('queue('+self.index_target_dict[child-1]+')')
                        job.trace_path.append('proc('+self.index_target_dict[child-1]+')')
                        screen_lock.release()
                        self.logger.logSendChild(self.index, child, prior_to_send)
                        self.network_graph[child].add_job(job)
                        cont_flag = True
                        break

                # cont flag will be on as long as we sent to a child
                if cont_flag:
                    continue

            # all children have value at this point
            # we can either get from cache or children depending on cache hit
            values = []
            if not stamp.cache_result and len(self.children) > 0:
                send_time = stamp.send_time
                for child in self.children:
                    values.append(stamp.values[child])
            else:
                send_time = stamp.cache_send_time
                for child in self.caches:
                    values.append(stamp.values[child])

            # get max value
            max_value = 0 if not values else max(values)

            if max_value == 0:
                stamp.final_time = stamp.processed_time
            else:
                stamp.final_time = send_time + max_value

            #TODO: This needs a major rewrite, the ordering makes no sense
            # if we ever sent to children or caches, we need to add another round of process time
            #TODO: backlink needs to be specifically for this child: if the backlink from this child exists, then proceed with the second processing
            
            #if not (len(self.children) == 0 and len(self.caches) == 0 and len(self.backlinks) == 0):
            if not (len(self.children) == 0 and len(self.caches) == 0):
                if stamp.final_time < self.next_available:
                    self.logger.logDelayed(self.index, stamp.final_time, self.next_available)
                    stamp.final_time = self.next_available

                #print "I'm in the backlinks"
                current_process_time = self.second_process_time()
                stamp.final_time = stamp.final_time + current_process_time
                self.next_available = stamp.final_time
                self.logger.logFinishedSecondProcess(self.index, stamp.final_time)
            

            self.next_available = stamp.final_time
            stamp.delta_time = stamp.final_time - stamp.enqueue_time
            screen_lock.acquire()
            job.trace.append(stamp.final_time)
            self.logger.logForwardPathTime(stamp.final_time, stamp.enqueue_time, stamp.delta_time)
            job.time = stamp.delta_time
            screen_lock.release()


            #print "before job.path for job:", job, "is:", job.path
            job.path = job.path[0:len(job.path)-1]
            #print "after job.path for job:", job, "is:", job.path
            #screen_lock.acquire()
            #screen_lock.release()

            """
                if stamp.final_time < self.next_available:
                    self.logger.logDelayed(self.index, stamp.final_time, self.next_available)
                    stamp.final_time = self.next_available

                #print "I'm in the backlinks"
                current_process_time = self.second_process_time()
                stamp.final_time = stamp.final_time + current_process_time
                job.trace.append(stamp.final_time)
                self.next_available = stamp.final_time
                self.logger.logFinishedSecondProcess(self.index, stamp.final_time)
            """

            # if there is a parent, send the result back to the parent
            if len(job.path) != 0:
            #if len(self.backlinks) != 0:
                temp = stamp.final_time
                stamp.final_time = stamp.final_time + self.backward_link()
                screen_lock.acquire()
                job.trace.append(stamp.final_time)
                stamp.delta_time = stamp.final_time - stamp.enqueue_time
                self.logger.logBackwardPathTime(stamp.final_time, stamp.enqueue_time, self.backward_link(), stamp.delta_time)
                screen_lock.release()
                job.time = stamp.delta_time
                parent = job.path[len(job.path)-1]
                self.logger.logSendParent(self.index, parent, temp)
                job.stamp_dict[parent].values[self.index] = stamp.delta_time
                del job.stamp_dict[self.index]
                self.network_graph[parent].add_job(job)
            #else:
            #    parent = job.path[len(job.path)-1]
            #    job.stamp_dict[parent].values[self.index] = stamp.delta_time
            #    del job.stamp_dict[self.index]
            #    self.network_graph[parent].add_job(job)
            
            screen_lock.acquire()
            #print job.time
            self.logger.logTrace(job.jid, job.trace, len(job.trace))
            self.logger.logTracePath(job.jid, job.trace_path, len(job.trace_path))
            screen_lock.release()
            #screen_lock.release()
            #screen_lock.acquire()
