from bcc import BPF
import sys
import time
import socket
import ctypes
import rf_model as rf_model
from datetime import datetime
import math
import sys
import queue
import threading

def usage():
    print(f"\nUsage: {sys.argv[0]} [XDP MODE] <ifdev> [ML MODEL MODE] [DETECTION RESPONSE] [OUTPUT MODE] [OPTIONAL PARAMETERS]")
    print("\nXDP MODE:")
    print("\t-S: use skb / generic mode")
    print("\t-D: use driver / native mode")
    print("\t-H: use hardware offload mode")
    print("ML MODEL MODE:")
    print("\t-C: C compiled model mode")
    print("\t-M: MAPs stored model mode")
    print("DETECTION RESPONSE:")
    print("\t-P: Passive mode")
    print("\t-A: Active mode")
    print("OUTPUT MODE:")
    print("\t-Lv: Logs from Perf Output (Verbose)")
    print("\t-Ls: Logs from Perf Output (Simple)")
    print("\t-Up: Userspace mode showing Port Scan Table")
    print("\t-Uf: Userspace mode showing Flows")
    print("\t-K: Kernel mode showing bpf_trace_printk (for debugging ebpf)")
    print("OPTIONAL:")
    print("\t--scan_attempts: Number of scan attempts to infer a Port Scan (Default: 25)")
    print("\t--scan_delay: Max Delay in seconds between scan attempts to infer Port Scan (Default: 1800 sec)")
    print(f"\ne.g.: {sys.argv[0]} -S eth0 -C -Lv\n")

def check_input_argv():
    if len(sys.argv) == 6:
        return 1
    elif len(sys.argv) == 8:
        return 2
    elif len(sys.argv) == 10:
        return 3
    usage()
    return 0

def get_input_argv():

    ml_mode = get_ml_model_mode() 
    if ml_mode == -1:
        print("EBPF-IDS | ERROR : INVALID ML MODEL MODE")
        usage()
        return None
    listen_mode = get_listen_mode() 
    if listen_mode == -1:
        print("EBPF-IDS: INVALID LISTEN MODE")
        usage()
        return None
    flags = set_flags()
    if flags == 0:
        print("EBPF-IDS: INVALID XDP MODE")
        usage()
        return None
    detection_mode = get_detection_mode()
    if detection_mode == -1:
        print("EBPF-IDS: INVALID DETECTION MODE")
        usage()
        return None
    
    return ml_mode,listen_mode,flags,detection_mode

def get_detection_mode():
    d = -1
    if "-P" == sys.argv[4]:
        return 0
    if "-A" == sys.argv[4]:
        return 1
    return d

def get_scan_options(check):
    sa,sd = 25,1800*10**9 # default values

    if check == 2 or check == 3:
        if "--scan_attempts" == sys.argv[6]:
            try:
                sa = int(sys.argv[7])
                if sa <= 0:
                    raise ValueError
            except (IndexError, ValueError) as e: 
                print("EBPF-IDS: SCAN ATTEMPTS PARAMETER MUST BE A POSITIVE INTEGER")
                usage()
                return None
        elif "--scan_delay" == sys.argv[6]:
            try:
                sd = int(sys.argv[7])*10**9
                if sd <= 0:
                    raise ValueError
            except (IndexError, ValueError) as e: 
                print("EBPF-IDS: SCAN DELAY PARAMETER MUST BE A POSITIVE INTEGER")
                usage()
                return None
        else:
            print("EBPF-IDS: INVALID OPTIONAL PARAMETER")
            usage()
            return None
            
    if check == 3:
        if "--scan_attempts" == sys.argv[8]:
            try:
                sa = int(sys.argv[9])
                if sa <= 0:
                    raise ValueError
            except (IndexError, ValueError) as e: 
                print("EBPF-IDS: SCAN ATTEMPTS PARAMETER MUST BE A POSITIVE INTEGER")
                usage()
                return None
        elif "--scan_delay" == sys.argv[8]:
            try:
                sd = int(sys.argv[9])*10**9
                if sd <= 0:
                    raise ValueError
            except (IndexError, ValueError) as e: 
                print("EBPF-IDS: SCAN DELAY PARAMETER MUST BE A POSITIVE INTEGER")
                usage()
                return None
        else:
            print("EBPF-IDS: INVALID OPTIONAL PARAMETER")
            usage()
            return None
            
    return sa,sd

def get_device_from_argv():
    device = sys.argv[2]
    offloaded_device = ctypes.c_char_p(device.encode('utf-8')) if "-H" == sys.argv[1] else None
    return device, offloaded_device # not sure about this, i cant test it

def get_ml_model_mode():
    model = -1
    if "-C" == sys.argv[3]:
        return 0
    if "-M" == sys.argv[3]:
        return 1
    return model

