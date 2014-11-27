#include <cstring>

#include <iostream>
#include <thread>
#include <iomanip>
#include <net/ethernet.h>
#include <netinet/ether.h>
#include <linux/ip.h>
#include <netinet/udp.h>
#include <netinet/in.h>
#include <netpacket/packet.h>
#include "in_cksum.h"

#include "ipproto.h"
#include "ospf.h"

#include "colors.h"
#include "helpers.h"
#include "interface.h"
#include "print.h"

using namespace std;
using namespace Colors;

void sigint_handler(int);
void sniff(const int, const ifreq&);
void answer_hello(int, const ether_header&, const iphdr&, const ospfhdr&);
void answer_db(int, const ether_header&, const iphdr&, const ospfhdr&);

struct ifreq ifr;
bool stop = false;
static pthread_mutex_t cs_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

std::thread* hello;

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: phantom <interface>" << endl;
    exit(-1);
  }

  if (initSignal(sigint_handler) < 0)
    exit(-1);

  int socket;
  if (initSocket(socket) < 0)
    exit(-1);

  strcpy(ifr.ifr_name, argv[1]);

  if (initInterface(socket, ifr) < 0)
    exit(-1);

  std::thread sniffer(sniff, socket, ifr);
  sniffer.join();

  if (resetInterface(socket, ifr) < 0)
    exit(-1);

  if (closeSocket(socket) < 0)
    exit(-1);

  return 0;
}

void sigint_handler(int)
{
  cout << yellow << "Received SIGINT to interrupt execution." << reset << endl;
  pthread_mutex_lock(&cs_mutex);
  stop = true;
  pthread_mutex_unlock(&cs_mutex);
}

void sniff(const int socket, const ifreq& ifr)
{
  cout << ifreq_to_str(ifr);

  u_char buff[ETHER_MAX_LEN];

  for (;;)
  {
    pthread_mutex_lock(&cs_mutex);
    if (stop)
    {
      pthread_mutex_unlock(&cs_mutex);
      break;
    }
    pthread_mutex_unlock(&cs_mutex);

    if (recv(socket, (char*)&buff, sizeof(buff), 0x00) < ETHER_MIN_LEN)
      continue;

    struct ether_header ether;
    memcpy(&ether, &buff, sizeof(ether));

    if (ntohs(ether.ether_type) == ETHERTYPE_IP)
    {

      struct iphdr ip;
      memcpy(&ip, &buff[ETHER_HDR_LEN], 1);
      const int IP_HDR_LEN = ip.ihl * 4;
      memcpy(&ip, &buff[ETHER_HDR_LEN], IP_HDR_LEN);

      if (ip.protocol == IPPROTO_OSPF)
      {
        struct ospfhdr ospf;
        memcpy(&ospf, &buff[ETHER_HDR_LEN + IP_HDR_LEN], sizeof(ospfhdr));

        //cout << ether_to_str(ether);
        //cout << ip_to_str(ip);
        //cout << ospf_to_str(ospf);

        switch (ospf.ospf_type)
        {
    			case 1:
            //cout << "Hello" << endl;
            //cout << ospf_hello_to_str(ospf);
            if (hello == nullptr)
              hello = new std::thread(answer_hello, socket, ether, ip, ospf);
      			break;
    			case 2:
      			cout << "Database Description" << endl;
      			//cout << ospf_db_to_str(ospf);
            answer_db(socket, ether, ip, ospf);            
      			break;
    			case 3:
      			//cout << "Link State Request" << endl;
      			//cout << ospf_lsr_to_str(ospf);
      			break;
    			case 4:
      			//cout << "Link State Update" << endl;
      			//cout << ospf_lsu_to_str(ospf);
      			break;
    			case 5:
      			//cout << "Link State Acknowledgment" << endl;
      			//cout << ospf_lsa_to_str(ospf);
      			break;
		    }

       // Tipo Descrição
       // ________________________________
       // 1      Hello
       // 2      Database Description
       // 3      Link State Request
       // 4      Link State Update
       // 5      Link State Acknowledgment

       // Pacote de aviso. (Hello packet)
       // Pacote de informações do Banco de Dados (Database Description packet)
       // Requisição de estado de link (Link State Request packet)
       // Atualização de estado de link (Link State Update packet)
       // Recebimento de informações de link (Link State Acknowledgment packet)
      }
    }
  }
}

