#PUCRS - Faculdade de Informática#

## Laboratório de Redes - Tarefa 4 - GNS 3 ##

### Por Guilherme Taschetto ([gtaschetto@gmail.com](mailto:gtaschetto@gmail.com)) ###

###<center>05/09/2014</center>###


**Objetivos:** Configuração do NAT (estático e dinâmico) e RIP no GNS3.

**Topologia:** A topologia da simulação consiste em:

 - 5 roteadores em série (R4 - R1 - R2 - R3 - R5);
 - R4 ligado à nuvem C1;
 - R3 ligado ao switch SW2;
 - R5 ligado ao switch SW1;
 - Todas interfaces do tipo *Fast Ethernet*.


----------
### Definição de Sub-redes com Endereçamento Classe C ###

Para as redes internas, entre os roteadores e os switches, foram definidos endereços de sub-redes conforme o esquema simplificado abaixo:

- R4 - R1: `192.168.1.0`
- R1 - R2: `192.168.2.0`
- R2 - R3: `192.168.3.0`
- R3 - R5: `192.168.4.0`
- R3 - SW2: `192.168.5.0`
- R5 - SW1: `192.168.6.0`

### Configuração das Interfaces ###
Para cada interface utilizada de cada roteador foram configurados endereço IPv4 e máscara de sub-rede. Ao acessar a configuração do roteador em questão, a sequência de comandos para configurar uma interface utilizada foi:

```
Router>enable						// habilita terminal  
Router#config term					// acessa terminal de configuração
Router(config)#int <INTERFACE>		// seleciona a interface para configuração
Router(config-if)#ip address <IP> <MASK>// define ip e máscara da interface
Router(config-if)#no shutdown		// habilita a interface
Router(config-if)#^Z				// retorna para o nível anterior
Router#write						// grava as configurações
Router#show interface <INTERFACE>	// lista as configurações da interface
```

Através dos comandos listados acima foram definidos endereços para as interfaces de acordo com os endereços de sub-rede previamente definidos.

| ROTEADOR  | INTERFACE   | ENDEREÇO IP   |
 ---------- | ----------- | --------------
| R4        | `f1/0`       | `192.168.1.2`  |
| R1        | `f0/0`       | `192.168.1.1`  |
| R1        | `f1/0`       | `192.168.2.2`  |
| R2        | `f0/0`       | `192.168.2.1`  |
| R2        | `f1/0`       | `192.168.3.2`  |
| R3        | `f0/0`       | `192.168.3.1`  |
| R3        | `f1/0`       | `192.168.4.2`  |
| R3        | `f2/0` (SW2) | `192.168.5.2`  |
| R5        | `f0/0`       | `192.168.4.1`  |
| R5        | `f1/0` (SW1) | `192.168.6.2`  |

> **Nota:** todas as interfaces foram configuradas com 24 bits de máscara de sub-rede, ou seja, `255.255.255.0`.

Para configurar a interface `f1/0` do roteador R1, por exemplo:
```
Router>enable
Router#config term
Router(config)#int f1/0
Router(config-if)#ip address 192.168.1.2 255.255.255.0
Router(config-if)#no shutdown
Router(config-if)#^Z
Router#write
```

Como exemplo de verificação de configuração bem sucedida, segue abaixo saída do comando `show interface` para a interface `f1/0` do roteador R4 (dados irrelevantes foram omitidos).

```
Router#show int f1/0
FastEthernet1/0 is up, line protocol is up 
  Hardware is AmdFE, address is cc09.1157.0010 (bia cc09.1157.0010)
  Internet address is 192.168.1.2/24
  MTU 1500 bytes, BW 100000 Kbit, DLY 100 usec, 
     reliability 255/255, txload 1/255, rxload 1/255
```


----------

### Roteamento Estático ###

De imediato é possível testar, através do comando `ping`, a comunicação entre cada roteador e às sub-redes às quais está diretamente conectado. Por exemplo, no roteador R1 é possível encontrar a interface `f1/0` do roteador R4.

```
Router#ping 192.168.1.2

Type escape sequence to abort.
Sending 5, 100-byte ICMP Echos to 192.168.1.2, timeout is 2 seconds:
.!!!!
Success rate is 80 percent (4/5), round-trip min/avg/max = 32/50/64 ms
```

Entretanto não é possível um roteador enviar um `ping` para sub-redes que encontram-se além dos roteadores diretamente ligados à ele, como, por exemplo, a interface `f0/0` do roteador R3:

```
Router#ping 192.168.3.1 

Type escape sequence to abort.
Sending 5, 100-byte ICMP Echos to 192.168.3.1, timeout is 2 seconds:
.....
Success rate is 0 percent (0/5)
```

Por que isto ocorra precisamos criar rotas estáticas no roteadores para cada rede que não está diretamente conectada. Por exemplo, para R4 temos:

```
Router(config)#ip route 192.168.2.0 255.255.255.0 192.168.1.1
Router(config)#ip route 192.168.3.0 255.255.255.0 192.168.1.1   
Router(config)#ip route 192.168.4.0 255.255.255.0 192.168.1.1
Router(config)#ip route 192.168.5.0 255.255.255.0 192.168.1.1
Router(config)#ip route 192.168.6.0 255.255.255.0 192.168.1.1
```

A nova tabela de roteamento pode ser consulta pelo comando `show ip route`:

####R4####
```
Router#show ip route
S    192.168.4.0/24 [1/0] via 192.168.1.1
S    192.168.5.0/24 [1/0] via 192.168.1.1
S    192.168.6.0/24 [1/0] via 192.168.1.1
C    192.168.1.0/24 is directly connected, FastEthernet1/0
S    192.168.2.0/24 [1/0] via 192.168.1.1
S    192.168.3.0/24 [1/0] via 192.168.1.1
```

