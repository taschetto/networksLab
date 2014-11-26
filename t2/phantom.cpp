#include <cstring>

#include <iostream>
#include <thread>
#include <iomanip>
#include <net/ethernet.h>
#include <netinet/ether.h>
#include <linux/ip.h>
#include <netinet/udp.h>
#include <netinet/in.h>

#include "ipproto.h"
#include "ospf.h"

#include "colors.h"
#include "interface.h"
#include "print.h"

using namespace std;
using namespace Colors;

void sigint_handler(int);
void sniff(const int, const ifreq&);

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

  struct ifreq ifr;
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

        switch((int)ospf.ospf_type){
			case 1:
            //cout << "Hello" << endl;
            cout << ospf_hello_to_str(ospf);
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
