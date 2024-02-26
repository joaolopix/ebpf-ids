from scapy.all import *
from scapy.layers.inet import IP, TCP,UDP
import signal
import time
import threading
from collections import OrderedDict
import rf_model
import queue
from datetime import datetime
 
class LRUCache:

    def __init__(self, capacity: int):
        self.cache = OrderedDict()
        self.capacity = capacity

    def len(self):
        return len(self.cache)
 
    def get(self, key):
        if key not in self.cache:
            return None
        else:
            self.cache.move_to_end(key)
            return self.cache[key]
 
    def put(self, key, value):
        self.cache[key] = value
        self.cache.move_to_end(key)
        if len(self.cache) > self.capacity:
            self.cache.popitem(last = False)

    def remove(self,key):
        del self.cache[key]
        self.capacity -= 1

class Pkt_Data:
    def __init__(self, src, dst, src_port,dst_port, l4_proto, pkt_len, flags):
        self.src_ip = src
        self.dst_ip = dst
        self.src_port = src_port
        self.dst_port = dst_port
        self.l4_proto = l4_proto
        self.pkt_len = pkt_len
        self.flags = flags

class Flow_Value:
    def __init__(self, packet_counter, timestamp, duration, transmited_bytes, flags, scan, scan_counter, suspicious):
        self.packet_counter = packet_counter
        self.timestamp = timestamp
        self.duration = duration
        self.transmitted_bytes = transmited_bytes
        self.flags = flags
        self.scan = scan
        self.scan_counter = scan_counter
        self.suspicious = suspicious

    def __iter__(self):
        yield self.packet_counter
        yield self.timestamp
        yield self.duration
        yield self.transmitted_bytes
        yield self.flags
        yield self.scan
        yield self.scan_counter
        yield self.suspicious

class Portscan_Value:
    def __init__(self, dst_ip,dst_port,counter,timestamp,ps_type,ps_method):
        self.dst_ip = dst_ip
        self.dst_port = dst_port
        self.counter = counter
        self.timestamp = timestamp
        self.ps_type = ps_type
        self.ps_method = ps_method
   
flow_table = LRUCache(500)
portscan_table = LRUCache(100)

INTERFACE_NAME = "enp0s8"  # Network Interface
INTERFACE_IP = "192.168.1.216" # IP Adress of the host
PS_THRESHOLD = 25
PS_DELAY = 1800

run = True
q = queue.Queue()
p = 0
t = 0

def parse_tcp(pkt):
    # get flow key
    src_ip = pkt[IP].src
    dst_ip = pkt[IP].dst
    src_port = pkt[IP].payload.sport
    dst_port = pkt[IP].payload.dport

    # Flags
    flags = pkt[TCP].flags
    URG = 1 #0x20
    ACK = 1 #0x10
    PSH = 1 #0x08
    RST = 1 #0x04
    SYN = 1 #0x02
    FIN = 1 #0x01

    if flags & 0x20:
        URG = 2
    if flags & 0x10:
        ACK = 2
    if flags & 0x08:
        PSH = 2
    if flags & 0x04:
        RST = 2
    if flags & 0x02:
        SYN = 2
    if flags & 0x01:
        FIN = 2

    used_flags = [URG,ACK,PSH,RST,SYN,FIN]

    packet = Pkt_Data(src_ip,dst_ip,src_port,dst_port,6,len(pkt),used_flags)
    return packet

def parse_udp(pkt):
    # get flow key
    src_ip = pkt[IP].src
    dst_ip = pkt[IP].dst
    src_port = pkt[IP].payload.sport
    dst_port = pkt[IP].payload.dport

    packet = Pkt_Data(src_ip,dst_ip,src_port,dst_port,17,len(pkt),[1,1,1,1,1,1])
    return packet

def prediction(duration,protocol,packet_counter,transmited_bytes,current_flags):
    features = [duration*10,protocol*10,packet_counter*10,transmited_bytes*10,current_flags*10]
    votes = [0,0,0,0,0]

    for i in range(len(rf_model.rf_tree_roots)):
        current_node = rf_model.rf_tree_roots[i]
        for depth in range(rf_model.max_tree_depth+1):
            node = rf_model.rf_nodes[current_node]
            if node.feature != -1:
                if features[node.feature] < node.value:
                    current_node = node.left
                else:
                    current_node = node.right
            else:
                votes[node.value] += 1
                break
    
    most_voted_class = -1
    most_voted_votes = 0
    for i in range(len(votes)):
        if votes[i] > most_voted_votes:
            most_voted_class = i
            most_voted_votes = votes[i]
    return most_voted_class