O processo foi repetido para todos os roteadores, levando em consideração as redes conectadas diretamenta ao roteador e suas interfaces.

####R1####
```
S    192.168.4.0/24 [1/0] via 192.168.2.1
S    192.168.5.0/24 [1/0] via 192.168.2.1
S    192.168.6.0/24 [1/0] via 192.168.2.1
C    192.168.1.0/24 is directly connected, FastEthernet0/0
C    192.168.2.0/24 is directly connected, FastEthernet1/0
S    192.168.3.0/24 [1/0] via 192.168.2.1
```

####R2####
```
S    192.168.4.0/24 [1/0] via 192.168.3.1
S    192.168.5.0/24 [1/0] via 192.168.3.1
S    192.168.6.0/24 [1/0] via 192.168.3.1
S    192.168.1.0/24 [1/0] via 192.168.2.2
C    192.168.2.0/24 is directly connected, FastEthernet0/0
C    192.168.3.0/24 is directly connected, FastEthernet1/0
```

####R3####
```
C    192.168.4.0/24 is directly connected, FastEthernet1/0
C    192.168.5.0/24 is directly connected, FastEthernet2/0
S    192.168.6.0/24 [1/0] via 192.168.4.1
S    192.168.1.0/24 [1/0] via 192.168.3.2
S    192.168.2.0/24 [1/0] via 192.168.3.2
C    192.168.3.0/24 is directly connected, FastEthernet0/0
```

####R5####
```
C    192.168.4.0/24 is directly connected, FastEthernet0/0
S    192.168.5.0/24 [1/0] via 192.168.4.2
C    192.168.6.0/24 is directly connected, FastEthernet1/0
S    192.168.1.0/24 [1/0] via 192.168.4.2
S    192.168.2.0/24 [1/0] via 192.168.4.2
S    192.168.3.0/24 [1/0] via 192.168.4.2
```

Agora, ao tentar o mesmo comando de `ping` no roteador R1:

```
Router#ping 192.168.3.1

Type escape sequence to abort.
Sending 5, 100-byte ICMP Echos to 192.168.3.1, timeout is 2 seconds:
..!!!
Success rate is 60 percent (3/5), round-trip min/avg/max = 64/85/96 ms
```

Um teste mais completo pode ser feito realizando o `ping` de ponta a ponta, ou seja, de R4 à R5:

```
Router>ping 192.168.6.2

Type escape sequence to abort.
Sending 5, 100-byte ICMP Echos to 192.168.6.2, timeout is 2 seconds:
.!!!!
Success rate is 80 percent (4/5), round-trip min/avg/max = 112/123/132 ms
```

Portanto, isto comprova que a configuração das tabelas estáticas de roteamento estão corretas. 

----------

###Rota Padrão (0.0.0.0/0)###
Para o acesso à nuvem é necessário configurar uma rota padrão em cada roteador, sempre roteando em direção à nuvem. Isto pode ser feito com o comando `ip route` porém passando o IP `0.0.0.0` e máscara `0.0.0.0` (caracterização da rota padrão). Por exemplo, em R5:

```
Router(config)#ip route 0.0.0.0 0.0.0.0 192.168.4.2
Router#show ip route
C    192.168.4.0/24 is directly connected, FastEthernet0/0
S    192.168.5.0/24 [1/0] via 192.168.4.2
C    192.168.6.0/24 is directly connected, FastEthernet1/0
S    192.168.1.0/24 [1/0] via 192.168.4.2
S    192.168.2.0/24 [1/0] via 192.168.4.2
S    192.168.3.0/24 [1/0] via 192.168.4.2
S*   0.0.0.0/0 [1/0] via 192.168.4.2
```

No roteador R4, que está conectado à nuvem, a configuração é análoga, embora a rota padrão indique o *gateway* padrão da interface de rede física do sistema operacional, que faz a ligação à nuvem.

```
guilherme@Ubuntu14 bin $ ifconfig
eth0      Link encap:Ethernet  HWaddr 08:00:27:e2:66:af  
          inet addr:10.0.2.15  Bcast:10.0.2.255  Mask:255.255.255.0

guilherme@Ubuntu14 bin $ ip route show
default via 10.0.2.2 dev eth0  proto static 
10.0.2.0/24 dev eth0  proto kernel  scope link  src 10.0.2.15  metric 1 
```

Agora é necessário configurar a interface `f0/0` de R4 com o mesmo IP e configurar uma rota padrão roteando o *gateway* padrão:

```
Router>enable
Router#conf term
Router(config)#int f0/0
Router(config-if)#ip address 10.0.2.15 255.255.255.0
Router(config-if)#no shutdown
Router#show ip route
S    192.168.4.0/24 [1/0] via 192.168.1.1
S    192.168.5.0/24 [1/0] via 192.168.1.1
     10.0.0.0/24 is subnetted, 1 subnets
C    10.0.2.0 is directly connected, FastEthernet0/0
S    192.168.6.0/24 [1/0] via 192.168.1.1
C    192.168.1.0/24 is directly connected, FastEthernet1/0
S    192.168.2.0/24 [1/0] via 192.168.1.1
S    192.168.3.0/24 [1/0] via 192.168.1.1
S*   0.0.0.0/0 [1/0] via 10.0.2.2
```

Tentei pingar Google e através de R4 e não deu:

```
Router#ping 201.86.233.231

Type escape sequence to abort.
Sending 5, 100-byte ICMP Echos to 201.86.233.231, timeout is 2 seconds:
.....
Success rate is 0 percent (0/5)
```
