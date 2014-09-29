#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

#include <net/if.h>             // ifr struct
#include <netinet/ether.h>      // header ethernet
#include <netinet/in.h>         // protocol definitions
#include <arpa/inet.h>          // manipulação de endereços IP
#include <netinet/in_systm.h>   // tipos de dados (???)

#include "packets.h"
#include "helpers.h"

void SIGINTHandler(int);

int run = 1;

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    printf("Usage: spoofer <interface>\n");
    exit(-1); 
  }

  if (signal(SIGINT, SIGINTHandler) == SIG_ERR)
  {
    printf("Can't catch SIGINT.\n");
    exit(-1);
  }

  int sockd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  if (sockd < 0)
  {
    printf("Socket creation failed: %s (errno=%u).\n", strerror(errno), errno);
    exit(-1);
  }

  struct ifreq ifr;
	strcpy(ifr.ifr_name, argv[1]);

	if (ioctl(sockd, SIOCGIFINDEX, &ifr) < 0)
  {
		printf("Control device %s failed: %s (errno=%u).", argv[2], strerror(errno), errno);
    exit(-1);
  }

  short flags = ifr.ifr_flags;

	ioctl(sockd, SIOCGIFFLAGS, &ifr);
	ifr.ifr_flags |= IFF_PROMISC;
	ioctl(sockd, SIOCSIFFLAGS, &ifr);

	unsigned char buff[BUFFSIZE];

  ioctl(sockd, SIOCGIFHWADDR, &ifr);

  char hAddr[17];
  AddrToStr(hAddr, (BYTE*)&ifr.ifr_hwaddr.sa_data[0], HADDR);
  printf("Capturing interface %s (%s)...\n", ifr.ifr_name, hAddr);
 
  while (run) 
  {
    recv(sockd,(char *) &buff, sizeof(buff), 0x00);

    Ethernet ethernet;
    MakeEthernet(&buff[0], &ethernet);

    if (ethernet.etherType == P_ARP)
    {
      ARP arp;
      MakeARP(&buff[14], &arp);

      if (arp.operation == 2) // ARP Request
      {
        PrintARP(arp);

        if (!AreEqual(&arp.targetHAddr[0], &ifr.ifr_hwaddr.sa_data[0], HLEN) && !AreEqual(&arp.senderHAddr[0], &ifr.ifr_hwaddr.sa_data[0], HLEN))
        {
          printf("Received ARP REQUEST targeted to another machine!\n");
        }
      }
    }
	}

	ifr.ifr_flags = flags;
	ioctl(sockd, SIOCSIFFLAGS, &ifr);

  printf("\nReturned interface flags to original value.\nFinished spoofing.\n");

  return 0;
}

void SIGINTHandler(int sig)
{
  printf("\nReceived signal to interrupt execution. Terminating...\n");
  run = 0;
}
