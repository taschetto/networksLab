##Simulação TCP
```
Window Size (KByte)	Total (MBytes)	Throughput (Mbits/sec)
1.00	84.1	23.5
4.00	148	41.3
8.00	312	87.3
32.0	337	94.1
128	337	94.2
```
##Simulação UDP
```
Taxa	Throughput (Mbits/sec)	Jitter (ms)	Loss (%)
0.05	0.05	0.009	0
1.00	0.05	0.007	0
5.00	1.66	0.008	0
10.0	2.49	0.011	0
50.0	9.98	0.006	0
```
##Simulação TCP com latência variável
```
Window Size (KByte)	Total (MBytes)	Throughput (Mbits/sec)
1.00	1.00	0.221
4.00	1.00	0.220
8.00	1.75	0.433
32.0	6.00	1.63
128	20.9	5.79

```
##Simulação UDP com latência variável
```
Taxa	Throughput (Mbits/sec)	Jitter (ms)	Loss (%)
0.05	0.05	6.720	0
1.00	0.05	6.734	8.7
5.00	1.66	6.435	0.0078
10.0	2.48	5.515	69
50.0	9.95	7.002	0.011
```
##Simulação TCP com perda de pacotes
```
Window Size (KByte)	Total (MBytes)	Throughput (Mbits/sec)
1.00	14.8	3.91
4.00	17.1	4.78
8.00	19.6	5.45
32.0	160	44.3
128	163	45.7
```
##Simulação UDP com perda de pacotes
```
Taxa	Throughput (Mbits/sec)	Jitter (ms)	Loss (%)
0.050	0.049	0.009	0.78
1.00	0.0487	0.015	2.1
5.00	1.63	0.005	1.9
10.0	2.45	0.006	1.7
50.0	9.80	0.007	1.7
```
