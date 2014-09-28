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

#define BUFFSIZE 1518

#define HADDR 0
#define PADDR 1

#define HLEN 6
#define PLEN 4

#define P_IPv4 0x0800
#define P_ARP  0x0806

typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned int   DWORD;

typedef struct
{
  BYTE destination[HLEN];
  BYTE source[HLEN];
  DWORD etherType;
} Ethernet;

typedef struct
{
  WORD hType;
  WORD pType;
  BYTE hLen;
  BYTE pLen;
  WORD operation;
  BYTE senderHAddr[HLEN];
  BYTE senderPAddr[PLEN];
  BYTE targetHAddr[HLEN];
  BYTE targetPAddr[PLEN];
} ARP;

void MakeEthernet(BYTE*, Ethernet*);
void PrintEthernet(Ethernet);

void MakeARP(BYTE*, ARP*);
void PrintARP(ARP);

void MakeAddr(BYTE*, BYTE*, BYTE);

WORD MakeWord(BYTE*);
DWORD MakeDWord(BYTE*);
void AddrToStr(char*, BYTE*, BYTE);

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
 
  while (run) 
  {
    recv(sockd,(char *) &buff, sizeof(buff), 0x00);

    Ethernet ethernet;
    MakeEthernet(&buff[0], &ethernet);
    //PrintEthernet(ethernet);

    switch(ethernet.etherType)
    {
      case P_ARP:
      {
        ARP arp;
        MakeARP(&buff[14], &arp);
        PrintEthernet(ethernet);
        PrintARP(arp);
      } break;
    }
	}

	ifr.ifr_flags = flags;
	ioctl(sockd, SIOCSIFFLAGS, &ifr);

  printf("FINISH!");

  return 0;
}

void MakeEthernet(BYTE* by, Ethernet* packet)
{
  MakeAddr(&by[0], (*packet).destination, HLEN);
  MakeAddr(&by[6], (*packet).source, HLEN);
  (*packet).etherType = MakeWord(&by[12]);
}

void PrintEthernet(Ethernet packet)
{
  printf("------------ ETHERNET PACKET -----------\n");
  char addr[17];
  AddrToStr(addr, packet.destination, HADDR);
	printf("Destination MAC: %s\n", addr);
  AddrToStr(addr, packet.source, HADDR);
	printf("     Source MAC: %s\n", addr);
  printf("     Ether Type: %04X\n", packet.etherType);
}

void MakeARP(BYTE* by, ARP* packet)
{
  (*packet).hType = MakeWord(&by[0]);
  (*packet).pType = MakeWord(&by[2]);
  (*packet).hLen  = by[4];
  (*packet).pLen  = by[5];
  (*packet).operation = MakeWord(&by[6]);
  MakeAddr(&by[8], (*packet).senderHAddr, HLEN);
  MakeAddr(&by[14], (*packet).senderPAddr, PLEN);
  MakeAddr(&by[18], (*packet).targetHAddr, HLEN);
  MakeAddr(&by[24], (*packet).targetPAddr, PLEN);
}

void PrintARP(ARP packet)
{
  printf("-------------- ARP PACKET --------------\n");
  printf("Hardware Type: %u\n", packet.hType);
  printf("Protocol Type: %04X\n", packet.pType);
  printf(" HAddr Length: %u\n", packet.hLen);
  printf(" PAddr Length: %u\n", packet.pLen);
  printf("    Operation: %u\n", packet.operation);
  char hAddr[17], pAddr[15];
  AddrToStr(hAddr, packet.senderHAddr, HADDR);
  AddrToStr(pAddr, packet.senderPAddr, PADDR);
  printf("          SHA: %s\n", hAddr);
  printf("          SPA: %s\n", pAddr);
  AddrToStr(hAddr, packet.targetHAddr, HADDR);
  AddrToStr(pAddr, packet.targetPAddr, PADDR);
  printf("          THA: %s\n", hAddr);
  printf("          TPA: %s\n", pAddr);
}

void MakeAddr(BYTE* by, BYTE* addr, BYTE size)
{
  int n;
  for (n = 0; n < size; n++)
    addr[n] = by[n];
}

WORD MakeWord(BYTE* by)
{
  return (by[0] << 8) | by[1];
}

DWORD MakeDWord(BYTE* by)
{
  WORD wHi = MakeWord(&by[0]);
  WORD wLo = MakeWord(&by[2]);

  return (wHi << 16) | wLo;
}

void AddrToStr(char* str, BYTE* addr, BYTE type)
{
  switch (type)
  {
    case HADDR:
      sprintf(str, "%02X:%02X:%02X:%02X:%02X:%02X", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
      break;
    
    case PADDR:
      sprintf(str, "%d.%d.%d.%d", addr[0], addr[1], addr[2], addr[3]);
      break;
  }
}

void SIGINTHandler(int sig)
{
  run = 0;
}
