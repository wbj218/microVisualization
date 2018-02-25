import re
import sys

class LogItem:
    def __init__(self):
        self.timestamp = 0
        self.src_addr = ""
        self.src_port = 0
        self.src_name = ""
        self.dest_addr = ""
        self.dest_port = 0
        self.dest_name = ""
        self.req_id = ""
        self.method = ""
        self.type = ""

def mmc_process_log(lines, type):
    log_items = []
    pattern_timestamp = re.compile("[0-9]+\.?[0-9]+ IP")

    for line in lines:
        if pattern_timestamp.match(line):
            words = line.split(" ")
            log_item = LogItem()
            log_item.timestamp = int(float(words[0])*1000000)
            src = words[2].split(".")
            log_item.src_addr = src[0]+"."+src[1]+"."+src[2]+"."+src[3]
            log_item.src_port = int(src[4])
            dest = words[4].split(".")
            log_item.dest_addr = dest[0]+"."+dest[1]+"."+dest[2]+"."+dest[3]
            log_item.dest_port = int(dest[4][0:-1])
        else:
            words = line.split("\"req_id\" : ")
            if words[0] != line:
                log_item.req_id = words[1].split(",")[0][1:-1]
                log_item.type = type     
                log_items.append(log_item)
    return log_items

def mongo_process_log(lines, type):
    log_items = []
    log_item = LogItem()
    pattern_timestamp = re.compile("[0-9]+\.?[0-9]+ IP")

    for line in lines:
        if pattern_timestamp.match(line):
            words = line.split(" ")
            log_item = LogItem()
            log_item.timestamp = int(float(words[0])*1000000)
            src = words[2].split(".")
            log_item.src_addr = src[0]+"."+src[1]+"."+src[2]+"."+src[3]
            log_item.src_port = int(src[4])
            dest = words[4].split(".")
            log_item.dest_addr = dest[0]+"."+dest[1]+"."+dest[2]+"."+dest[3]
            log_item.dest_port = int(dest[4][0:-1])
        else:
            words = line.split("req_id.!...")
            if words[0] != line:
                log_item.req_id = words[1].split("..")[0]      
                log_item.type = type               
                log_items.append(log_item)
    return log_items

def write_file(log_items, file):
    counter = 0
    for log_item in log_items:
        file.write(str(counter) +  ": req_id = "  + log_item.req_id + ", type = " + log_item.type + ", timestamp = "  + str(log_item.timestamp) + "\n")           
        counter += 1

def main():
    with open("./logs/mmc_review_storage.log", "r") as file:
        lines = file.readlines()
    log_items = mmc_process_log(lines, "client_send")
    with open('./logs/processed/mmc_review_storage.log', 'w') as file:
        write_file(log_items, file)

    for i in range(5):
        with open("./logs/mmc_user_db_" + str(i) + ".log", "r") as file:
            lines = file.readlines()
        log_items = mmc_process_log(lines, "client_send")
        with open("./logs/processed/mmc_user_db_" + str(i) + ".log", 'w') as file:
            write_file(log_items, file)

    for i in range(5):
        with open("./logs/mmc_movie_db_" + str(i) + ".log", "r") as file:
            lines = file.readlines()
        log_items = mmc_process_log(lines, "client_send")
        with open("./logs/processed/mmc_movie_db_" + str(i) + ".log", 'w') as file:
            write_file(log_items, file)  

    with open("./logs/docker_mmc_review_storage.log", "r") as file:
        lines = file.readlines()
    log_items = mmc_process_log(lines, "docker_recv")
    with open('./logs/processed/docker_mmc_review_storage.log', 'w') as file:
        write_file(log_items, file)    
    
    for i in range(5):
        with open("./logs/docker_mmc_user_db_" + str(i) + ".log", "r") as file:
            lines = file.readlines()
        log_items = mmc_process_log(lines, "docker_recv")
        with open("./logs/processed/docker_mmc_user_db_" + str(i) + ".log", 'w') as file:
            write_file(log_items, file)

    for i in range(5):
        with open("./logs/docker_mmc_movie_db_" + str(i) + ".log", "r") as file:
            lines = file.readlines()
        log_items = mmc_process_log(lines, "docker_recv")
        with open("./logs/processed/docker_mmc_movie_db_" + str(i) + ".log", 'w') as file:
            write_file(log_items, file)       

    with open("./logs/mongo_review_storage.log", "r") as file:
        lines = file.readlines()
    log_items = mongo_process_log(lines, "client_send")
    with open('./logs/processed/mongo_review_storage.log', 'w') as file:
        write_file(log_items, file)

    for i in range(5):
        with open("./logs/mongo_user_db_" + str(i) + ".log", "r") as file:
            lines = file.readlines()
        log_items = mongo_process_log(lines, "client_send")
        with open("./logs/processed/mongo_user_db_" + str(i) + ".log", 'w') as file:
            write_file(log_items, file)

    for i in range(5):
        with open("./logs/mongo_movie_db_" + str(i) + ".log", "r") as file:
            lines = file.readlines()
        log_items = mongo_process_log(lines, "client_send")
        with open("./logs/processed/mongo_movie_db_" + str(i) + ".log", 'w') as file:
            write_file(log_items, file)  

    with open("./logs/docker_mongo_review_storage.log", "r") as file:
        lines = file.readlines()
    log_items = mongo_process_log(lines, "docker_recv")
    with open('./logs/processed/docker_mongo_review_storage.log', 'w') as file:
        write_file(log_items, file)    
    
    for i in range(5):
        with open("./logs/docker_mongo_user_db_" + str(i) + ".log", "r") as file:
            lines = file.readlines()
        log_items = mongo_process_log(lines, "docker_recv")
        with open("./logs/processed/docker_mongo_user_db_" + str(i) + ".log", 'w') as file:
            write_file(log_items, file)

    for i in range(5):
        with open("./logs/docker_mongo_movie_db_" + str(i) + ".log", "r") as file:
            lines = file.readlines()
        log_items = mongo_process_log(lines, "docker_recv")
        with open("./logs/processed/docker_mongo_movie_db_" + str(i) + ".log", 'w') as file:
            write_file(log_items, file)       




if __name__ == '__main__':
    main()


