from threading import *
import numpy as np
import memcached_job
import random

TOTAL_RUNS = 1000

#arrival_lambda = 0.6
arrival_lambda = 0.1

n_jobs_finished = 0

thread_lock = Semaphore(value=1)

tcp_lambda = np.random.uniform(0.22, 0.28)
# tcp_lambda = 0.25
libevent_lambda = 0.5
# memcached_lambda = 0.5
memcached_lambda = np.random.uniform(0.47, 0.53)
read_lambda = 0.556

n_conn = 10
n_thread = 1

debug_mode = True

if debug_mode:
    screen_lock = Semaphore(value=1)

QUEUE_RATE = 0

STATE_IDLE = 0
STATE_TCP = 1
STATE_LIBEVENT = 2
STATE_READ = 3
STATE_MEMCACHED = 4


class MemcachedProc(Thread):
    def __init__(self, n_jobs_total, tcp_lambda, libevent_lambda, read_lambda, memcached_lambda, n_conn, debug):
        Thread.__init__(self)

        self.tcp_lambda = tcp_lambda
        self.libevent_lambda = libevent_lambda
        self.memcached_lambda = memcached_lambda
        self.read_lambda = read_lambda
        self.state = STATE_IDLE
        self.daemon = True
        self.debug = debug

        self.curr_time = 0
        self.current_process_time = 0

        # self.step_recorder = {}
        self.time_recorder = {}

        self.n_jobs_total = n_jobs_total
        self.n_jobs_finished = 0

        self.avg_tcp_queueing_time = 0
        self.avg_libevent_queueing_time = 0
        self.avg_read_queueing_time = 0
        self.avg_memcached_queueing_time = 0

        self.avg_total_time_tail_99 = 0
        self.avg_total_time_tail_999 = 0
        self.avg_total_time = 0

        self.jid_processed = []

        self.latency = []

        self.n_conn = n_conn

        self.tcp_queue = []
        self.libevent_queue = []
        self.read_queue = []
        self.memcached_queue = []
        for i in xrange(self.n_conn):
            self.read_queue.append([])
        self.active_read_conns = []

    def tcp_time(self):
        return np.random.exponential(scale=(1 / float(self.tcp_lambda)))

        # return np.random.exponential(scale=(np.random.uniform(2, 6)))
        # return 1 / float(self.tcp_lambda)

    def libevent_time(self):
        return np.random.exponential(scale=(1 / float(self.libevent_lambda)))
        # return np.random.exponential(scale=(np.random.uniform(1, 3)))
        # return 1 / float(self.libevent_lambda)

    def read_time(self):
        return np.random.exponential(scale=(1 / float(self.read_lambda)))
        # return np.random.exponential(scale=(np.random.uniform(0.8, 2.8)))
        # return 1 / float(self.memcached_lambda)

    def memcached_time(self):
        return np.random.exponential(scale=(1 / float(self.memcached_lambda)))
        # return np.random.exponential(scale=(np.random.uniform(1, 3)))
        # return 1 / float(self.read_lambda)

    def add_job(self, job):
        # create new stamp if this is the first time visiting this queue along this path
        # stamp = st.Stamp()
        # stamp.enqueue_time = job.time
        # job.trace.append(job.time)
        #                              tcp,   libevent, read,  memcached
        # self.step_recorder[job.jid] = [False, False,    False, False]
        #                              arrive,   tcp_in, tcp_out, libevent_in, libevent_out, read_in, read_out
        self.time_recorder[job.jid] = [job.time, 0, 0, 0, 0, 0, 0,
                                       #                              memcached_in, memcached_out
                                       0, 0]

        self.tcp_queue.append(job)

    def run(self):
        global n_jobs_finished
        while n_jobs_finished < self.n_jobs_total:

            if self.state == STATE_IDLE:
                tcp_queue_head_time = 0
                memcached_queue_head_time = 0
                read_queue_head_time = 0
                libevent_queue_head_time = 0

                if len(self.libevent_queue) > 0 and len(self.memcached_queue) == 0 and len(self.active_read_conns) == 0:
                    libevent_queue_head_time = self.libevent_queue[0].time
                    if libevent_queue_head_time <= self.curr_time:
                        self.state = STATE_LIBEVENT
                        continue

                if len(self.tcp_queue) > 0:
                    tcp_queue_head_time = self.tcp_queue[0].time
                    if tcp_queue_head_time <= self.curr_time:
                        self.state = STATE_TCP
                        continue

                if len(self.memcached_queue) > 0:
                    memcached_queue_head_time = self.memcached_queue[0].time
                    if memcached_queue_head_time <= self.curr_time:
                        self.state = STATE_MEMCACHED
                        continue

                if len(self.active_read_conns) > 0:
                    read_queue_head_time = self.read_queue[self.active_read_conns[0]][0].time
                    if read_queue_head_time <= self.curr_time:
                        self.state = STATE_READ
                        continue

                time_list = []
                if not tcp_queue_head_time == 0:
                    time_list.append(tcp_queue_head_time)
                if not memcached_queue_head_time == 0:
                    time_list.append(memcached_queue_head_time)
                if not read_queue_head_time == 0:
                    time_list.append(read_queue_head_time)
                if not libevent_queue_head_time == 0:
                    time_list.append(libevent_queue_head_time)
                if time_list != []:
                    self.curr_time = min(time_list)
                    continue

            elif self.state == STATE_TCP:
                job = self.tcp_queue.pop(0)
                self.time_recorder[job.jid][1] = self.curr_time
                if self.debug:
                    screen_lock.acquire()
                    print '[log]: conn', job.conn_id, 'job', job.jid, 'tcp_in at', self.curr_time
                    screen_lock.release()
                self.current_process_time = self.tcp_time()
                self.curr_time += self.current_process_time
                # self.step_recorder[job.jid][0] = True
                self.time_recorder[job.jid][2] = self.curr_time
                job.time = self.curr_time
                self.libevent_queue.append(job)
                self.state = STATE_IDLE
                if self.debug:
                    screen_lock.acquire()
                    print '[log]: conn', job.conn_id, 'job', job.jid, 'tcp_out at', self.curr_time
                    screen_lock.release()
                continue
            elif self.state == STATE_LIBEVENT:
                queue_len = len(self.libevent_queue)
                self.current_process_time = self.libevent_time()
                for i in xrange(queue_len):
                    if self.libevent_queue[0].time <= self.curr_time:
                        job = self.libevent_queue.pop(0)
                        self.time_recorder[job.jid][3] = self.curr_time
                        if self.debug:
                            screen_lock.acquire()
                            print '[log]: conn', job.conn_id, 'job', job.jid, 'libevent_in at', self.curr_time
                            screen_lock.release()
                        self.time_recorder[job.jid][4] = self.curr_time + self.current_process_time
                        job.time = self.curr_time
                        if self.debug:
                            screen_lock.acquire()
                            print '[log]: conn', job.conn_id, 'job', job.jid, 'libevent_out at', self.curr_time + self.current_process_time
                            screen_lock.release()
                        self.read_queue[job.conn_id].append(job)
                        if job.conn_id not in self.active_read_conns:
                            self.active_read_conns.append(job.conn_id)
                    else:
                        break
                self.curr_time += self.current_process_time
                self.state = STATE_IDLE
                continue

            elif self.state == STATE_READ:
                active_conn = self.active_read_conns.pop(0)
                queue_len = len(self.read_queue[active_conn])
                self.current_process_time = self.read_time()
                for i in xrange(queue_len):
                    if self.read_queue[active_conn][0].time <= self.curr_time:
                        job = self.read_queue[active_conn].pop(0)
                        self.time_recorder[job.jid][5] = self.curr_time
                        if self.debug:
                            screen_lock.acquire()
                            print '[log]: conn', job.conn_id, 'job', job.jid, 'read_in at', self.curr_time
                            screen_lock.release()
                        self.time_recorder[job.jid][6] = self.curr_time + self.current_process_time
                        job.time = self.curr_time
                        if self.debug:
                            screen_lock.acquire()
                            print '[log]: conn', job.conn_id, 'job', job.jid, 'read_out at', self.curr_time
                            screen_lock.release()
                        self.memcached_queue.append(job)
                    else:
                        break
                self.curr_time += self.current_process_time
                self.state = STATE_IDLE
                continue

            elif self.state == STATE_MEMCACHED:
                job = self.memcached_queue.pop(0)
                self.time_recorder[job.jid][7] = self.curr_time
                if self.debug:
                    screen_lock.acquire()
                    print '[log]: conn', job.conn_id, 'job', job.jid, 'memcached_in at', self.curr_time
                    screen_lock.release()
                self.current_process_time = self.memcached_time()
                self.curr_time += self.current_process_time
                # self.step_recorder[job.jid][0] = True
                self.time_recorder[job.jid][8] = self.curr_time
                job.time = self.curr_time
                if self.debug:
                    screen_lock.acquire()
                    print '[log]: conn', job.conn_id, 'job', job.jid, 'memcached_out at', self.curr_time
                    screen_lock.release()
                thread_lock.acquire()
                n_jobs_finished += 1
                thread_lock.release()
                self.n_jobs_finished += 1

                self.jid_processed.append(job.jid)
                self.state = STATE_IDLE
                if n_jobs_finished % 10000 == 0:
                    print n_jobs_finished, 'jobs finished'
                continue

            else:
                pass

        for jid in self.jid_processed:
            self.avg_tcp_queueing_time += self.time_recorder[jid][1] - self.time_recorder[jid][0]
            self.avg_libevent_queueing_time += self.time_recorder[jid][3] - self.time_recorder[jid][2]
            self.avg_read_queueing_time += self.time_recorder[jid][5] - self.time_recorder[jid][4]
            self.avg_memcached_queueing_time += self.time_recorder[jid][7] - self.time_recorder[jid][6]
            self.avg_total_time += self.time_recorder[jid][8] - self.time_recorder[jid][0]
            self.latency.append(self.time_recorder[jid][8] - self.time_recorder[jid][0])

        self.avg_tcp_queueing_time /= self.n_jobs_total
        self.avg_libevent_queueing_time /= self.n_jobs_total
        self.avg_read_queueing_time /= self.n_jobs_total
        self.avg_memcached_queueing_time /= self.n_jobs_total

        self.latency = sorted(self.latency)
        self.avg_total_time_tail_99 = sum(self.latency[int(0.99 * self.n_jobs_finished):]) / int(
            0.01 * self.n_jobs_finished)
        self.avg_total_time_tail_999 = sum(self.latency[int(0.999 * self.n_jobs_finished):]) / int(
            0.001 * self.n_jobs_finished)
        self.avg_total_time /= self.n_jobs_total
        thread_lock.acquire()
        print '#######################################################################'
        print 'tcp:         ', self.avg_tcp_queueing_time
        print 'libevent:    ', self.avg_libevent_queueing_time
        print 'read:        ', self.avg_read_queueing_time
        print 'memcached:   ', self.avg_memcached_queueing_time
        print 'total:       ', self.avg_total_time
        print '99th tail    ', self.avg_total_time_tail_99
        print '99.9th tail: ', self.avg_total_time_tail_999
        print 'std:         ', np.std(self.latency)
        print '#######################################################################'
        thread_lock.release()


