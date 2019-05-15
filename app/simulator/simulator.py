import networkqueue as nq
import numpy as np
import sys
import json
import job
import time
import random
import networkcluster as nc
import sqlqueue as sq
import logger as lg
from threading import *
screen_lock = Semaphore(value=1)

RELATIVE_PATH = "../inputs/" # relative path to inputs
CACHE_NAME = "cache" # name to search for caches
SQL_NAME = "mysql" # name to search for sql servers
#TOTAL_RUNS = 100000
#TOTAL_RUNS = 100000
TOTAL_RUNS = 10
#QUEUE_RATE = 100
QUEUE_RATE = 0 # server request time

# Main method that starts program
if __name__ == "__main__":
    assert((len(sys.argv) == 4) or (len(sys.argv) == 5 and (sys.argv[4] == "-v" or (sys.argv[4] == "log"))))

    input_file = sys.argv[1]
    arrival_lambda = int(sys.argv[2])
    proc_lambda = int(sys.argv[3])
    num_nodes = 0
    num_sources = 0
    num_targets = 0
    name_index_dict = {}
    index_name_dict = {}
    source_index_dict = {}
    index_source_dict = {}
    target_index_dict = {}
    index_target_dict = {}
    bidirectional = {}
    sources = []
    edges = []
    edge_targets = []
    edge_sources = []

    # functions for defining process and link times
    def inter_arrival_time():
        return np.random.exponential(scale=(1/float(arrival_lambda)))

    def first_process_time():
        #return np.random.normal(loc = 10.0, scale=10)
        return np.random.exponential(scale=(1/float(proc_lambda)))

    def second_process_time():
        #return np.random.normal(loc = 20.0, scale = 2)
        return np.random.exponential(scale=(1/float(proc_lambda)))

    def forward_link():
        #return np.random.normal(loc = 10.0, scale = 10)
        return np.random.exponential(scale=(1/float(proc_lambda)))

    def backward_link():
        #return np.random.normal(loc = 20.0, scale = 2)
        return np.random.exponential(scale=(1/float(proc_lambda)))

    # obtains the raw name of the microservice (application name without numbers)
    def get_raw_name(s):
        skip = 0
        for i in xrange(len(s)):
            c = s[len(s)-i-1]
            if c.isdigit():
                skip = skip + 1
            else:
                break

        return s[:len(s)-skip]

    # check for verbose mode
    if (len(sys.argv)==5):
        if (sys.argv[4] == "-v"):
            logger = lg.CmdLineLogger(index_name_dict)
        elif (sys.argv[4] == "log"):
            logger = lg.CmdLineQLogger(index_name_dict)
    else:
        logger = lg.DummyLogger()

    # opens and reads input file for data
    with open(RELATIVE_PATH + input_file) as data_file:
        data = json.load(data_file)
        #exist_edge = False

        # collects nodes and edges
        for item in data["graph"]:
            if "node" in item:
                name_index_dict[str(item["node"])] = num_nodes
                index_name_dict[num_nodes] = str(item["node"])
                #if item["node"].find(src_name) != -1:
                #    sources.append(num_nodes)
                num_nodes = num_nodes + 1
            elif "edge" in item:
                edges.append( (str(item["source"]), str(item["target"])) )
                source_index_dict[str(item["source"])] = num_sources
                index_source_dict[num_sources] = str(item["source"])
                target_index_dict[str(item["target"])] = num_targets
                index_target_dict[num_targets] = str(item["target"])
                if (item["bidirectional"]=="yes"):
                    bidirectional[num_sources] = 1
                else:
                    bidirectional[num_sources] = 0
                num_sources = num_sources + 1
                num_targets = num_targets + 1
                edge_sources.append(str(item["source"]))
                edge_targets.append(str(item["target"]))
                #exist_edge = True

        # find neighbors for nodes
        for item in data["graph"]:
            if "node" in item:
                if ((str(item["node"]) in edge_sources) & (str(item["node"]) not in edge_targets)):
                    sources.append(name_index_dict[str(item["node"])])

        if not sources:
            sources.append(name_index_dict[str(item["node"])])

        #print "[log] sources are: ", sources

        #TODO: check if adding str() to item["node"], item["edge"] above messes up the bundle graph
        #bundle graph input file
        f = open('../bundle_inputs/templated_'+sys.argv[1], 'w+')
        f.write("[\n")
        empty = True
        for item in data["graph"]:
            #print item
            bundle_edges = []
            if "node" in item:
                if item["node"] in edge_sources:
                    #TODO: need to find how many targets have this node as edge
                    for i in xrange(0,len(edge_sources)):
                        if (index_source_dict[i] == item["node"]):
                            #tmp_target = source_index_dict[]
                            bundle_edges.append(edge_targets[i]) #source_index_dict[item["node"]]])
                elif item["node"] not in edge_sources:
                    for i in xrange(0,len(edge_targets)):
                        if (index_target_dict[i] == item["node"]):
                            bundle_edges.append(edge_sources[i]) #source_index_dict[item["node"]]])
                if (len(bundle_edges)!=0):
                    if empty is True:
                        empty = False
                    else:
                        f.write(",\n")
                    f.write("{\"name\":\""+item["node"]+"\",\"size\":1000,\"edges\":[\""+bundle_edges[0])
                for i in xrange(1,len(bundle_edges)-1):
                    f.write("\",\""+bundle_edges[i])
                f.write("\"]}")
        f.write("\n]\n")
        f.close()

        sys.exit(0)

        #sankey graph input file
        appNameFull = sys.argv[1]
        appName = appNameFull.split('.',2)
        #print appName[0]
        f = open('../sankey_inputs/sankey_'+appName[0]+'.csv', 'w+')
        f.write("source,target,value\n")
        for item in data["graph"]:
            #print item
            sankey_edges = []
            if "edge" in item:
                source = item["source"]
                target = item["target"]
                f.write(source+","+target+",1\n")
        f.close()


    adj_lst = {}
    back_lst = {}
    cache_lst = {}

    for i in xrange(num_nodes):
        adj_lst[i] = []
        back_lst[i] = []
        cache_lst[i] = []

    # create maps for adjacencies, backlinks, and caches of a node
    for edge in edges:
        src = name_index_dict[edge[0]]
        tar = name_index_dict[edge[1]]
        #print "edge[0]:", edge[0], "edge[1]:", edge[1], "src:", src, "index_source_dict[src]:", index_source_dict[src], "tar:", tar, "index_target_dict[tar-1]:", index_target_dict[tar-1]
        #TODO: uncomment -- changed to [2] because of twitter graph format, change to [3] for spigo graphs
        if edge[1].split(".")[2].find(CACHE_NAME) != -1:
            cache_lst[src].append(tar)
        #if edge[1].split(".")[3].find(CACHE_NAME) != -1:
        #    cache_lst[src].append(tar)
        adj_lst[src].append(tar)
        #TODO: if bi-directional edge, add backlink
        index_src = source_index_dict[edge[0]]
        if (bidirectional[index_src]==1):
            #print "bidirectional: yes"
            back_lst[tar].append(src)
        #else:
            #print "bidirectional: no"

    network_graph = {}
    queue_list = []
    info = []

    # Depth first search to find clusters
    for src_node in sources:
        #print "src_node examined: ", src_node
        visited = set()
        stack = [src_node]
        src_cluster = nc.NetworkCluster()
        src_q = nq.NetworkQueueContainer(src_node, adj_lst[src_node], cache_lst[src_node], back_lst[src_node], network_graph, first_process_time, second_process_time, forward_link, backward_link, QUEUE_RATE, index_source_dict, index_target_dict, logger)
        queue_list.append(src_q)
        src_cluster.add_member(src_q)
        network_graph[src_node] = src_cluster
        info.append([index_name_dict[src_node]])
        #print "[log] [build] src_cluster: ", src_cluster

        # while there are nodes
        while stack:
            node = stack.pop()
            screen_lock.acquire()
            #print "[log] [build] node: ", node
            screen_lock.release()
            children = adj_lst[node]
            screen_lock.acquire()
            #print "[log] [build] children: ", children
            screen_lock.release()
            visited.add(node)

            components = {}

            # find the clusters among this node's children
            for child in children:
                if child in visited:
                    continue
                stack.append(child)
                #print "[log] [build] child: ", child
                name = index_name_dict[child]
                name_split = name.split(".")
                #TODO: uncomment -- changed to [1] and [2] for twitter graphs, change back to [2] and [3] for previous apps
                zone = name_split[1]
                app = get_raw_name(name_split[2])
                #zone = name_split[2]
                #app = get_raw_name(name_split[3])
                #app = "twitter_compose"

                if app in components:
                    components[app].append(child)
                else:
                    components[app] = []
                    components[app].append(child)

            #print "[log] components: ", components

            # for each cluster, fix the back links, forward links, and
            # instantiate the network queue
            for cluster in components:
                #print "[log] [build] cluster: ", cluster
                ncluster = nc.NetworkCluster()
                i = []
                index = components[cluster][0]
                for member in components[cluster]:
                    for s in back_lst[member]:
                        adj_lst[s].remove(member)
                        if index not in adj_lst[s] and cluster.find(CACHE_NAME) == -1:
                            adj_lst[s].append(index)
                            #gets trapped in loop, once edge visited mark
                        if cluster.find(CACHE_NAME) != -1:
                            cache_lst[s].remove(member)
                            cache_lst[s].append(index)
                    if cluster.find(SQL_NAME) != -1:
                        qc = sq.SQLQueueContainer(index, adj_lst[index], cache_lst[index], back_lst[index], network_graph, first_process_time, second_process_time, forward_link, backward_link, QUEUE_RATE, index_source_dict, index_target_dict, logger)
                    else:
                        qc = nq.NetworkQueueContainer(index, adj_lst[index], cache_lst[index], back_lst[index], network_graph, first_process_time, second_process_time, forward_link, backward_link, QUEUE_RATE, index_source_dict, index_target_dict, logger)
                    queue_list.append(qc)
                    ncluster.add_member(qc)
                    network_graph[member] = ncluster
                    i.append(index_name_dict[member])
                info.append(i)

    # fix self pointers as part of clusters (A->B, B->A will turn into A->A, A->A so we want to remove them)
    for adj in adj_lst:
        if adj in adj_lst[adj]:
            adj_lst[adj].remove(adj)
    for cache in cache_lst:
        if cache in cache_lst[cache]:
            cache_lst[cache].remove(cache)

    # start queues, they will start polling on jobs
    for queue in queue_list:
        queue.start()

    jobs = []
    arrival_time = 0
    jid = 0
    #start_time = 0
    for src in sources:
        for i in xrange(TOTAL_RUNS):
            j = job.Job()
            jobs.append(j)
            j.time = arrival_time
            j.jid = jid
            #print 'job.time of job: ', j, ' is: ', job.time
            #print "index_name_dict is: ", index_name_dict[src]
            if not edges:
                j.trace_path.append('queue('+index_name_dict[src]+')')
                j.trace_path.append('proc('+index_name_dict[src]+')')
            network_graph[src].add_job(j)
            arrival_time = arrival_time + inter_arrival_time()
            jid = jid + 1
            #print "[log] [main]: arrival_time", arrival_time

    for j in jobs:
        while j.path:
            time.sleep(0.1)


    for j in jobs:
        print j.time

    """
    #gantt graph input file
    #print 'len trace path: ', len(jobs[1].trace_path)
    #print 'len trace: ', len(jobs[1].trace)
    bidir = "no"
    #inputName = sys.argv[1]
    #appName = inputName.split('.')
    #appName = inputName[0]
    #print appName[0]
    f = open('../gantt_inputs/gantt_'+appName[0]+'.log', 'w+')
    for j in jobs:
        for i in xrange(len(j.trace_path)):
            test_source = j.trace_path[i]
            test_source = test_source.lstrip('queue(')
            test_source = test_source.lstrip('proc(')
            test_source = test_source.rstrip(')')
            #print 'test source is: ', test_source
            #print index_source_dict
            if (test_source in index_source_dict):
                current_source = source_index_dict[test_source]
                #print 'current source is: ', current_source
                bidir = "bi"
                #print 'bidirectional is: ', bidirectional[current_source]
            f.write(str(bidir)+','+str(j.trace_path[i])+','+str(j.trace[max(0,2*(i-1)+1)])+','+str(j.trace[2*i])+'\n')

    f.close()
    """