def flow_table_add(packet):

    flow_key = (packet.src_ip,packet.dst_ip,packet.src_port,packet.dst_port,packet.l4_proto)
    value = flow_table.get(flow_key)

    if value is not None:
        value.packet_counter += 1
        value.duration += time.time()-value.timestamp
        value.timestamp = time.time()
        value.transmitted_bytes += packet.pkt_len
        for i in range(len(packet.flags)):
            if packet.flags[i] == 2:
                value.flags[i] = 2

        flags = 0
        for i in packet.flags:
            flags += i
            flags = flags*10
        flags = int(flags/10)
        prev_scan = value.scan
        scan = prediction(value.duration,packet.l4_proto,value.packet_counter,value.transmitted_bytes,flags)

        if prev_scan != 0 and prev_scan != scan:
            value.suspicious = 1

        if value.suspicious == 2:
            value.suspicious = 0
        if value.scan_counter*100/value.packet_counter <= 5 and value.suspicious == 1:
            value.suspicious = 2

        if scan != 0:
            value.scan_counter += 1

        value.scan = scan   
        return flow_key,value     

    else:
        flow_value = Flow_Value(1,time.time(),0,packet.pkt_len,packet.flags,0,0,0)

        flags = 0
        for i in packet.flags:
            flags += i
            flags = flags*10
        flags = int(flags/10)

        scan = prediction(flow_value.duration,packet.l4_proto,flow_value.packet_counter,flow_value.transmitted_bytes,flags)
        if scan != 0:
            flow_value.scan_counter += 1

        flow_value.scan = scan  

        flow_table.put(flow_key,flow_value) 
        return flow_key,flow_value

def portscan_table_add(fk,fv):

    key = fk[0]
    value = portscan_table.get(key)

    if value is not None:
        
        if fv.timestamp - value.timestamp > PS_DELAY:
            value = Portscan_Value(fk[1],fk[3],1,fv.timestamp,fv.scan,0)
            return -2,value
        else:
            value.timestamp = fv.timestamp
            value.counter += 1
            if value.dst_ip != fk[1]:
                value.ps_method = 1
            if value.dst_port != fk[3] and value.ps_method == 1:
                value.ps_method = 2
            
            value.dst_ip = fk[1]
            value.dst_port = fk[3]
            value.ps_type = fv.scan

            if value.counter >= PS_THRESHOLD:
                return 1,value
            else:
                return 0,value

    else:
        ps_value = Portscan_Value(fk[1],fk[3],1,fv.timestamp,fv.scan,0)
        portscan_table.put(key,ps_value) 
        if ps_value.counter >= PS_THRESHOLD:
            return 1,ps_value
        else:
            return 0,ps_value

def portscan_table_remove(key):

    value = portscan_table.get(key)
    value.counter -= 1
    if value.counter == 0:
        portscan_table.remove(key)

    return -1,value

def packet_parser(pkt):
    if IP in pkt and hasattr(pkt[IP], 'payload'):
        if isinstance(pkt[IP].payload, TCP):
            packet = parse_tcp(pkt)
            return packet
        if isinstance(pkt[IP].payload, UDP):
            packet = parse_udp(pkt)
            return packet
    return None

def packet_handler(pkt):
    global p,t

    if t == 0:
        t = time.time()
    p += 1

    packet = packet_parser(pkt)
    if packet is not None:
        fk,fv = flow_table_add(packet)
        if fv.scan != 0:
            e,portscan = portscan_table_add(fk,fv)
            q.put((e,fk,portscan))

        elif fv.suspicious == 2:
            e,portscan = portscan_table_remove(fk[0])
            q.put((e,fk,portscan))

