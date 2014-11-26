#pragma once

#include <string>

#include <net/if.h>
#include <netinet/ether.h>
#include <linux/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include "ospf.h"

std::string ifreq_to_str(const ifreq&);
std::string ether_to_str(const ether_header&);
std::string ip_to_str(const iphdr&);
std::string ospf_to_str(const ospfhdr&);
std::string ospf_hello_to_str(const ospfhdr&);
std::string ospf_db_to_str(const ospfhdr&);
std::string ospf_lsr_to_str(const ospfhdr&);
std::string ospf_lsu_to_str(const ospfhdr&);
std::string ospf_lsa_to_str(const ospfhdr&);