void answer_hello(int sockt, const ether_header& ethernet, const iphdr& ip, const ospfhdr& ospf)
{
  char buff[ETHER_MAX_LEN] = { 0 };
  
  // Monta Ethernet
  struct ether_header ether_rep;
  memcpy(&ether_rep.ether_dhost, ethernet.ether_shost, ETH_ALEN);
  memcpy(&ether_rep.ether_shost, ifr.ifr_hwaddr.sa_data, ETH_ALEN);
  ether_rep.ether_type = ethernet.ether_type;

  // Monta IP
  struct iphdr ip_rep;
  ip_rep.ihl = ip.ihl;
  ip_rep.version = ip.version;
  ip_rep.tos = ip.tos;
  ip_rep.tot_len = ip.tot_len;
  ip_rep.id = ip.id;
  ip_rep.frag_off = ip.frag_off;
  ip_rep.ttl = ip.ttl;
  ip_rep.protocol = ip.protocol;
  ip_rep.daddr = ip.saddr; // destino é o sender
  ip_rep.saddr = ((struct sockaddr_in*)&ifr.ifr_addr )->sin_addr.s_addr; // sender é o IP da interface

  const int IP_HDR_LEN = ip_rep.ihl * 4;

  // Monta OSPF
  struct ospfhdr ospf_rep;
  ospf_rep.ospf_version  = ospf.ospf_version;
  ospf_rep.ospf_type     = 0x01; // Hello
  memcpy(&ospf_rep.ospf_routerid, &ifr.ifr_addr.sa_data[0], 4);
  memcpy(&ospf_rep.ospf_areaid, &ospf.ospf_areaid, 4);
  ospf_rep.ospf_chksum = 0;
  ospf_rep.ospf_authtype = 0;
  memset(&ospf_rep.ospf_authdata, 0x00, 8);

  // Monta Hello
  struct in_addr link[2] = { 0x0F0E0D0C , 0x0C0D0E0F };

  memcpy(&ospf_rep.ospf_hello.hello_mask, &ospf.ospf_hello.hello_mask, 4);
  ospf_rep.ospf_hello.hello_helloint = ospf.ospf_hello.hello_helloint;
  ospf_rep.ospf_hello.hello_options = 0x00;
  ospf_rep.ospf_hello.hello_priority = 0x01;
  ospf_rep.ospf_hello.hello_deadint = ospf.ospf_hello.hello_deadint;
  memset(&ospf_rep.ospf_hello.hello_dr, 0, 4);
  memset(&ospf_rep.ospf_hello.hello_bdr, 0, 4);
  memcpy(&ospf_rep.ospf_hello.hello_neighbor, &link[0], 4); 
  memcpy(&ospf_rep.ospf_hello.hello_neighbor, &link[1], 4); 

  // Atualiz Length e Checksum do OSPF
  ospf_rep.ospf_len = htons(sizeof(struct ospfhdr));
  ospf_rep.ospf_chksum = 0;
  ospf_rep.ospf_chksum = in_cksum((unsigned short *)&ospf_rep, sizeof(struct ospfhdr));
  const int OSPF_HDR_LEN = ntohs(ospf_rep.ospf_len);

  // Atualiza Length e Checksum do IP  
  int size = ETHER_HDR_LEN + IP_HDR_LEN + OSPF_HDR_LEN;
  ip_rep.tot_len = size - ETHER_HDR_LEN; // Tudo que está do IP pra baixo
  ip_rep.check = 0;
  ip_rep.check = in_cksum((unsigned short *)&ip_rep, sizeof(struct iphdr));

  // Monta o pacote com os dados
  memcpy(&buff[0], &ether_rep, ETHER_HDR_LEN);
  memcpy(&buff[ETHER_HDR_LEN], &ip_rep, IP_HDR_LEN);
  memcpy(&buff[ETHER_HDR_LEN + IP_HDR_LEN], &ospf_rep, OSPF_HDR_LEN);

  struct sockaddr_ll destAddr;
  destAddr.sll_family = htons(PF_PACKET);
  destAddr.sll_protocol = htons(ETH_P_ALL);
  destAddr.sll_halen = ETH_ALEN;
  destAddr.sll_ifindex = 2;

  memcpy(&destAddr.sll_addr, &ethernet.ether_shost, ETH_ALEN);

  for (;;)
  {
    pthread_mutex_lock(&cs_mutex);
    if (stop)
    {
      pthread_mutex_unlock(&cs_mutex);
      break;
    }
    pthread_mutex_unlock(&cs_mutex);

    std::cout << "Sending OSPF HELLO every " << ntohs(ospf_rep.ospf_hello.hello_helloint) << " seconds...";

    if (sendto(sockt, buff, size, 0, (struct sockaddr *)&destAddr, sizeof(struct sockaddr_ll)) < 0)
      error();
    else
      ok();

    std::this_thread::sleep_for(std::chrono::milliseconds(ospf_rep.ospf_hello.hello_helloint * 1000));
  }
}

