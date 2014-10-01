#include <sys/types.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <net/if.h>             // ifr struct
#include <netinet/ether.h>      // header ethernet
#include <netinet/in.h>         // protocol definitions
#include <arpa/inet.h>          // manipulação de endereços IP
#include <netinet/in_systm.h>   // tipos de dados (???)

#include <csignal>
#include <cstring>
#include <iostream>
#include <mutex>
#include <thread>

#include "colors.h"
#include "helpers.h"
#include "ethernet.h"
#include "arp.h"
#include "ip.h"

using namespace Colors;

void SIGINTHandler(int);

std::mutex runMutex;
int run = 1;

void attack(const Arp& reply)
{
  Ethernet ethernet;
  memcpy(ethernet.destination, reply.targetHAddr, HLEN);
  memcpy(ethernet.source, reply.senderHAddr, HLEN);
  ethernet.etherType = 0x0806;

  std::cout << red << "Hello from thread ATTACK!" << reset << std::endl;
  std::cout << ethernet.ToString() << std::endl;
  std::cout << reply.ToString() << std::endl;
  
  while(run)
  {
    runMutex.lock();
    std::cout << yellow << '.' << reset;
    runMutex.unlock();
  }
}

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cerr << "Usage: spoofer <interface>" << std::endl;
    exit(-1); 
  }

  BYTE intMac[HLEN];
  BYTE intIp[PLEN];
  std::thread* attackThread = nullptr;

  std::cout << "Capture SIGINT...";

  if (signal(SIGINT, SIGINTHandler) == SIG_ERR)
  {
    error();
    exit(-1);
  }

  ok();
  std::cout << "Create socket...";

  int sockd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  if (sockd < 0)
  {
    error();
    close(sockd);
    exit(-1);
  }

  ok(); 
  std::cout << "Retrieve interface index...";

  struct ifreq ifr;
	strcpy(ifr.ifr_name, argv[1]);

	if (ioctl(sockd, SIOCGIFINDEX, &ifr) < 0)
  {
    error();
    close(sockd);
    exit(-1);
  }

  ok();
  std::cout << "Retrieve interface hardware address...";

  if (ioctl(sockd, SIOCGIFHWADDR, &ifr) < 0)
  {
    error();
    close(sockd);
    exit(-1);
  }

  MakeMAC((BYTE*)&ifr.ifr_hwaddr.sa_data[0], &intMac[0]);

  ok();
  std::cout << "Retrieve interface protocol address...";

  if (ioctl(sockd, SIOCGIFADDR, &ifr) < 0)
  {
    error();
    close(sockd);
    exit(-1);
  }

  MakeIP((BYTE*)&ifr.ifr_addr.sa_data[2], &intIp[0]);

  ok();
  std::cout << "Retrieve interface flags... ";

	if (ioctl(sockd, SIOCGIFFLAGS, &ifr) < 0)
  {
    error();
    close(sockd);
    exit(-1);
  }

  ok();
  std::cout << "Set interface to PROMISCUOUS mode... ";

	ifr.ifr_flags |= IFF_PROMISC;

	if (ioctl(sockd, SIOCSIFFLAGS, &ifr) < 0)
  {
    error();
    close(sockd);
    exit(-1);
  }

  ok();
  std::cout << std::endl << blue << "Capturing interface " << ifr.ifr_name << " (" << MACToStr(intMac) << " :: " << IPToStr(intIp) << ")..." << reset << std::endl;
 
	unsigned char buff[BUFFSIZE];

  while (run) 
  {
    recv(sockd,(char *) &buff, sizeof(buff), 0x00);

    Ethernet ethernet(&buff[0]);

    if (ethernet.etherType == P_ARP)
    {
      Arp arp(&buff[14]);

      //if (arp.operation == 1 && !CompareIP(arp.targetPAddr, intIp))
      {
        std::cout << blue << "Received ARP Request:" << reset << std::endl << arp.ToString() << std::endl;

        Arp reply = arp;
        reply.operation = 2;

        memcpy(reply.targetHAddr, reply.senderHAddr, HLEN);
        memcpy(reply.senderHAddr, intMac, HLEN);

        BYTE ipSwap[PLEN];

        memcpy(ipSwap, reply.targetPAddr, PLEN);
        memcpy(reply.targetPAddr, reply.senderPAddr, PLEN);
        memcpy(reply.senderPAddr, ipSwap, PLEN);

        attackThread = new std::thread(attack, reply);
        break;
      }
    }
	}

  std::cout << yellow << "Waiting for attack thread to terminate..." << reset << std::endl;

  attackThread->join();

	ifr.ifr_flags &= ~IFF_PROMISC;  
  std::cout << "Unset interface from PROMISCUOUS mode...";

  if (ioctl(sockd, SIOCSIFFLAGS, &ifr) < 0)
  {
    error();
    close(sockd);
    exit(-1);
  }

  ok();
  close(sockd);

  return 0;
}

void SIGINTHandler(int sig)
{
  std::cout << yellow << "Received signal to interrupt execution. Terminating...\n\n" << reset;
  runMutex.lock();
  run = 0;
  runMutex.unlock();
}

