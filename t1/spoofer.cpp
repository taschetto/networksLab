#include <stdlib.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

#include <iostream>

#include <net/if.h>             // ifr struct
#include <netinet/ether.h>      // header ethernet
#include <netinet/in.h>         // protocol definitions
#include <arpa/inet.h>          // manipulação de endereços IP
#include <netinet/in_systm.h>   // tipos de dados (???)

#include "helpers.h"
#include "ethernet.h"
#include "arp.h"
#include "ip.h"

void SIGINTHandler(int);

int run = 1;

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cerr << "Usage: spoofer <interface>" << std::endl;
    exit(-1); 
  }

  if (signal(SIGINT, SIGINTHandler) == SIG_ERR)
  {
    std::cerr << "Can't catch SIGINT." << std::endl;
    exit(-1);
  }

  int sockd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  if (sockd < 0)
  {
    std::cerr << "Socket creation failed: " << strerror(errno) << " (errno=" << errno << ")." << std::endl;
    exit(-1);
  }

  struct ifreq ifr;
	strcpy(ifr.ifr_name, argv[1]);

	if (ioctl(sockd, SIOCGIFINDEX, &ifr) < 0)
  {
    std::cerr << "Control device " << argv[1] << " failed: " << strerror(errno) << " (errno=" << errno << ")." << std::endl;
    exit(-1);
  }

  ioctl(sockd, SIOCGIFHWADDR, &ifr);

  std::cout << "Capturing interface " << ifr.ifr_name << " (" << MACToStr((BYTE*)ifr.ifr_hwaddr.sa_data) << ")..." << std::endl;

  short flags = ifr.ifr_flags;

	ioctl(sockd, SIOCGIFFLAGS, &ifr);
	ifr.ifr_flags |= IFF_PROMISC;
	ioctl(sockd, SIOCSIFFLAGS, &ifr);

	unsigned char buff[BUFFSIZE];
 
  while (run) 
  {
    recv(sockd,(char *) &buff, sizeof(buff), 0x00);

    Ethernet ethernet(&buff[0]);

    if (ethernet.etherType == P_ARP)
    {
      Arp arp(&buff[14]);

      if (arp.operation == 2) // ARP Request
      {
        std::cout << arp.ToString();

        if (!AreEqual(&arp.targetHAddr[0], (BYTE*)ifr.ifr_hwaddr.sa_data, HLEN) && !AreEqual(&arp.senderHAddr[0], (BYTE*)ifr.ifr_hwaddr.sa_data, HLEN))
        {
          std::cout << "Received ARP REQUEST targeted to another machine!" << std::endl;
        }
      }
    }
	}

	ifr.ifr_flags = flags;
	ioctl(sockd, SIOCSIFFLAGS, &ifr);

  std::cout << std::endl << "Returned interface flags to original value.\nFinished spoofing." << std::endl;

  return 0;
}

void SIGINTHandler(int sig)
{
  std::cout << std::endl << "Received signal to interrupt execution. Terminating..." << std::endl;
  run = 0;
}
