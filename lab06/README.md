## Objetivos
Configurar o protocolo IPv6 no roteador e nas máquinas da sub-rede, de forma a explorar as possíveis formas de configuração do endereço IPv6. Além disso, deve ser habilitado o roteamento OSPF para a pilha IPv6.

### Configuração do Roteador

Acessando o terminal do roteador:
```
Router>enable
Router#conf term
Enter configuration commands, one per line.  End with CNTL/Z.
Router(config)#
```

Configuração da interface GigabitEthernet e do roteador OSPF:
```
Router(config)#ipv6 unicast-routing 
Router(config)#ipv6 router ospf 1
Router(config-rtr)#router-id 1.1.1.1
Router(config-rtr)#exit
```

Configuração do DHCP no roteador
```
Router(config)#ipv6 cef
Router(config)#ipv6 dhcp
Router(config)#ipv6 dhcp pool area3 
Router(config-dhcpv6)#address prefix 2001:feed:beef::/64 lifetime infinite infinite
```

Configuração da Interface Gigabit
```
Router(config)#int g0
Router(config-if)#ipv6 enable
Router(config-if)#ipv6 nd managed-config-flag 
Router(config-if)#ipv6 dhcp server area3
Router(config-if)#ipv6 address 2001:feed:beef::1/64
Router(config-if)#ipv6 ospf 1 area 0
Router(config-if)#no shutdown
Router(config-if)#exit
```

Saindo e salvando a configuração:
```
Router(config)#exit
Router#write
Building configuration... [OK]
```

Teste da configuração:

```
Router#show ipv6 route
IPv6 Routing Table - default - 4 entries
Codes: C - Connected, L - Local, S - Static, U - Per-user Static route
       B - BGP, HA - Home Agent, MR - Mobile Router, R - RIP
       D - EIGRP, EX - EIGRP external, ND - Neighbor Discovery
       O - OSPF Intra, OI - OSPF Inter, OE1 - OSPF ext 1, OE2 - OSPF ext 2
       ON1 - OSPF NSSA ext 1, ON2 - OSPF NSSA ext 2
C   2001:FEED:BEEF::/64 [0/0]
     via GigabitEthernet0, directly connected
L   2001:FEED:BEEF::/128 [0/0]
     via GigabitEthernet0, receive
L   2001:FEED:BEEF::1/128 [0/0]
     via GigabitEthernet0, receive
L   FF00::/8 [0/0]
     via Null0, receive
```

```
Router#show ipv6 interface brief 
...
GigabitEthernet0           [up/up]
    FE80::523D:E5FF:FED9:7D0
    2001:FEED:BEEF::
    2001:FEED:BEEF::1
```

### Autoconfiguração das máquinas da sub-rede por Router Advertisement
### Configuração das máquinas da sub-rede com DHCP para IPv6

#### Resultados e Entrega
- Grupos: em duplas
- Entrega: Relatório contendo:
 - requisitos para habilitar IPv6 e também OSPF;
 - configuração realizada;
 - resultados coletados
 - análise do experimento. 

