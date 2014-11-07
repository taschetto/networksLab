
#Servidor
IP 10.32.143.237
Porta TCP 5001
Porta UDP 5002

#Cliente
IP 10.32.143.169

# Link Padrão
##TCP
throughput variando janela TCP
The Maximum bandwidth can be calculated as follows:

\mathrm{Throughput} \le \frac {\mathrm{RWIN}} {\mathrm{RTT}} \,\!

where RWIN is the TCP Receive Window and RTT is the round-trip time for the path. The Max TCP Window size in the absence of TCP window scale option is 65,535 bytes.Example: Max Bandwidth = 65535 bytes / 0.220 s = 297886.36 B/s * 8 = 2.383 Mbit/s. Over a single TCP connection between those endpoints, the tested bandwidth will be restricted to 2.376 Mbit/s even if the contracted bandwidth is greater.
###1Kbytes
```
labredes@labredes-OptiPlex-3010 t2 (master) $ iperf -c 10.32.143.237 -i 1 -p 5001 -w 1K -t 30
WARNING: TCP window size set to 1024 bytes. A small window size
will give poor performance. See the Iperf documentation.
------------------------------------------------------------
Client connecting to 10.32.143.237, TCP port 5001
TCP window size: 4.50 KByte (WARNING: requested 1.00 KByte)
------------------------------------------------------------
[  3] local 10.32.143.169 port 40894 connected with 10.32.143.237 port 5001
[ ID] Interval       Transfer     Bandwidth
[  3]  0.0- 1.0 sec  2.62 MBytes  22.0 Mbits/sec
[  3]  1.0- 2.0 sec  2.75 MBytes  23.1 Mbits/sec
[  3]  2.0- 3.0 sec  2.75 MBytes  23.1 Mbits/sec
[  3]  3.0- 4.0 sec  2.75 MBytes  23.1 Mbits/sec
[  3]  4.0- 5.0 sec  2.75 MBytes  23.1 Mbits/sec
[  3]  5.0- 6.0 sec  2.62 MBytes  22.0 Mbits/sec
[  3]  6.0- 7.0 sec  2.75 MBytes  23.1 Mbits/sec
[  3]  7.0- 8.0 sec  2.75 MBytes  23.1 Mbits/sec
[  3]  8.0- 9.0 sec  2.75 MBytes  23.1 Mbits/sec
[  3]  9.0-10.0 sec  2.75 MBytes  23.1 Mbits/sec
[  3] 10.0-11.0 sec  2.88 MBytes  24.1 Mbits/sec
[  3] 11.0-12.0 sec  2.75 MBytes  23.1 Mbits/sec
[  3] 12.0-13.0 sec  2.75 MBytes  23.1 Mbits/sec
[  3] 13.0-14.0 sec  2.88 MBytes  24.1 Mbits/sec
[  3] 14.0-15.0 sec  2.75 MBytes  23.1 Mbits/sec
[  3] 15.0-16.0 sec  2.75 MBytes  23.1 Mbits/sec
[  3] 16.0-17.0 sec  2.75 MBytes  23.1 Mbits/sec
[  3] 17.0-18.0 sec  2.75 MBytes  23.1 Mbits/sec
[  3] 18.0-19.0 sec  2.75 MBytes  23.1 Mbits/sec
[  3] 19.0-20.0 sec  2.62 MBytes  22.0 Mbits/sec
[  3] 20.0-21.0 sec  2.75 MBytes  23.1 Mbits/sec
[  3] 21.0-22.0 sec  2.75 MBytes  23.1 Mbits/sec
[  3] 22.0-23.0 sec  2.75 MBytes  23.1 Mbits/sec
[  3] 23.0-24.0 sec  2.75 MBytes  23.1 Mbits/sec
[  3] 24.0-25.0 sec  2.75 MBytes  23.1 Mbits/sec
[  3] 25.0-26.0 sec  2.75 MBytes  23.1 Mbits/sec
[  3] 26.0-27.0 sec  2.75 MBytes  23.1 Mbits/sec
[  3] 27.0-28.0 sec  2.75 MBytes  23.1 Mbits/sec
[  3] 28.0-29.0 sec  2.75 MBytes  23.1 Mbits/sec
[  3] 29.0-30.0 sec  2.75 MBytes  23.1 Mbits/sec
[  3]  0.0-30.1 sec  82.5 MBytes  23.0 Mbits/sec

```
###4Kbytes,
```
labredes@labredes-OptiPlex-3010 t2 (master) $ iperf -c 10.32.143.237 -i 1 -p 5001 -w 4K -t 30
------------------------------------------------------------
Client connecting to 10.32.143.237, TCP port 5001
TCP window size: 8.00 KByte (WARNING: requested 4.00 KByte)
------------------------------------------------------------
[  3] local 10.32.143.169 port 40899 connected with 10.32.143.237 port 5001
[ ID] Interval       Transfer     Bandwidth
[  3]  0.0- 1.0 sec  5.12 MBytes  43.0 Mbits/sec
[  3]  1.0- 2.0 sec  5.00 MBytes  41.9 Mbits/sec
[  3]  2.0- 3.0 sec  5.00 MBytes  41.9 Mbits/sec
[  3]  3.0- 4.0 sec  5.12 MBytes  43.0 Mbits/sec
[  3]  4.0- 5.0 sec  5.00 MBytes  41.9 Mbits/sec
[  3]  5.0- 6.0 sec  5.00 MBytes  41.9 Mbits/sec
[  3]  6.0- 7.0 sec  5.12 MBytes  43.0 Mbits/sec
[  3]  7.0- 8.0 sec  5.00 MBytes  41.9 Mbits/sec
[  3]  8.0- 9.0 sec  4.88 MBytes  40.9 Mbits/sec
[  3]  9.0-10.0 sec  5.00 MBytes  41.9 Mbits/sec
[  3] 10.0-11.0 sec  5.00 MBytes  41.9 Mbits/sec
[  3] 11.0-12.0 sec  4.88 MBytes  40.9 Mbits/sec
[  3] 12.0-13.0 sec  5.00 MBytes  41.9 Mbits/sec
[  3] 13.0-14.0 sec  5.00 MBytes  41.9 Mbits/sec
[  3] 14.0-15.0 sec  4.88 MBytes  40.9 Mbits/sec
[  3] 15.0-16.0 sec  5.12 MBytes  43.0 Mbits/sec
[  3] 16.0-17.0 sec  4.88 MBytes  40.9 Mbits/sec
[  3] 17.0-18.0 sec  5.00 MBytes  41.9 Mbits/sec
[  3] 18.0-19.0 sec  4.88 MBytes  40.9 Mbits/sec
[  3] 19.0-20.0 sec  5.00 MBytes  41.9 Mbits/sec
[  3] 20.0-21.0 sec  4.88 MBytes  40.9 Mbits/sec
[  3] 21.0-22.0 sec  5.00 MBytes  41.9 Mbits/sec
[  3] 22.0-23.0 sec  5.00 MBytes  41.9 Mbits/sec
[  3] 23.0-24.0 sec  4.88 MBytes  40.9 Mbits/sec
[  3] 24.0-25.0 sec  5.00 MBytes  41.9 Mbits/sec
[  3] 25.0-26.0 sec  5.00 MBytes  41.9 Mbits/sec
[  3] 26.0-27.0 sec  5.12 MBytes  43.0 Mbits/sec
[  3] 27.0-28.0 sec  5.00 MBytes  41.9 Mbits/sec
[  3] 28.0-29.0 sec  5.00 MBytes  41.9 Mbits/sec
[  3] 29.0-30.0 sec  5.00 MBytes  41.9 Mbits/sec
[  3]  0.0-30.0 sec   150 MBytes  41.9 Mbits/sec

```
###8Kbytes
```
labredes@labredes-OptiPlex-3010 t2 (master) $ iperf -c 10.32.143.237 -i 1 -p 5001 -w 8K -t 30
------------------------------------------------------------
Client connecting to 10.32.143.237, TCP port 5001
TCP window size: 16.0 KByte (WARNING: requested 8.00 KByte)
------------------------------------------------------------
[  3] local 10.32.143.169 port 40906 connected with 10.32.143.237 port 5001
[ ID] Interval       Transfer     Bandwidth
[  3]  0.0- 1.0 sec  10.2 MBytes  86.0 Mbits/sec
[  3]  1.0- 2.0 sec  10.1 MBytes  84.9 Mbits/sec
[  3]  2.0- 3.0 sec  10.2 MBytes  86.0 Mbits/sec
[  3]  3.0- 4.0 sec  10.1 MBytes  84.9 Mbits/sec
[  3]  4.0- 5.0 sec  10.1 MBytes  84.9 Mbits/sec
[  3]  5.0- 6.0 sec  10.1 MBytes  84.9 Mbits/sec
[  3]  6.0- 7.0 sec  10.2 MBytes  86.0 Mbits/sec
[  3]  7.0- 8.0 sec  10.1 MBytes  84.9 Mbits/sec
[  3]  8.0- 9.0 sec  10.1 MBytes  84.9 Mbits/sec
[  3]  9.0-10.0 sec  10.2 MBytes  86.0 Mbits/sec
[  3] 10.0-11.0 sec  10.1 MBytes  84.9 Mbits/sec
[  3] 11.0-12.0 sec  10.1 MBytes  84.9 Mbits/sec
[  3] 12.0-13.0 sec  10.2 MBytes  86.0 Mbits/sec
[  3] 13.0-14.0 sec  10.2 MBytes  86.0 Mbits/sec
[  3] 14.0-15.0 sec  10.0 MBytes  83.9 Mbits/sec
[  3] 15.0-16.0 sec  10.1 MBytes  84.9 Mbits/sec
[  3] 16.0-17.0 sec  10.1 MBytes  84.9 Mbits/sec
[  3] 17.0-18.0 sec  10.1 MBytes  84.9 Mbits/sec
[  3] 18.0-19.0 sec  10.2 MBytes  86.0 Mbits/sec
[  3] 19.0-20.0 sec  10.0 MBytes  83.9 Mbits/sec
[  3] 20.0-21.0 sec  10.2 MBytes  86.0 Mbits/sec
[  3] 21.0-22.0 sec  10.2 MBytes  86.0 Mbits/sec
[  3] 22.0-23.0 sec  10.2 MBytes  86.0 Mbits/sec
[  3] 23.0-24.0 sec  10.1 MBytes  84.9 Mbits/sec
[  3] 24.0-25.0 sec  10.1 MBytes  84.9 Mbits/sec
[  3] 25.0-26.0 sec  10.1 MBytes  84.9 Mbits/sec
[  3] 26.0-27.0 sec  10.1 MBytes  84.9 Mbits/sec
[  3] 27.0-28.0 sec  10.1 MBytes  84.9 Mbits/sec
[  3] 28.0-29.0 sec  10.1 MBytes  84.9 Mbits/sec
[  3] 29.0-30.0 sec  10.2 MBytes  86.0 Mbits/sec
[  3]  0.0-30.0 sec   305 MBytes  85.2 Mbits/sec

```
###32Kbytes,
```
labredes@labredes-OptiPlex-3010 t2 (master) $ iperf -c 10.32.143.237 -i 1 -p 5001 -w 32K -t 30
------------------------------------------------------------
Client connecting to 10.32.143.237, TCP port 5001
TCP window size: 64.0 KByte (WARNING: requested 32.0 KByte)
------------------------------------------------------------
[  3] local 10.32.143.169 port 40907 connected with 10.32.143.237 port 5001
[ ID] Interval       Transfer     Bandwidth
[  3]  0.0- 1.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3]  1.0- 2.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3]  2.0- 3.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3]  3.0- 4.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3]  4.0- 5.0 sec  11.1 MBytes  93.3 Mbits/sec
[  3]  5.0- 6.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3]  6.0- 7.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3]  7.0- 8.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3]  8.0- 9.0 sec  11.1 MBytes  93.3 Mbits/sec
[  3]  9.0-10.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3] 10.0-11.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3] 11.0-12.0 sec  11.1 MBytes  93.3 Mbits/sec
[  3] 12.0-13.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3] 13.0-14.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3] 14.0-15.0 sec  11.1 MBytes  93.3 Mbits/sec
[  3] 15.0-16.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3] 16.0-17.0 sec  11.1 MBytes  93.3 Mbits/sec
[  3] 17.0-18.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3] 18.0-19.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3] 19.0-20.0 sec  11.1 MBytes  93.3 Mbits/sec
[  3] 20.0-21.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3] 21.0-22.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3] 22.0-23.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3] 23.0-24.0 sec  11.1 MBytes  93.3 Mbits/sec
[  3] 24.0-25.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3] 25.0-26.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3] 26.0-27.0 sec  11.1 MBytes  93.3 Mbits/sec
[  3] 27.0-28.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3] 28.0-29.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3] 29.0-30.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3]  0.0-30.0 sec   337 MBytes  94.1 Mbits/sec

```
###128Kbytes
```
labredes@labredes-OptiPlex-3010 t2 (master) $ iperf -c 10.32.143.237 -i 1 -p 5001 -w 128K -t 30
------------------------------------------------------------
Client connecting to 10.32.143.237, TCP port 5001
TCP window size:  256 KByte (WARNING: requested  128 KByte)
------------------------------------------------------------
[  3] local 10.32.143.169 port 40925 connected with 10.32.143.237 port 5001
[ ID] Interval       Transfer     Bandwidth
[  3]  0.0- 1.0 sec  11.4 MBytes  95.4 Mbits/sec
[  3]  1.0- 2.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3]  2.0- 3.0 sec  11.1 MBytes  93.3 Mbits/sec
[  3]  3.0- 4.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3]  4.0- 5.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3]  5.0- 6.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3]  6.0- 7.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3]  7.0- 8.0 sec  11.1 MBytes  93.3 Mbits/sec
[  3]  8.0- 9.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3]  9.0-10.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3] 10.0-11.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3] 11.0-12.0 sec  11.1 MBytes  93.3 Mbits/sec
[  3] 12.0-13.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3] 13.0-14.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3] 14.0-15.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3] 15.0-16.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3] 16.0-17.0 sec  11.1 MBytes  93.3 Mbits/sec
[  3] 17.0-18.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3] 18.0-19.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3] 19.0-20.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3] 20.0-21.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3] 21.0-22.0 sec  11.1 MBytes  93.3 Mbits/sec
[  3] 22.0-23.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3] 23.0-24.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3] 24.0-25.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3] 25.0-26.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3] 26.0-27.0 sec  11.1 MBytes  93.3 Mbits/sec
[  3] 27.0-28.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3] 28.0-29.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3] 29.0-30.0 sec  11.2 MBytes  94.4 Mbits/sec
[  3]  0.0-30.0 sec   337 MBytes  94.2 Mbits/sec

```
##UDP
throughput,jitter,perda de pacotes variando taxa de envio.

