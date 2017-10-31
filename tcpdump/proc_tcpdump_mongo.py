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


def main():
    with open("./logs/mongo_review_storage.log", "r") as file:
        lines = file.readlines()

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
                log_items.append(log_item)

    counter = 0
    for log_item in log_items:
        print(counter, 'req_id =', log_item.req_id, 'timestamp =', log_item.timestamp)
        counter += 1

if __name__ == '__main__':
    main()


