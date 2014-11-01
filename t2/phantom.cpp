#include <cstring>
#include <iostream>
#include <net/ethernet.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>

#include "colors.h"
#include "interface.h"

using namespace std;
using namespace Colors;

void sigint_handler(int);

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

  // Ready to capture. :-)
  cout << blue << "Ready to capture from: " << reset << ifr.ifr_name << endl;
  cout << blue << "      Interface Index: " << reset << ifr.ifr_ifindex << endl;
  cout << blue << "        Hardware Addr: " << reset << ether_ntoa((struct ether_addr*)ifr.ifr_hwaddr.sa_data) << endl;
  cout << blue << "        Protocol Addr: " << reset << inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr) << endl;

  if (resetInterface(socket, ifr) < 0)
    exit(-1);

  if (closeSocket(socket) < 0)
    exit(-1);

  return 0;
}

void sigint_handler(int)
{
}
