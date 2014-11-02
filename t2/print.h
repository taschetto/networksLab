#pragma once

#include <string>

#include <net/if.h>
#include <netinet/ether.h>
#include <linux/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>

std::string ifreq_to_str(const ifreq&);
std::string ether_to_str(const ether_header&);
std::string ip_to_str(const iphdr&);
