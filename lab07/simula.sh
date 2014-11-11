#!/bin/bash

SERVER=10.32.143.236
TCP=5001
UDP=5002
DURATION=30
FILE=report.md
TITLE=""

SimulaTcp()
{
  echo $TITLE " ~ " $1
  echo "##$TITLE ~ $1" >> "$FILE"
  echo "\`\`\`" >> "$FILE"
  iperf -c $SERVER -i 1 -p $TCP -w $1 -t $DURATION >> "$FILE"
  echo "\`\`\`" >> "$FILE"
}

SimulaUdp()
{
  echo $TITLE " ~ " $1
  echo "##$TITLE ~ $1" >> "$FILE"
  echo "\`\`\`" >> "$FILE"
  iperf -c $SERVER -u -t 3 -p $UDP -b $1 -t $DURATION >> "$FILE"
  echo "\`\`\`" >> "$FILE"
}

GrupoSimulaTcp()
{
  SimulaTcp "1K"
  SimulaTcp "4K"
  SimulaTcp "8K"
  SimulaTcp "32K"
  SimulaTcp "128K"
}

GrupoSimulaUdp()
{
  SimulaUdp "50k" 
  SimulaUdp "1M"
  SimulaUdp "5M"
  SimulaUdp "10M"
  SimulaUdp "50M"
}

TITLE="Simulação TCP"
GrupoSimulaTcp

TITLE="Simulação UDP"
GrupoSimulaUdp

sudo tc qdisc del dev eth3 root
sudo tc qdisc add dev eth3 root netem delay 100ms 10ms 25%

TITLE="Simulação TCP com latência variável"
GrupoSimulaTcp

TITLE="Simulação UDP com latência variável"
GrupoSimulaUdp

sudo tc qdisc del dev eth3 root
sudo tc qdisc add dev eth3 root netem loss 10% 25%

TITLE="Simulação TCP com perda de pacotes"
GrupoSimulaTcp

TITLE="Simulação UDP com perda de pacotes"
GrupoSimulaUdp

sudo tc qdisc del dev eth3 root
