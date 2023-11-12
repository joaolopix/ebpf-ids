from bcc import BPF
import sys
import time
import socket
import datetime
import ctypes
import rf_model as rf_model

def usage():
    print(f"Usage: {sys.argv[0]} -S <ifdev>")
    print("-S: use skb mode")
    print("-D: use driver mode")
    print("-H: use hardware offload mode")
    print(f"e.g.: {sys.argv[0]} -S eth0")

def check_input_args():
    if len(sys.argv) < 3:
        usage()
        return 1
    return 0

def get_device_from_argv():
    device = sys.argv[2]
    offloaded_device = ctypes.c_char_p(device.encode('utf-8')) if "-H" == sys.argv[1] else None
    return device, offloaded_device # not sure about this, i cant test it

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



def main():
    if check_input_args():
        exit(1)
    print("EBPF-IDS: STARTED")

    device, offloaded_device = get_device_from_argv()
    flags = set_flags()
   
    mode = BPF.XDP
    ebpf_str = read_ebpf_file('ebpf.c')

    n_trees = len(rf_model.rf_tree_roots)
    n_nodes = len(rf_model.rf_nodes)
    mtd = rf_model.max_tree_depth

    
    b = BPF(text=ebpf_str, device=offloaded_device,cflags=[f"-DN_TREES={n_trees}",f"-DN_NODES={n_nodes}",f"-DMTD={mtd}"])
    fn = b.load_func("ebpf_main", mode, device=offloaded_device)
    print("EBPF-IDS: CODE LOADED")

    set_model_map(b)
    print("EBPF-IDS: ML MODEL LOADED")

    b.attach_xdp(device, fn, flags)
    print("EBPF-IDS: ATTACHED")

    try:
        while True:
            time.sleep(2) 
            print("\n\nTable size: "+ str(len(b["flow_table"])) + "\n")
            for k,v in b["flow_table"].items(): 

                src_ip = socket.inet_ntoa(k.src_ip.to_bytes(4, byteorder='little'))
                dst_ip = socket.inet_ntoa(k.dst_ip.to_bytes(4, byteorder='little'))
                src_port = socket.ntohs(k.src_port)
                dst_port = socket.ntohs(k.dst_port)
                #ts = datetime.datetime.fromtimestamp(v.timestamp/1e9)
                ts = v.duration/1e9
                
                print('{:<15s}:{:<6} ---> {:<15s}:{:<6} | {:<6} | Cont: {:<6} | Bytes: {:<6} | Duration: {} | Scan: {}'
                      .format(src_ip, src_port, dst_ip, dst_port, k.protocol, v.packet_counter,v.transmited_bytes,round(ts,4),v.scan))
            #b.trace_print()


    except KeyboardInterrupt:
        print("\nRemoving filter from device")
        b.remove_xdp(device, flags)

if __name__ == '__main__':
    main()
