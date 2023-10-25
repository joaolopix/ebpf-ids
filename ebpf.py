from bcc import BPF
import sys
import time
import socket
import datetime

def usage():
    print(f"Usage: {sys.argv[0]} [-S] <ifdev>")
    print("-S: use skb mode")
    print("-D: use driver mode")
    print("-H: use hardware offload mode")
    print(f"e.g.: {sys.argv[0]} [-S] eth0")

def check_input_args():
    if len(sys.argv) != 3:
        usage()
        return 1
    return 0

def get_device_from_argv():
    device = sys.argv[2]
    offloaded_device = device if "-H" in sys.argv else None
    return device, offloaded_device

def read_ebpf_file(src):
    with open(src, 'r') as ebpfile:
        return ebpfile.read()

def set_flags():
    flags = 0
    if "-S" in sys.argv:
        flags |= BPF.XDP_FLAGS_SKB_MODE
    if "-D" in sys.argv:
        flags |= BPF.XDP_FLAGS_DRV_MODE
    if "-H" in sys.argv:
        flags |= BPF.XDP_FLAGS_HW_MODE
    return flags

def main():
    if check_input_args():
        exit(1)
    print("EBPF-IDS: STARTED")

    device, offloaded_device = get_device_from_argv()
    flags = set_flags()
   
    mode = BPF.XDP
    ebpf_str = read_ebpf_file('ebpf.c')

    b = BPF(text=ebpf_str, device=offloaded_device)
    fn = b.load_func("flow_counter", mode, offloaded_device)
    b.attach_xdp(device, fn, flags)
    print("EBPF-IDS: LOADED AND ATTACHED")

    try:
        while True:
            time.sleep(2) 
            print("\n\nTable size: "+ str(len(b["flow_table"])) + "\n")
            for k,v in b["flow_table"].items(): 
                src_ip = socket.inet_ntoa(k.src_ip.to_bytes(4, byteorder='little'))
                dst_ip = socket.inet_ntoa(k.dst_ip.to_bytes(4, byteorder='little'))
                src_port = socket.ntohs(k.src_port)
                dst_port = socket.ntohs(k.dst_port)
                ts = datetime.datetime.fromtimestamp(v.timestamp/1e9)
                print('{:<15s}:{:<6} ---> {:<15s}:{:<6} | {:<6} | Cont: {:<6} | {}'.format(src_ip, src_port, dst_ip, dst_port, k.protocol, v.count, ts))

    except KeyboardInterrupt:
        print("\nRemoving filter from device")
        b.remove_xdp(device, flags)

if __name__ == '__main__':
    main()
