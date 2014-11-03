#include <cstring>

#include <iostream>
#include <thread>
#include <iomanip>
#include <net/ethernet.h>
#include <netinet/ether.h>
#include <linux/ip.h>
#include <netinet/udp.h>

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
      memcpy(&ip, &buff[ETHER_HDR_LEN], ip.ihl * 4);

      if (ntohs(ip.protocol) == IPPROTO_UDP)
      {
        cout << ether_to_str(ether);
        cout << ip_to_str(ip);
        cout << endl << red << "Received UDP packet!" << reset << endl;
      }
    }
  }
}
