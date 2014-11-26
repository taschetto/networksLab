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
void answer_to_ospf_hello(int, ether_header&, iphdr&, ospfhdr&);

struct ifreq ifr;
bool stop = false;
static pthread_mutex_t cs_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

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

       // Tipo Descrição
       // ________________________________
       // 1      Hello
       // 2      Database Description
       // 3      Link State Request
       // 4      Link State Update
       // 5      Link State Acknowledgment

void send_ospf();
void send_hello();
void send_db();
void send_lsr();
void send_lsu();
void send_lsa();



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

        cout << ether_to_str(ether);
        cout << ip_to_str(ip);
        cout << ospf_to_str(ospf);

        switch (ospf.ospf_type)
        {
    			case 1:
            //cout << "Hello" << endl;
            cout << ospf_hello_to_str(ospf);
            answer_to_ospf_hello(socket, ether, ip, ospf);
      			break;
    			case 2:
      			//cout << "Database Description" << endl;
      			cout << ospf_db_to_str(ospf);
      			break;
    			case 3:
      			//cout << "Link State Request" << endl;
      			cout << ospf_lsr_to_str(ospf);
      			break;
    			case 4:
      			//cout << "Link State Update" << endl;
      			cout << ospf_lsu_to_str(ospf);
      			break;
    			case 5:
      			//cout << "Link State Acknowledgment" << endl;
      			cout << ospf_lsa_to_str(ospf);
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

void answer_to_ospf_hello(int socket, ether_header& ethernet, iphdr& ip, ospfhdr& ospf)
{
  u_char buff[ETHER_MAX_LEN];

  // Altera os headers pro pacote de resposta

  memcpy(ethernet.ether_dhost, ethernet.ether_shost, 6);
  memcpy(ethernet.ether_shost, ifr.ifr_hwaddr.sa_data, 6);

  ip.daddr = ip.saddr;
  ip.saddr = ip.saddr;

  cout << ip_to_str(ip);

  // Monta o pacote com os dados

  memcpy(&buff[0], &ethernet, ETHER_HDR_LEN);
  const int IP_HDR_LEN = ip.ihl * 4;
  memcpy(&buff[ETHER_HDR_LEN], &ip, IP_HDR_LEN);
  memcpy(&buff[ETHER_HDR_LEN + IP_HDR_LEN], &ospf, sizeof(ospfhdr));

  struct sockaddr_ll destAddr;
  destAddr.sll_family = htons(PF_PACKET);
  destAddr.sll_protocol = htons(ETH_P_ALL);
  destAddr.sll_halen = ETH_ALEN;
  destAddr.sll_ifindex = ifr.ifr_ifindex;

  memcpy(&destAddr.sll_addr, ethernet.ether_shost, ETH_ALEN);
  std::cout << green << "Answering..." << std::endl;

  int size = ETHER_HDR_LEN + IP_HDR_LEN + sizeof(ospfhdr);
  if (sendto(socket, buff, size, 0, (struct sockaddr *)&destAddr, sizeof(struct sockaddr_ll)) < 0)
  {
    error();
    return;
  }

  ok();
}
