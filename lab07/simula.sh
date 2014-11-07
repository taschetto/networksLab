#!/bin/bash

echo "Simulação TCP"

iperf -c 10.32.143.237 -i 1 -p 5001 -w 1K -t 30
iperf -c 10.32.143.237 -i 1 -p 5001 -w 4K -t 30
iperf -c 10.32.143.237 -i 1 -p 5001 -w 8K -t 30
iperf -c 10.32.143.237 -i 1 -p 5001 -w 32K -t 30
iperf -c 10.32.143.237 -i 1 -p 5001 -w 128K -t 30

echo "Simulação UDP"

iperf -c 10.32.143.237 -u -t 3 -p 5002 -b 50k -t 30
iperf -c 10.32.143.237 -u -t 3 -p 5002 -b 1M -t 30
iperf -c 10.32.143.237 -u -t 3 -p 5002 -b 5M -t 30
iperf -c 10.32.143.237 -u -t 3 -p 5002 -b 10M -t 30
iperf -c 10.32.143.237 -u -t 3 -p 5002 -b 50M -t 30

sudo tc qdisc del dev eth3 root
sudo tc qdisc add dev eth3 root netem delay 100ms 10ms 25%

echo "Simulação TCP com latência variável"

iperf -c 10.32.143.237 -i 1 -p 5001 -w 1K -t 30
iperf -c 10.32.143.237 -i 1 -p 5001 -w 4K -t 30
iperf -c 10.32.143.237 -i 1 -p 5001 -w 8K -t 30
iperf -c 10.32.143.237 -i 1 -p 5001 -w 32K -t 30
iperf -c 10.32.143.237 -i 1 -p 5001 -w 128K -t 30

echo "Simulação UDP com latência variável"

iperf -c 10.32.143.237 -u -t 3 -p 5002 -b 50k -t 30
iperf -c 10.32.143.237 -u -t 3 -p 5002 -b 1M -t 30
iperf -c 10.32.143.237 -u -t 3 -p 5002 -b 5M -t 30
iperf -c 10.32.143.237 -u -t 3 -p 5002 -b 10M -t 30
iperf -c 10.32.143.237 -u -t 3 -p 5002 -b 50M -t 30

sudo tc qdisc del dev eth3 root
sudo tc qdisc add dev eth0 root netem loss 3% 25%

echo "Simulação TCP com perda de pacotes"

iperf -c 10.32.143.237 -i 1 -p 5001 -w 1K -t 30
iperf -c 10.32.143.237 -i 1 -p 5001 -w 4K -t 30
iperf -c 10.32.143.237 -i 1 -p 5001 -w 8K -t 30
iperf -c 10.32.143.237 -i 1 -p 5001 -w 32K -t 30
iperf -c 10.32.143.237 -i 1 -p 5001 -w 128K -t 30

echo "Simulação UDP com perda de pacotes"

iperf -c 10.32.143.237 -u -t 3 -p 5002 -b 50k -t 30
iperf -c 10.32.143.237 -u -t 3 -p 5002 -b 1M -t 30
iperf -c 10.32.143.237 -u -t 3 -p 5002 -b 5M -t 30
iperf -c 10.32.143.237 -u -t 3 -p 5002 -b 10M -t 30
iperf -c 10.32.143.237 -u -t 3 -p 5002 -b 50M -t 30

sudo tc qdisc del dev eth3 root