void answer_db(int sockt, const ether_header& ethernet, const iphdr& ip, const ospfhdr& ospf)
{
  char buff[ETHER_MAX_LEN] = { 0 };

  // Monta Ethernet
  struct ether_header ether_rep;
  memcpy(&ether_rep.ether_dhost, ethernet.ether_shost, ETH_ALEN);
  memcpy(&ether_rep.ether_shost, ifr.ifr_hwaddr.sa_data, ETH_ALEN);
  ether_rep.ether_type = ethernet.ether_type;

  // Monta IP
  struct iphdr ip_rep;
  ip_rep.ihl = ip.ihl;
  ip_rep.version = ip.version;
  ip_rep.tos = ip.tos;
  ip_rep.tot_len = ip.tot_len;
  ip_rep.id = ip.id;
  ip_rep.frag_off = ip.frag_off;
  ip_rep.ttl = ip.ttl;
  ip_rep.protocol = ip.protocol;
  ip_rep.daddr = ip.saddr; // destino é o sender
  ip_rep.saddr = ((struct sockaddr_in*)&ifr.ifr_addr )->sin_addr.s_addr; // sender é o IP da interface

  const int IP_HDR_LEN = ip_rep.ihl * 4;

  // Monta OSPF
  struct ospfhdr ospf_rep;
  ospf_rep.ospf_version  = ospf.ospf_version;
  ospf_rep.ospf_type     = 0x01; // Hello
  memcpy(&ospf_rep.ospf_routerid, &ifr.ifr_addr.sa_data[0], 4);
  memcpy(&ospf_rep.ospf_areaid, &ospf.ospf_areaid, 4);
  ospf_rep.ospf_chksum = 0;
  ospf_rep.ospf_authtype = 0;
  memset(&ospf_rep.ospf_authdata, 0x00, 8);

  // Monta DB Description
  
  /*ospf.ospf_db.db_ifmtu = 0x0000;
  ospf.ospf_db.db_options = 0x08;
  ospf.ospf_db.db_flags = 0x08;
  ospf.ospf_db.db_seq = 0x01;
*/
  // O que botar no LSA HEADER? :/

  // Atualiza Length e Checksum do OSPF
  ospf_rep.ospf_len = htons(sizeof(struct ospfhdr));
  ospf_rep.ospf_chksum = 0;
  ospf_rep.ospf_chksum = in_cksum((unsigned short *)&ospf_rep, sizeof(struct ospfhdr));
  const int OSPF_HDR_LEN = ntohs(ospf_rep.ospf_len);

  // Atualiza Length e Checksum do IP
  int size = ETHER_HDR_LEN + IP_HDR_LEN + OSPF_HDR_LEN;
  ip_rep.tot_len = size - ETHER_HDR_LEN; // tudo que está do IP pra baixo
  ip_rep.check = 0;
  ip_rep.check = in_cksum((unsigned short *)&ip_rep, sizeof(struct iphdr));

  // Monta o pacote com os dados
  memcpy(&buff[0], &ether_rep, ETHER_HDR_LEN);
  memcpy(&buff[ETHER_HDR_LEN], &ip_rep, IP_HDR_LEN);
  memcpy(&buff[ETHER_HDR_LEN + IP_HDR_LEN], &ospf_rep, OSPF_HDR_LEN);

  struct sockaddr_ll destAddr;
  destAddr.sll_family = htons(PF_PACKET);
  destAddr.sll_protocol = htons(ETH_P_ALL);
  destAddr.sll_halen = ETH_ALEN;
  destAddr.sll_ifindex = 2;
  memcpy(&destAddr.sll_addr, &ethernet.ether_shost, ETH_ALEN);

  for (;;)
  {
    pthread_mutex_lock(&cs_mutex);
    if (stop)
    {
      pthread_mutex_unlock(&cs_mutex);
      break;
    }
    pthread_mutex_unlock(&cs_mutex);

    std::cout << "Sending OSPF HELLO every " << ntohs(ospf_rep.ospf_hello.hello_helloint) << " seconds...";

    if (sendto(sockt, buff, size, 0, (struct sockaddr *)&destAddr, sizeof(struct sockaddr_ll)) < 0)
      error();
    else
      ok();

    std::this_thread::sleep_for(std::chrono::milliseconds(ospf_rep.ospf_hello.hello_helloint * 1000));
  }
}