def inter_arrival_time():
    return np.random.exponential(scale=(1 / float(arrival_lambda)))
    # return 1 / float(arrival_lambda)


if __name__ == '__main__':

    # assert len(sys.argv) == 2 or (len(sys.argv) == 3 and sys.argv[2] == 'log')
    # global n_jobs_finished
    memcached_workers = []
    for i in xrange(n_thread):
        memcached_workers.append(
            MemcachedProc(TOTAL_RUNS, tcp_lambda, libevent_lambda, read_lambda, memcached_lambda, n_conn, debug_mode))
        memcached_workers[i].start()

    jobs = []
    arrival_time = 0
    jid = 0
    # start_time = 0


    for i in xrange(TOTAL_RUNS):
        j = memcached_job.Job()
        jobs.append(j)
        j.time = arrival_time
        j.jid = jid
        j.conn_id = random.randint(0, n_conn - 1)
        if debug_mode:
            # print 'job.time of job: ', j, ' is: ', job.time
            # print 'index_name_dict is: ', index_name_dict[src]
            screen_lock.acquire()
            print '[log]: conn', j.conn_id, 'job', j.jid, ' arrives at', arrival_time
            screen_lock.release()

        memcached_workers[j.conn_id % n_thread].add_job(j)
        arrival_time = arrival_time + inter_arrival_time()
        jid = jid + 1

    for i in xrange(n_thread):
        memcached_workers[i].join()
