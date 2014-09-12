ARP Request
	IP Origem	Mac Origem
	IP Destino
	Mac Destino: 0:0:0:0:0:0

A máquina que possui o IP destino responde com o Mac dela. Nosso programa também irá responder por ela, tentando fazer com que a origem do ARP Request pense que o nosso Mac é o endereço de hardware correto do Ip Destino.

Não adianta mandar uma única mensagem de ARP Reply, pois a máquina atacada poderá, com o tempo, descobrir o Mac correto. A idéia então, é ficar enviando vários ARP Replies até o usuário cancelar.

O **primeiro** ARP Reply deve ser impressora na tela.

O programa deverá também capturar e mostrar os pacotes recebidos em função do ataque. IP-TCP-PORTA, IP-UDP-PORTA, IP-ICMP.

No fim, restaurar a máquina para o IP correto. Ou seja, devemos enviar um ARP Request e esperar um ARP Reply da máquina verdadeira. Então, enviar um ARP Request para a máquina atacada.