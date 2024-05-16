# eBPF-IDS using ML and XDP Offload

This repository contains the source code and other materials for an Intrusion Detection System developed using the extended Berkeley Packet Filter. It uses Machine Learning, namely a Random Forest algorithm, for detection. Additionally, the solution can use a partial hardware offload using the XDP Offload mode. This solution is designed solely for the detection of Port Scans.

This work was developed in the context of the Thesis related to Masters in Informatics Security, presented to the Department of Informatics Engineering of the Faculty of Sciences and Technology of the University of Coimbra.

## Contents
For the correct functionality of this solution, the following are required:
* A solution before XDP Offload was integrated, used for legacy purposes
* Dataset processing and Machine Learning Training
* The Source Code containing the final solution
* The same IDS capabilities implemented totally in User Space using the Scapy Library

## Requirements
For the correct functionality of this solution, the following are required:
* Kernel 5.15.0 (or above)
* Netronome SmartNIC
  * Agilio BPF Firmware
* CPUs: 4
* Python 3.10.12 (or above)
* BCC Library

## Usage

  ```sh
  Usage: ebpfids_userspace.py [XDP MODE] <ifdev> [ML MODEL MODE] [DETECTION RESPONSE] [OUTPUT MODE] [OPTIONAL PARAMETERS]

XDP MODE:
        -S: use skb / generic mode
        -D: use driver / native mode
        -H: use hardware offload mode
ML MODEL MODE:
        -C: C compiled model mode
        -M: MAPs stored model mode
DETECTION RESPONSE:
        -P: Passive mode
        -A: Active mode
OUTPUT MODE:
        -Lv: Logs from Perf Output (Verbose)
        -Ls: Logs from Perf Output (Simple)
        -Up: Userspace mode showing Port Scan Table
        -Uf: Userspace mode showing Flows
        -K: Kernel mode showing bpf_trace_printk (for debugging ebpf code)
OPTIONAL:
        --scan_attempts: Number of scan attempts to infer a Port Scan (Default: 25)
        --scan_delay: Max Delay in seconds between scan attempts to infer Port Scan (Default: 1800 sec)

e.g.: ebpfids_userspace.py -S eth0 -C -P -Lv
  ```
