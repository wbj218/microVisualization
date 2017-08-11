import json
import operator


with open('../logs/Client_0.log', 'r') as file:
    client_json = json.load(file)

# with open('../logs/Client_1.log', 'r') as file:
#     client_json.update(json.load(file))

# with open('../logs/Client_2.log', 'r') as file:
#     client_json.update(json.load(file))

# with open('../logs/Client_3.log', 'r') as file:
#     client_json.update(json.load(file))

# with open('../logs/Client_4.log', 'r') as file:
#     client_json.update(json.load(file))

# with open('../logs/Client_5.log', 'r') as file:
#     client_json.update(json.load(file))

# with open('../logs/Client_6.log', 'r') as file:
#     client_json.update(json.load(file))

# with open('../logs/Client_7.log', 'r') as file:
#     client_json.update(json.load(file))




with open('../logs/UserAccount.log', 'r') as file:
    account_json = json.load(file)




total_latency = {}
ngx_latency = {}
account_latency = {}
streaming_latency = {}




for key, value in client_json.items():    
    ngx_latency[key] = account_json[key]["UserAccount"]["if_purchased"]["begin"] - client_json[key]["begin"]

for key, value in account_json.items():    
    account_latency[key] = account_json[key]["UserAccount"]["if_purchased"]["end"] -  account_json[key]["UserAccount"]["if_purchased"]["begin"]

for key, value in client_json.items():    
    streaming_latency[key] = client_json[key]["end"] - account_json[key]["UserAccount"]["if_purchased"]["end"]

for key, value in client_json.items():    
    total_latency[key] = client_json[key]["end"] - client_json[key]["begin"]




avg_total_latency = 0
avg_ngx_latency = 0
avg_account_latency = 0
avg_streaming_latency = 0


for key, value in total_latency.items():
    avg_total_latency += value

for key, value in ngx_latency.items():
    avg_ngx_latency += value

for key, value in account_latency.items():
    avg_account_latency += value

for key, value in streaming_latency.items():
    avg_streaming_latency += value



print ("n_reqs = ", len(total_latency))

print (avg_ngx_latency / len(ngx_latency))
print (avg_account_latency / len(account_latency))

print (avg_streaming_latency / len(streaming_latency))

print (avg_total_latency / len(total_latency))
