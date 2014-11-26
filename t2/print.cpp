#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <net/if.h>
#include <netinet/ether.h>
#include <linux/ip.h>
#include <arpa/inet.h>
#include "ospf.h"

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

string ospf_to_str(const ospfhdr& ospf)
{
  ostringstream oss;

  oss.fill('0');
  oss << endl;
  oss << blue << "OSPF Packet"  << reset << endl;
  oss << blue << "    Version: " << reset << (int)ospf.ospf_version  << endl;
  oss << blue << "       Type: " << reset << (int)ospf.ospf_type     << endl;
  oss << blue << "     Length: " << reset << ntohs(ospf.ospf_len)    << endl;

  char straddr[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &ospf.ospf_routerid, straddr, sizeof straddr);
  oss << blue << "   RouterId: " << reset << straddr << endl;

  inet_ntop(AF_INET, &ospf.ospf_areaid, straddr, sizeof straddr);
  oss << blue << "     AreaId: " << reset << straddr << endl;
  oss << blue << "   CheckSum: " << reset << "0x" << std::setw(4) << std::hex << ntohs(ospf.ospf_chksum) << endl;
  oss << blue << "   AuthType: " << reset << (int)ospf.ospf_authtype << endl;

  return oss.str();
}

string ospf_hello_to_str(const ospfhdr& ospf)
{
  ostringstream oss;

  oss.fill('0');
  oss << endl;
  oss << blue << "OSPF Hello Packet"  << reset << endl;

  char straddr[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &ospf.ospf_hello.hello_mask, straddr, sizeof straddr);
  oss << blue << "    Network Mask: " << reset << straddr << endl;
  oss << blue << "    Hello interval: " << reset << ntohs(ospf.ospf_hello.hello_helloint) << endl;
  oss << blue << "    Hello Options: " << reset << "0x" << std::setw(2) << std::hex << ntohs(ospf.ospf_hello.hello_options)  << endl;
  oss << blue << "    Router Priority: " << reset << (int)ospf.ospf_hello.hello_priority  << endl;
  oss << blue << "    Router Dead Interval: " << reset << (int32_t)ospf.ospf_hello.hello_deadint << endl;

  inet_ntop(AF_INET, &ospf.ospf_hello.hello_dr, straddr, sizeof straddr);
  oss << blue << "    Designated Router: " << reset << straddr  << endl;
  inet_ntop(AF_INET, &ospf.ospf_hello.hello_bdr, straddr, sizeof straddr);
  oss << blue << "    Backup Designated Router: " << reset << straddr << endl;
  inet_ntop(AF_INET, &ospf.ospf_hello.hello_neighbor[0], straddr, sizeof straddr);
  oss << blue << "    Active Neighbor: " << reset << straddr  << endl;


//struct in_addr hello_mask;
//uint16_t hello_helloint;
//uint8_t hello_options;
//uint8_t hello_priority;
//uint32_t hello_deadint;
//struct in_addr hello_dr;
//struct in_addr hello_bdr;
//struct in_addr hello_neighbor[1];
  return oss.str();
}


string ospf_db_to_str(const ospfhdr& ospf)
{
  ostringstream oss;

  oss.fill('0');
  oss << endl;
  oss << blue << "OSPF Database Description Packet"  << reset << endl;
  oss << blue << "    DB IfMtu: " << reset << (int)ospf.ospf_db.db_ifmtu << endl;
  oss << blue << "    DB Options: " << reset << (int)ospf.ospf_db.db_options  << endl;
  oss << blue << "    DB Flags: " << reset << (int)ospf.ospf_db.db_flags  << endl;
  oss << blue << "    DB Seq: " << reset << (int)ospf.ospf_db.db_seq  << endl;
 // oss << blue << "    DB Lshdr: " << reset <<  << endl;

//uint16_t db_ifmtu;
//uint8_t db_options;
//uint8_t db_flags;
//uint32_t db_seq;
//struct lsa_hdr db_lshdr[1]; /* may repeat	*/

  return oss.str();
}
string ospf_lsr_to_str(const ospfhdr& ospf)
{
  ostringstream oss;

  oss.fill('0');
  oss << endl;
  oss << blue << "OSPF Link State Request Packet"  << reset << endl;
 // oss << blue << "    Options: " << reset << (int)(int)ospf.ospf_hello.hello_options  << endl;

  return oss.str();
}
string ospf_lsu_to_str(const ospfhdr& ospf)
{
  ostringstream oss;

  oss.fill('0');
  oss << endl;
  oss << blue << "OSPF Link State Update Packet"  << reset << endl;
 // oss << blue << "    Options: " << reset << (int)(int)ospf.ospf_hello.hello_options  << endl;


  return oss.str();
}
string ospf_lsa_to_str(const ospfhdr& ospf)
{
  ostringstream oss;

  oss.fill('0');
  oss << endl;
  oss << blue << "OSPF Link State Acknowledgment Packet"  << reset << endl;
 // oss << blue << "    Options: " << reset << (int)(int)ospf.ospf_hello.hello_options  << endl;

  return oss.str();
}













