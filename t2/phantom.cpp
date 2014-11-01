#include <cstring>

#include <iostream>
#include <thread>

#include <net/ethernet.h>
#include <netinet/ether.h>
#include <linux/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>

#include "colors.h"
#include "interface.h"

using namespace std;
using namespace Colors;

void sigint_handler(int);
void sniff(const int, const ifreq&);

bool stop = false;

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

void sigint_handler(int)
{
  cout << yellow << "Received SIGINT to interrupt execution." << reset << endl;
  stop = true;
}

void sniff(const int socket, const ifreq& ifr)
{
  cout << blue << "Ready to capture from: " << reset << ifr.ifr_name << endl;
  cout << blue << "      Interface Index: " << reset << ifr.ifr_ifindex << endl;
  cout << blue << "        Hardware Addr: " << reset << ether_ntoa((struct ether_addr*)ifr.ifr_hwaddr.sa_data) << endl;
  cout << blue << "        Protocol Addr: " << reset << inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr) << endl;

  u_char buff[ETHER_MAX_LEN];

  for (;;)
  {
    if (stop)
      break;

    if (recv(socket, (char*)&buff, sizeof(buff), 0x00) < ETHER_MIN_LEN)
      continue;

    struct ether_header ether;
    memcpy(&ether, &buff, sizeof(ether));
      
    cout << endl;
    cout << blue << "Ethernet Packet" << reset << endl;
    cout << blue << "Destination: " << reset << ether_ntoa((struct ether_addr*)ether.ether_dhost) << endl;
    cout << blue << "        Src: " << reset << ether_ntoa((struct ether_addr*)ether.ether_shost) << endl;
    cout << blue << "       Type: " << reset << ntohs(ether.ether_type) << endl;

    if (ntohs(ether.ether_type) == ETHERTYPE_IP)
    {
      struct iphdr ip;
      memcpy(&ip, &buff[ETHER_HDR_LEN], sizeof(iphdr));

      cout << endl;
      cout << blue << "IP Packet" << reset << endl;
      cout << blue << "        IHL: " << reset << ip.ihl << endl;
      cout << blue << "    Version: " << reset << ip.version << endl;
      cout << blue << "        TOS: " << reset << ip.tos << endl;
      cout << blue << "        LEN: " << reset << ip.tot_len << endl;
      cout << blue << "         ID: " << reset << ip.id << endl;
      cout << blue << "   Frag Off: " << reset << ip.frag_off << endl;
      cout << blue << "        TTL: " << reset << ip.ttl << endl;
      cout << blue << "   Protocol: " << reset << ntohs(ip.protocol) << endl;
      cout << blue << "      Check: " << reset << ip.check << endl;
      cout << blue << "     Source: " << reset << inet_ntoa(((struct sockaddr_in *)&ip.saddr)->sin_addr) << endl;
      cout << blue << "Destination: " << reset << inet_ntoa(((struct sockaddr_in *)&ip.daddr)->sin_addr) << endl;

      if (ntohs(ip.protocol) == IPPROTO_UDP)
      {
        cout << endl << red << "Received UDP packet!" << reset << endl;
      }
    }
  }
}