def print_flows():
    print("\n\n" + "_"*126 + "\nTable size: "+ str(flow_table.len()) + "\n" + "_"*126)
    print('{:<15s}:{:<6} ---> {:<15s}:{:<6} | {:<6} | {:<6} | {:<6} | {:<8} | {:<6} | {:<7} | {:<3} '
            .format("SRC IP", "PORT", "DST IP", "PORT", "PROTO", "PKTS","BYTES","DTIME","FLAGS","SCAN","SCAN PROBABILITY"))
    ftc_copy = flow_table.cache.copy()
    for flow_key, flow_value in ftc_copy.items():
        src_ip, dst_ip, src_port, dst_port, proto = flow_key
        packet_counter, timestamp, duration, transmited_bytes, flags, scan, scan_counter, suspicious = flow_value
        f = ['U','A','P','R','S','F']
        fl = ""
        for i,flag in enumerate(flags):
            if flag == 2:
                fl += f[i]
            else:
                fl += '.'
        scan_to_str = ['normal','udp','fin','syn','ack']
        print('{:<15}:{:<6} ---> {:<15}:{:<6} | {:<6} | {:<6} | {:<6} | {:<8} | {:<6} | {:<7} | {:<3} %'
                .format(src_ip, src_port, dst_ip, dst_port, proto, packet_counter,transmited_bytes,round(duration,4),fl,scan_to_str[scan],int((scan_counter/packet_counter)*100)))
    time.sleep(2)

def logs():
    global run
    stored_notifications = {}
    while run:
        value = q.get()
        if value is None:
            break
        e,fk,ps_value = value
        portscan_method = ["Vertical","Horizontal","Block"]
        portscan_type = ["","Udp","Fin","Syn","Ack"]
        src_ip = fk[0]
        dst_ip = fk[1]
        dst_port = fk[3]
        src_port = fk[2]
        current_datetime = datetime.fromtimestamp(time.time())

        if e == -2: # restart count
            del stored_notifications[src_ip][dst_ip]
            stored_notifications[src_ip] = {dst_ip: [(dst_port,current_datetime,src_port,ps_value.ps_type)]}

        if e == -1: # remove element from stored notifications
            if src_ip in stored_notifications.keys():
                for i in stored_notifications[src_ip][dst_ip]:
                    if i[0] == dst_port and i[2] == src_port:
                        stored_notifications[src_ip][dst_ip].remove(i)
                        break
                if len(stored_notifications[src_ip][dst_ip]) == 0:
                    del stored_notifications[src_ip][dst_ip]
                    if len(stored_notifications[src_ip]) == 0:
                        del stored_notifications[src_ip]

        elif e == 0: # store a notification
            if src_ip not in stored_notifications.keys():
                stored_notifications[src_ip] = {dst_ip: [(dst_port,current_datetime,src_port,ps_value.ps_type)]}
            else:
                if dst_ip not in stored_notifications[src_ip].keys():
                    stored_notifications[src_ip][dst_ip] = [(dst_port,current_datetime,src_port,ps_value.ps_type)]
                else:
                    stored_notifications[src_ip][dst_ip] += [(dst_port,current_datetime,src_port,ps_value.ps_type)]

        elif e== 1: # provide an alert
            if src_ip in stored_notifications.keys():
                for k,v in stored_notifications[src_ip].items():
                    for i in v:
                        print("{} - ALERT: {} {} Port Scan detected from {}:{} to {}:{}".format(i[1],portscan_method[ps_value.ps_method],portscan_type[i[3]],src_ip,i[2],k,i[0]))
                del stored_notifications[src_ip]
            # print all stored notifications and delete them as all subsequent notifactions are alerts
            print("{} - ALERT: {} {} Port Scan detected from {}:{} to {}:{}".format(current_datetime,portscan_method[ps_value.ps_method],portscan_type[ps_value.ps_type],src_ip,src_port,dst_ip,dst_port))
        
        #print_flows()

def main():
    print("U-IDS: STARTED")

    def sigint_handler(signum, frame):
        global run,p,t
        run = False
        q.put(None)
        t = time.time()-t
        print("\nU-IDS: CTRL^C CALLED")
        print("U-IDS: WAITING FOR LOG THREAD JOIN")
        log_thread.join()
        print("U-IDS: PACKET CAPTURE STOPPED")
        print("U-IDS: STATISTICS")
        print("\tprocessed packets: %d \n"
              "\telapsed time %d" % (p,t))
        print("\tpackets per second: %d" % (int(p/t)))

        exit(0)

    # Register the SIGINT handler
    signal.signal(signal.SIGINT, sigint_handler)

    print("U-IDS: LOGS THREAD STARTED")
    log_thread = threading.Thread(target=logs)
    log_thread.start()

    print("U-IDS: SNIFFING TRAFFIC\n")
    sniff(iface=INTERFACE_NAME, filter="ip dst " + INTERFACE_IP,prn=packet_handler,store=0)

if __name__ == "__main__":
    main()