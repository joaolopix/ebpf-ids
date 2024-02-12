from bcc import BPF
import sys
import time
import socket
import ctypes
import rf_model as rf_model
from datetime import datetime, timedelta
import math

def usage():
    print(f"\nUsage: {sys.argv[0]} [XDP MODE] <ifdev> [ML MODEL MODE] [LISTEN MODE] [OPTIONAL PARAMETERS]")
    print("\nXDP MODE:")
    print("\t-S: use skb / generic mode")
    print("\t-D: use driver / native mode")
    print("\t-H: use hardware offload mode")
    print("ML MODEL MODE:")
    print("\t-C: C compiled model mode")
    print("\t-M: MAPs stored model mode")
    print("LISTEN MODE:")
    print("\t-Ul: Userspace mode showing Logs from Perf Output")
    print("\t-Up: Userspace mode showing Port Scan Table")
    print("\t-Uf: Userspace mode showing Flows")
    print("\t-K: Kernel mode showing bpf_trace_printk (used for debugging)")
    print("OPTIONAL:")
    print("\t--scan_attempts: Number of scan attempts to infer a Port Scan (Default: 25)")
    print("\t--scan_delay: Max Delay in seconds between scan attempts to infer Port Scan (Default: 600 sec)")
    print(f"\ne.g.: {sys.argv[0]} -S eth0 -C -Ul\n")

def check_input_argv():
    if len(sys.argv) == 5:
        return 1
    elif len(sys.argv) == 7:
        return 2
    elif len(sys.argv) == 9:
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
    
    return ml_mode,listen_mode,flags

def get_scan_options(check):
    sa,sd = 25,600*10**9
    if check == 2 or check == 3:
        if "--scan_attempts" == sys.argv[5]:
            try:
                sa = int(sys.argv[6])
            except (IndexError, ValueError) as e: 
                print("EBPF-IDS: SCAN ATTEMPTS PARAMETER IS NOT AN INTEGER")
                usage()
                return None
        elif "--scan_delay" == sys.argv[5]:
            try:
                sd = int(sys.argv[6])*10**9
            except (IndexError, ValueError) as e: 
                print("EBPF-IDS: SCAN DELAY PARAMETER IS NOT AN INTEGER")
                usage()
                return None
        else:
            print("EBPF-IDS: INVALID OPTIONAL PARAMETER")
            usage()
            return None
            
    if check == 3:
        if "--scan_attempts" == sys.argv[7]:
            try:
                sa = int(sys.argv[8])
            except (IndexError, ValueError) as e: 
                print("EBPF-IDS: SCAN ATTEMPTS PARAMETER IS NOT AN INTEGER")
                usage()
                return None
        elif "--scan_delay" == sys.argv[7]:
            try:
                sd = int(sys.argv[8])*10**9
            except (IndexError, ValueError) as e: 
                print("EBPF-IDS: SCAN DELAY PARAMETER IS NOT AN INTEGER")
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
    if "-K" == sys.argv[4]:
        return 0
    if "-Uf" == sys.argv[4]:
        return 1
    if "-Up" == sys.argv[4]:
        return 2
    if "-Ul" == sys.argv[4]:
        return 3
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

    def print_event_data(cpu, data, size):
        e = b["output"].event(data)

        def convert_ip_to_string(ip_integer):
            ip_integer = ip_integer & 0xFFFFFFFF
            return socket.inet_ntoa(ip_integer.to_bytes(4, byteorder='little'))

        src_ip = convert_ip_to_string(e.src_ip)
        dst_ip = convert_ip_to_string(e.dst_ip)
        current_datetime = datetime.fromtimestamp(time.time()-e.timestamp)
        dst_port = socket.ntohs(e.dst_port)
        portscan_type = ["Vertical","Horizontal","Block"]
        print("{} - ALERT: {} Port Scan detected from {} to {}:{}".format(current_datetime,portscan_type[e.type],src_ip,dst_ip,dst_port))
            
    def print_flowtable():
        print("\n\n" + "_"*123 + "\nTable size: "+ str(len(b["flow_table"])) + "\n" + "_"*123)
        print('{:<15s}:{:<6} ---> {:<15s}:{:<6} | {:<6} | {:<6} | {:<6} | {:<8} | {:<6} | {:<4} | {:<3} '
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
            scan_to_str = ['yes','no']
            scan_prob = math.floor((v.scan_counter/v.packet_counter)*100)
            
            print('{:<15s}:{:<6} ---> {:<15s}:{:<6} | {:<6} | {:<6} | {:<6} | {:<8} | {:<6} | {:<4} | {:<3} %'
                    .format(src_ip, src_port, dst_ip, dst_port, k.protocol, v.packet_counter,v.transmited_bytes,round(ts,4),fl,scan_to_str[int(v.scan)],scan_prob))

    def print_pstable():
        print("\n\n"+ "_"*50)
        print('{:<15s} | {:<6} | {:<15s}:{:<6}'.format("SRC IP", " PORT", "DST IP", "PORT"))
        for k,v in b["portscan_table"].items():
            src_ip = socket.inet_ntoa(k)
            print('{:<15s} '.format(src_ip),end = '')
            for i in range(v.index_counter):
                dst_ip = socket.inet_ntoa(v.scans[i].dst_ip.to_bytes(4, byteorder='little'))
                src_port = socket.ntohs(v.scans[i].src_port)
                dst_port = socket.ntohs(v.scans[i].dst_port)
                print('| {:<6} | {:<15s}:{:<6} \n{:<15s} '.format(src_port, dst_ip, dst_port," "),end='')
            print("\r"+"_"*50)


    if mode == 3:
        b["output"].open_perf_buffer(print_event_data)

    try:
        while True:
            if mode == 1:
                time.sleep(2) 
                print_flowtable()   
            elif mode == 2:
                time.sleep(2) 
                print_pstable() 
            elif mode == 3:
                b.perf_buffer_poll()
            else:
                b.trace_print()

    except KeyboardInterrupt:
        return
        
def main():

    check = check_input_argv()
    if check != 0:
        input_values = get_input_argv()
        if input_values is None:
            exit(1)
        ml_mode,listen_mode,flags = input_values
        scan_options = get_scan_options(check)
        if scan_options is None:
            exit(1)
        sa,sd = scan_options
    else:
        exit(1)

    print("EBPF-IDS: STARTED")

    device, offloaded_device = get_device_from_argv()

    mode = BPF.XDP
    ebpf_str = read_ebpf_file('ebpf.c')

    n_trees = len(rf_model.rf_tree_roots)
    n_nodes = len(rf_model.rf_nodes)
    mtd = rf_model.max_tree_depth

    b = BPF(text=ebpf_str, device=offloaded_device,cflags=[f"-DN_TREES={n_trees}",f"-DN_NODES={n_nodes}",
                                                           f"-DMAX_TREE_DEPTH={mtd}",f"-DMODEL_MODE={ml_mode}",
                                                           f"-DPS_THRESHOLD={sa}",f"-DPS_DELAY={sd}"])
    fn = b.load_func("ebpf_main", mode, device=offloaded_device)
    print("EBPF-IDS: CODE LOADED")

    if(ml_mode):
        set_model_map(b)
        print("EBPF-IDS: ML MODEL LOADED")

    b.attach_xdp(device, fn, flags)
    print("EBPF-IDS: ATTACHED\n")

    listen(b,listen_mode)

    print("\n\nEBPF-IDS: REMOVING FILTER FROM DEVICE")
    b.remove_xdp(device, flags)

if __name__ == '__main__':
    main()
