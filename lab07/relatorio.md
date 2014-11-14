#PUCRS - Faculdade de Informática#

## Laboratório de Redes - Tarefa 7 - IPERF & NETEM ##

### Por Guilherme Taschetto ([gtaschetto@gmail.com](mailto:gtaschetto@gmail.com)) ###

###<center>14/11/2014</center>###


**Objetivos:** Simulação, monitoração e avaliação de desempenho de comunicação através dos protocols TDP e UDP sob distintas condições.

##Procedimento para a simulação
Em uma máquina servidora (```10.32.143.236```) abri duas abas do terminal, uma instanciando um servidor TCP e a outra UDP:
```
iperf -s # cria um servidor TCP na porta padrão (5001)
iperf -s -u -p 5002 (cria um servidor UDP na porta 5002
```

Como as simulações envolviam a repetição de comandos parametrizados, criei um script bash para a realização das simulações. Algumas linhas foram omitidas para permitir uma melhor leitura. A explicação é feita em comentários no próprio script.
```
#!/bin/bash

# Parâmetros da simulação
SERVER=10.32.143.236	# IP do servidor.
TCP=5001				# Porta TCP do servidor.
UDP=5002				# Porta UDP do servidor.
DURATION=30				# Duração de cada simulação, em segundos.
FILE=report.md			# Arquivo do relatório.

# Realiza uma simulação TCP com os parâmetros e saida para o arquivo.
SimulaTcp()
{
  iperf -c $SERVER -i 1 -p $TCP -w $1 -t $DURATION >> "$FILE"
}

# Realiza uma simulação UDP com os parâmetros e saída para o arquivo.
SimulaUdp()
{
  iperf -c $SERVER -u -t 3 -p $UDP -b $1 -t $DURATION >> "$FILE"
}

# Executa um lote de simulações TCP.
GrupoSimulaTcp()
{
  SimulaTcp "1K"
  SimulaTcp "4K"
  SimulaTcp "8K"
  SimulaTcp "32K"
  SimulaTcp "128K"
}

# Execute um lote de simulações UDP.
GrupoSimulaUdp()
{
  SimulaUdp "50k" 
  SimulaUdp "1M"
  SimulaUdp "5M"
  SimulaUdp "10M"
  SimulaUdp "50M"
}

GrupoSimulaTcp # Roda todas as simulações para TCP
GrupoSimulaUdp # Roda todas as simulações para UDP

# Limpa a configuração do netem e adiciona simulação de delay variável (100ms +- 10ms) para a interface eth3.
sudo tc qdisc del dev eth3 root
sudo tc qdisc add dev eth3 root netem delay 100ms 10ms 25%

GrupoSimulaTcp # Roda todas as simulações para TCP
GrupoSimulaUdp # Roda todas as simulações para UDP

# Limpa a configuração do netem e adiciona simulação de perda de pacotes (10% com probabilidade de 25% de ocorrer) para a interface eth3.
sudo tc qdisc del dev eth3 root
sudo tc qdisc add dev eth3 root netem loss 10% 25%

GrupoSimulaTcp # Roda todas as simulações para TCP
GrupoSimulaUdp # Roda todas as simulações para UDP

# Limpa a configuração do netem.
sudo tc qdisc del dev eth3 root
```
Com isto, foi possível gerar um grande arquivo com todas as simulações desejadas.

----------
### Simulações sem alterações de rede ###

Blá.

----------
### Simulações com latência variável ###

Blá.

----------
### Simulações com perda de pacotes ###

Blá.