def get_listen_mode():
    mode = -1
    if "-K" == sys.argv[5]:
        return 0
    if "-Uf" == sys.argv[5]:
        return 1
    if "-Up" == sys.argv[5]:
        return 2
    if "-Lv" == sys.argv[5]:
        return 3
    if "-Ls" == sys.argv[5]:
        return 4
    return mode

def read_ebpf_file(src):
    with open(src, 'r') as ebpfile:
        return ebpfile.read()

def set_flags():
    flags = 0
    if "-S" == sys.argv[1]:
        flags |= BPF.XDP_FLAGS_SKB_MODE
    if "-D" == sys.argv[1]:
        flags |= BPF.XDP_FLAGS_DRV_MODE
    if "-H" == sys.argv[1]:
        flags |= BPF.XDP_FLAGS_HW_MODE
    return flags

def set_model_map(b):

    index = 0
    for node in rf_model.rf_nodes:
        b["tree_nodes"][ctypes.c_int(index)] = node
        index += 1
    index = 0
    for root in rf_model.rf_tree_roots:
        b["tree_roots"][ctypes.c_int(index)] = ctypes.c_int(root)
        index += 1

def listen(b,mode):

    def queue_event_data():
        while run:
            event = q.get()
            if event is None:
                return
            print_queued_event_data(event)
    
    def print_queued_event_data(event):
        d = {event["src_ip"]:{  event["dst_ip"]:{   "ports": [event["dst_port"]],
                                                    "start": event["current_datetime"],
                                                    "last": event["current_datetime"],
                                                    "scan_type":[event["portscan_type"]]
                                                },
                                "scan_method":event["portscan_method"],
                                "alert_time":event["current_datetime"]
                            }
            }
        time.sleep(2) # give queue time to fill up
        start_time = time.time()
        while time.time()-start_time <= 2: # read queue for a max of 2 seconds 
            try:
                item = q.get(timeout=0)
                if item is None:
                    break
                if item["src_ip"] not in d.keys():
                    value = {item["dst_ip"]:{   "ports": [item["dst_port"]],
                                                "start": item["current_datetime"],
                                                "last": item["current_datetime"],
                                                "scan_type":[item["portscan_type"]]
                                                },
                                "scan_method":item["portscan_method"],
                                "alert_time":item["current_datetime"]}
                    d[item["src_ip"]] = value
                else:
                    if item["dst_ip"] not in d[item["src_ip"]].keys():
                        value = {   "ports": [item["dst_port"]],
                                    "start": item["current_datetime"],
                                    "last": item["current_datetime"],
                                    "scan_type":[item["portscan_type"]]
                                }
                        d[item["src_ip"]][item["dst_ip"]] = value               
                    else:                     
                        d[item["src_ip"]][item["dst_ip"]]["ports"]+=[item["dst_port"]]
                        d[item["src_ip"]][item["dst_ip"]]["last"]= item["current_datetime"]
                        if item["portscan_type"] not in d[item["src_ip"]][item["dst_ip"]]["scan_type"]:
                            d[item["src_ip"]][item["dst_ip"]]["scan_type"]+=[item["portscan_type"]]

            except queue.Empty:
                for k,v in d.items():
                    print("{} - ALERT . Port Scan detected from {}".format(v["alert_time"],k))
                    print("{:<34s} |___ Scan Method: {}".format(" ",v["scan_method"]))
                    for j,i in v.items():
                        if j not in ["scan_method", "alert_time"]:
                            print("{:<34s} | |___Target IP: {}".format(" ",j))
                            print("{:<34s} | | |___ Scan Type: {}".format(" ",i["scan_type"]))
                            port_count = len(i["ports"])
                            ports = [min(i["ports"]),max(i["ports"])]
                            if port_count == 1 or ports[0] == ports[1]:
                                print("{:<34s} | | |___ Scan Attempts: {}".format(" ",port_count))
                                print("{:<34s} | | |___ Target Port: {}".format(" ",ports[0]))
                            else:
                                print("{:<34s} | | |___ Scan Attempts: {}".format(" ",port_count))
                                print("{:<34s} | | |___ Target Ports: [{}-{}]".format(" ",ports[0],ports[1]))
                            print("{:<34s} | | |___ Duration Time: {}".format(" ",i["last"]-i["start"]))
                break

    def print_event_data(cpu, data, size):

        class Event(ctypes.Structure):
            _fields_ = [("type", ctypes.c_int),
                ("ps_method", ctypes.c_int),
                ("ps_type", ctypes.c_int),
                ("src_ip", ctypes.c_uint32),
                ("dst_ip", ctypes.c_uint32),
                ("dst_port", ctypes.c_uint16),
                ("src_port", ctypes.c_uint16)]

        e = ctypes.cast(data,ctypes.POINTER(Event)).contents

        src_ip = socket.inet_ntoa(e.src_ip.to_bytes(4, byteorder='little'))
        dst_ip = socket.inet_ntoa(e.dst_ip.to_bytes(4, byteorder='little'))
        current_datetime = datetime.fromtimestamp(time.time())
        dst_port = socket.ntohs(e.dst_port)
        src_port = socket.ntohs(e.src_port)
        portscan_method = ["Vertical","Horizontal","Block"]
        portscan_type = ["","Udp","Fin","Syn","Ack"]

        if e.type == -2: # restart count
            del stored_notifications[src_ip][dst_ip]
            stored_notifications[src_ip] = {dst_ip: [(dst_port,current_datetime,src_port,e.ps_type)]}

        if e.type == -1: # remove element from stored notifications
            if src_ip in stored_notifications.keys():
                for i in stored_notifications[src_ip][dst_ip]:
                    if i[0] == dst_port and i[2] == src_port:
                        stored_notifications[src_ip][dst_ip].remove(i)
                        break
                if len(stored_notifications[src_ip][dst_ip]) == 0:
                    del stored_notifications[src_ip][dst_ip]
                    if len(stored_notifications[src_ip]) == 0:
                        del stored_notifications[src_ip]

        elif e.type == 0: # store a notification
            if src_ip not in stored_notifications.keys():
                stored_notifications[src_ip] = {dst_ip: [(dst_port,current_datetime,src_port,e.ps_type)]}
            else:
                if dst_ip not in stored_notifications[src_ip].keys():
                    stored_notifications[src_ip][dst_ip] = [(dst_port,current_datetime,src_port,e.ps_type)]
                else:
                    stored_notifications[src_ip][dst_ip] += [(dst_port,current_datetime,src_port,e.ps_type)]

        elif e.type == 1: # provide an alert
            if mode == 3: # verbose
                if src_ip in stored_notifications.keys():
                    for k,v in stored_notifications[src_ip].items():
                        for i in v:
                            print("{} - ALERT: {} {} Port Scan detected from {}:{} to {}:{}".format(i[1],portscan_method[e.ps_method],portscan_type[i[3]],src_ip,i[2],k,i[0]))
                    del stored_notifications[src_ip]
                # print all stored notifications and delete them as all subsequent notifactions are alerts
                print("{} - ALERT: {} {} Port Scan detected from {}:{} to {}:{}".format(current_datetime,portscan_method[e.ps_method],portscan_type[e.ps_type],src_ip,src_port,dst_ip,dst_port))
            elif mode == 4: # simple
                if src_ip in stored_notifications.keys():
                    for k,v in stored_notifications[src_ip].items():
                        for i in v:
                            event = {"src_ip":src_ip,
                                    "dst_ip":k, 
                                    "current_datetime":i[1],
                                    "dst_port":i[0], 
                                    "portscan_method":portscan_method[e.ps_method],
                                    "portscan_type":portscan_type[i[3]]}
                            q.put(event)
                    del stored_notifications[src_ip]
                # queue all stored notifications and delete them as all subsequent notifactions are alerts
                event = {"src_ip":src_ip,
                        "dst_ip":dst_ip, 
                        "current_datetime":current_datetime,
                        "dst_port":dst_port, 
                        "portscan_method":portscan_method[e.ps_method],
                        "portscan_type":portscan_type[e.ps_type]}
                q.put(event)    
           
    def print_flowtable():
        print("\n\n" + "_"*126 + "\nTable size: "+ str(len(b["flow_table"])) + "\n" + "_"*126)
        print('{:<15s}:{:<6} ---> {:<15s}:{:<6} | {:<6} | {:<6} | {:<6} | {:<8} | {:<6} | {:<7} | {:<3} '
                .format("SRC IP", "PORT", "DST IP", "PORT", "PROTO", "PKTS","BYTES","DTIME","FLAGS","SCAN","SCAN PROBABILITY"))
        for k,v in b["flow_table"].items(): 
            src_ip = socket.inet_ntoa(k.src_ip.to_bytes(4, byteorder='little'))
            dst_ip = socket.inet_ntoa(k.dst_ip.to_bytes(4, byteorder='little'))
            src_port = socket.ntohs(k.src_port)
            dst_port = socket.ntohs(k.dst_port)
            ts = v.duration/1e9
            f = ['U','A','P','R','S','F']
            fl = ""
            for i,flag in enumerate(v.flags):
                if flag == 2:
                    fl += f[i]
                else:
                    fl += '.'
            #scan_to_str = ['yes','no']
            scan_to_str = ['normal','udp','fin','syn','ack']
            scan_prob = math.floor((v.scan_counter/v.packet_counter)*100)
            print('{:<15s}:{:<6} ---> {:<15s}:{:<6} | {:<6} | {:<6} | {:<6} | {:<8} | {:<6} | {:<7} | {:<3} %'
                    .format(src_ip, src_port, dst_ip, dst_port, k.protocol, v.packet_counter,v.transmited_bytes,round(ts,4),fl,scan_to_str[int(v.scan)],scan_prob))

    def print_pstable():
        print("\n\n"+ "_"*52)
        print('{:<15s} | {:<6} | {:<15s}:{:<6}'.format("SRC IP", "SCANS", "LAST DST IP", "LAST PORT"))
        for k,v in b["portscan_table"].items():
            src_ip = socket.inet_ntoa(k)
            dst_ip = socket.inet_ntoa(v.dst_ip.to_bytes(4, byteorder='little'))
            dst_port = socket.ntohs(v.dst_port)
            print('{:<15s} | {:<6} | {:<15s}:{:<6}'.format(src_ip, v.ps_counter, dst_ip, dst_port))

    def perf_output_error(value):
        q_lost.put(value)
        b["event_flag"][ctypes.c_int(0)] = ctypes.c_int(0) # events cant be generated
    
    def queue_lost_data():
        while run:
            value = q_lost.get()
            if value is None:
                return
            print_queued_error_data(value)

    def print_queued_error_data(value):
        time.sleep(3) # give queue time to fill up and stored events to be printed
        total = value
        while True:
            try:
                value = q_lost.get(timeout=0)
                if value is None:
                    break
                total += value
            except queue.Empty:
                print("\nEBPF-IDS: ERROR - PERF OUTPUT AS REACHED MAXIMUM CAPACITY, %d POSSIBILITY LOST SAMPLES, STOPPED EVENTS SUBMISSION" % (total))
                break
        time.sleep(2)
        print("EBPF-IDS: RESTARTING PERF OUTPUT\n")
        time.sleep(1)
        b["event_flag"][ctypes.c_int(0)] = ctypes.c_int(1) # events can be generated
    
    if mode == 3 or mode == 4:
        b["output"].open_perf_buffer(print_event_data,lost_cb=perf_output_error)
        stored_notifications = {}
        run = True
        q_lost = queue.Queue()
        lost_sample_thread = threading.Thread(target=queue_lost_data)
        lost_sample_thread.start()
        if mode == 4:
            q = queue.Queue()
            q_thread = threading.Thread(target=queue_event_data)
            q_thread.start()   
    
    try:
        while True:
            if mode == 1:
                time.sleep(2) 
                print_flowtable()   
            elif mode == 2:
                time.sleep(2) 
                print_pstable() 
            elif mode == 3 or mode == 4:
                b.perf_buffer_poll()
            else:
                b.trace_print()

    except KeyboardInterrupt:
        if mode == 3 or mode == 4:
            run = False
            q_lost.put(None)
            lost_sample_thread.join()
            if mode == 4:
                q.put(None)
                q_lost.put(None)
                q_thread.join()
        return
        
