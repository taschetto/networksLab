#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <net/if.h>
#include <netinet/ether.h>
#include <linux/ip.h>
#include <netinet/udp.h>

#include "print.h"
#include "colors.h"

using namespace std;
using namespace Colors;

string ifreq_to_str(const ifreq& ifr)
{
  ostringstream oss;

  oss << blue << "Ready to capture from: " << reset << ifr.ifr_name << endl;
  oss << blue << "      Interface Index: " << reset << (int)ifr.ifr_ifindex << endl;
  oss << blue << "        Hardware Addr: " << reset << ether_ntoa((struct ether_addr*)ifr.ifr_hwaddr.sa_data) << endl;
  oss << blue << "        Protocol Addr: " << reset << inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr) << endl;
  
  return oss.str();
}

string ether_to_str(const ether_header& ether)
{
  ostringstream oss;

  oss.fill('0');
  oss << endl;
  oss << blue << "Ethernet Packet" << reset << endl;
  oss << blue << "Destination: " << reset << ether_ntoa((struct ether_addr*)ether.ether_dhost) << endl;
  oss << blue << "        Src: " << reset << ether_ntoa((struct ether_addr*)ether.ether_shost) << endl;
  oss << blue << "       Type: " << reset << "0x" << std::setw(4) << std::hex << ntohs(ether.ether_type) << endl;

  return oss.str();
}

string ip_to_str(const iphdr& ip)
{
  ostringstream oss;

  oss.fill('0');
  oss << endl;
  oss << blue << "IP Packet" << reset << endl;
  oss << blue << "        IHL: " << reset << std::dec << (int)ip.ihl << endl;
  oss << blue << "    Version: " << reset << (int)ip.version << endl;
  oss << blue << "        TOS: " << reset << "0x" << (int)ip.tos << endl;
  oss << blue << "        LEN: " << reset << ntohs(ip.tot_len) << endl;
  oss << blue << "         ID: " << reset << ntohs(ip.id) << endl;
  oss << blue << "   Frag Off: " << reset << ntohs(ip.frag_off) << endl;
  oss << blue << "        TTL: " << reset << (int)ip.ttl << endl;
  oss << blue << "   Protocol: " << reset << (int)ip.protocol << endl;
  oss << blue << "      Check: " << reset << "0x" << std::setw(4) << std::hex << ntohs(ip.check) << endl;
  
  char straddr[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &ip.saddr, straddr, sizeof straddr);
  oss << blue << "     Source: " << reset << straddr << endl;

  inet_ntop(AF_INET, &ip.daddr, straddr, sizeof straddr);
  oss << blue << "Destination: " << reset << straddr << endl;

  return oss.str();
}
