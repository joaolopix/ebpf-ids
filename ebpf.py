from bcc import BPF
import sys
import time
import socket
import ctypes
import rf_model as rf_model

def usage():
    print(f"\nUsage: {sys.argv[0]} [XDP MODE] <ifdev> [ML MODEL MODE] [LISTEN MODE]")
    print("\nXDP MODE:")
    print("\t-S: use skb mode")
    print("\t-D: use driver mode")
    print("\t-H: use hardware offload mode")
    print("ML MODEL MODE:")
    print("\t-C: C compiled model mode")
    print("\t-M: MAPs stored model")
    print("LISTEN MODE:")
    print("\t-U: Userspace mode")
    print("\t-K: Kernel mode")
    print(f"\ne.g.: {sys.argv[0]} -S eth0 -C\n")

def check_input_args():
    if len(sys.argv) < 4:
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

def get_ml_model_mode():
    model = -1
    if "-C" == sys.argv[3]:
        return 0
    if "-M" == sys.argv[3]:
        return 1
    return model

def set_model_map(b):

    index = 0
    for node in rf_model.rf_nodes:
        b["tree_nodes"][ctypes.c_int(index)] = node
        index += 1
    index = 0
    for root in rf_model.rf_tree_roots:
        b["tree_roots"][ctypes.c_int(index)] = ctypes.c_int(root)
        index += 1

def get_listen_mode():
    mode = -1
    if "-K" == sys.argv[4]:
        return 0
    if "-U" == sys.argv[4]:
        return 1
    return mode

def listen(b,mode):

    try:
        while True:
            if mode: # Userspace mode
                time.sleep(2) 
                print("\n\nTable size: "+ str(len(b["flow_table"])) + "\n")
                print('{:<15s}:{:<6} ---> {:<15s}:{:<6} | {:<6} | {:<6} | {:<6} | {:<8} | {:<4} | {}'
                        .format("SRC IP", "PORT", "DST IP", "PORT", "PROTO", "PKTS","BYTES","DTIME","SCAN","FLAGS"))
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
                    
                    print('{:<15s}:{:<6} ---> {:<15s}:{:<6} | {:<6} | {:<6} | {:<6} | {:<8} | {:<4} | {}'
                            .format(src_ip, src_port, dst_ip, dst_port, k.protocol, v.packet_counter,v.transmited_bytes,round(ts,4),v.scan,fl))
            else:
                b.trace_print()


    except KeyboardInterrupt:
       return
        
def main():
    if check_input_args():
        exit(1)
    print("EBPF-IDS: STARTED")

    device, offloaded_device = get_device_from_argv()
    flags = set_flags()

    ml_mode = get_ml_model_mode() 
    if ml_mode == -1:
        print("EBPF-IDS: INVALID ML MODEL MODE"); exit(1)
    
    listen_mode = get_listen_mode() 
    if listen_mode == -1:
        print("EBPF-IDS: INVALID LISTEN MODE"); exit(1)
   
    mode = BPF.XDP
    ebpf_str = read_ebpf_file('ebpf.c')

    n_trees = len(rf_model.rf_tree_roots)
    n_nodes = len(rf_model.rf_nodes)
    mtd = rf_model.max_tree_depth

    b = BPF(text=ebpf_str, device=offloaded_device,cflags=[f"-DN_TREES={n_trees}",f"-DN_NODES={n_nodes}",f"-DMAX_TREE_DEPTH={mtd}",f"-DMODEL_MODE={ml_mode}"])
    fn = b.load_func("ebpf_main", mode, device=offloaded_device)
    print("EBPF-IDS: CODE LOADED")

    if(ml_mode):
        set_model_map(b)
        print("EBPF-IDS: ML MODEL LOADED")

    b.attach_xdp(device, fn, flags)
    print("EBPF-IDS: ATTACHED")

    listen(b,listen_mode)

    print("\nEBPF-IDS: REMOVING FILTER FROM DEVICE")
    b.remove_xdp(device, flags)

if __name__ == '__main__':
    main()