def main():

    check = check_input_argv()
    if check != 0:
        input_values = get_input_argv()
        if input_values is None:
            return
        ml_mode,listen_mode,flags,detection_mode = input_values
        scan_options = get_scan_options(check)
        if scan_options is None:
            return
        sa,sd = scan_options
    else:
        return

    print("EBPF-IDS: STARTED")

    device, offloaded_device = get_device_from_argv()

    mode = BPF.XDP
    ebpf_str = read_ebpf_file('ebpf.c')

    n_trees = len(rf_model.rf_tree_roots)
    n_nodes = len(rf_model.rf_nodes)
    mtd = rf_model.max_tree_depth

    b = BPF(text=ebpf_str, device=offloaded_device,cflags=[f"-DN_TREES={n_trees}",f"-DN_NODES={n_nodes}",
                                                           f"-DMAX_TREE_DEPTH={mtd}",f"-DMODEL_MODE={ml_mode}",
                                                           f"-DPS_THRESHOLD={sa}",f"-DPS_DELAY={sd}",
                                                           f"-DDETECTION_MODE={detection_mode}"])
    fn = b.load_func("ebpf_main", mode, device=offloaded_device)
    print("EBPF-IDS: CODE LOADED")

    if(ml_mode):
        set_model_map(b)
        print("EBPF-IDS: ML MODEL LOADED")

    b["event_flag"][ctypes.c_int(0)] = ctypes.c_int(1) # events can be generated

    b.attach_xdp(device, fn, flags)
    print("EBPF-IDS: ATTACHED\n")

    listen(b,listen_mode)

    print("\n\nEBPF-IDS: REMOVING FILTER FROM DEVICE")
    b.remove_xdp(device, flags)

if __name__ == '__main__':
    main()
