#include <csignal>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>

#include "interface.h"
#include "helpers.h"

int initSignal(void (*handler)(int))
{
  std::cout << "Capture SIGINT...";

  if (signal(SIGINT, handler) == SIG_ERR)
  {
    error();
    return -1;
  }

  ok();
  return 0;
}

int initSocket(int& sockd)
{
  std::cout << "Create socket...";

  sockd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  if (sockd < 0)
  {
    error();
    return sockd;
  }

  ok();
  return 0;
}

int closeSocket(int& sockd)
{
  std::cout << "Close socket...";
  int ret = close(sockd);
  if (ret < 0)
  {
    error();
    return ret;
  }

  ok();
  return 0;
}

int initInterface(const int& socket, ifreq& ifr)
{
  std::cout << "Retrieve interface index...";

  if (ioctl(socket, SIOCGIFINDEX, &ifr) < 0)
  {
    error();
    return -1;
  }

  ok();
  std::cout << "Retrieve interface hardware address...";

  if (ioctl(socket, SIOCGIFHWADDR, &ifr) < 0)
  {
    error();
    return -1;
  }

  ok();
  std::cout << "Retrieve interface protocol address...";

  if (ioctl(socket, SIOCGIFADDR, &ifr) < 0)
  {
    error();
    return -1;
  }

  ok();
  std::cout << "Retrieve interface flags... ";

  if (ioctl(socket, SIOCGIFFLAGS, &ifr) < 0)
  {
    error();
    return -1;
  }

  ok();
  std::cout << "Set interface to PROMISCUOUS mode... ";

  ifr.ifr_flags |= IFF_PROMISC;

  if (ioctl(socket, SIOCSIFFLAGS, &ifr) < 0)
  {
    error();
    return -1;
  }

  ok();

  return 0;
}

int resetInterface(const int socket, ifreq& ifr)
{
  ifr.ifr_flags &= ~IFF_PROMISC;  
  std::cout << "Unset interface from PROMISCUOUS mode...";

  if (ioctl(socket, SIOCSIFFLAGS, &ifr) < 0)
  {
    error();
    return -1;
  }

  ok();

  return 0;
}