###50 kbits/s

```
labredes@labredes-OptiPlex-3010 t2 (master) $ iperf -c 10.32.143.237 -u -t 3 -p 5002 -b 50K -t 30
------------------------------------------------------------
Client connecting to 10.32.143.237, UDP port 5002
Sending 1470 byte datagrams
UDP buffer size:  208 KByte (default)
------------------------------------------------------------
[  3] local 10.32.143.169 port 34128 connected with 10.32.143.237 port 5002
[ ID] Interval       Transfer     Bandwidth
[  3]  0.0-30.3 sec   185 KBytes  50.0 Kbits/sec
[  3] Sent 129 datagrams
[  3] Server Report:
[  3]  0.0-30.3 sec   185 KBytes  50.0 Kbits/sec   0.010 ms    0/  129 (0%)
```

###1Mbit/s
```
labredes@labredes-OptiPlex-3010 t2 (master) $ iperf -c 10.32.143.237 -u -t 3 -p 5002 -b 1M -t 30
------------------------------------------------------------
Client connecting to 10.32.143.237, UDP port 5002
Sending 1470 byte datagrams
UDP buffer size:  208 KByte (default)
------------------------------------------------------------
[  3] local 10.32.143.169 port 34668 connected with 10.32.143.237 port 5002
[ ID] Interval       Transfer     Bandwidth
[  3]  0.0-30.0 sec  3.58 MBytes  1000 Kbits/sec
[  3] Sent 2553 datagrams
[  3] Server Report:
[  3]  0.0-30.0 sec  3.58 MBytes  1000 Kbits/sec   0.010 ms    0/ 2553 (0%)

```
###5 Mbit/s
```
labredes@labredes-OptiPlex-3010 t2 (master) $ iperf -c 10.32.143.237 -u -t 3 -p 5002 -b 5M -t 30
------------------------------------------------------------
Client connecting to 10.32.143.237, UDP port 5002
Sending 1470 byte datagrams
UDP buffer size:  208 KByte (default)
------------------------------------------------------------
[  3] local 10.32.143.169 port 59038 connected with 10.32.143.237 port 5002
[ ID] Interval       Transfer     Bandwidth
[  3]  0.0-30.0 sec  17.9 MBytes  5.00 Mbits/sec
[  3] Sent 12757 datagrams
[  3] Server Report:
[  3]  0.0-30.0 sec  17.9 MBytes  5.00 Mbits/sec   0.008 ms    0/12757 (0%)
```
###50Mbit/s
```
labredes@labredes-OptiPlex-3010 t2 (master) $ iperf -c 10.32.143.237 -u -t 3 -p 5002 -b 50M -t 30
------------------------------------------------------------
Client connecting to 10.32.143.237, UDP port 5002
Sending 1470 byte datagrams
UDP buffer size:  208 KByte (default)
------------------------------------------------------------
[  3] local 10.32.143.169 port 47014 connected with 10.32.143.237 port 5002
[ ID] Interval       Transfer     Bandwidth
[  3]  0.0-30.0 sec   179 MBytes  50.0 Mbits/sec
[  3] Sent 127661 datagrams
[  3] Server Report:
[  3]  0.0-30.0 sec   179 MBytes  50.0 Mbits/sec   0.007 ms    0/127660 (0%)
[  3]  0.0-30.0 sec  1 datagrams received out-of-order

```

# Link com latência variável
Configurar a interface de rede da máquina utilizada como cliente para incluir latência variável usando o netem. Mostrar a configuração no relatório.
##TCP
throughput variando janela TCP
###1Kbytes
###4Kbytes,
###8Kbytes
###32Kbytes,
###128Kbytes
##UDP
throughput,jitter,perda de pacotes variando taxa de envio.
###50 kbits/s
###1Mbit/s
###5 Mbit/s
###10 Mbit/s,
###50Mbit/s

# Link com perda de pacotes
Configurar a interface de rede da máquina utilizada como cliente para
incluir perda
de pacotes usando o netem. Mostrar a configuração no
relatório.
##TCP
throughput variando janela TCP
###1Kbytes
###4Kbytes,
###8Kbytes
###32Kbytes,
###128Kbytes
##UDP
throughput,jitter,perda de pacotes variando taxa de envio.
###50 kbits/s
###1Mbit/s
###5 Mbit/s
###10 Mbit/s,
###50Mbit/s

