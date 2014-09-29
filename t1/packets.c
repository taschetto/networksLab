#include <stdio.h>
#include "packets.h"
#include "helpers.h"

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

void MakeIP(BYTE* by, IP* packet)
{
  (*packet).version = HiNibble(&by[0]);
  (*packet).IHL     = LoNibble(&by[0]);
  (*packet).typeOfService = by[1];
  (*packet).totalLength = MakeWord(&by[2]);
  (*packet).identification = MakeWord(&by[4]);
  (*packet).flags = 0;
  (*packet).fragmentOffset = 0;
  (*packet).timeToLive = by[8];
  (*packet).protocol = by[9];
  (*packet).headerChecksum = MakeWord(&by[10]);
  MakeAddr(&by[12], (*packet).sourceAddr, PLEN);
  MakeAddr(&by[16], (*packet).sourceAddr, PLEN);
  (*packet).options = (*packet).IHL > 5 ? MakeDWord(&by[20]) : 0;
}

void PrintIP(IP packet)
{
  printf("--------------- IP PACKET --------------\n");
  printf("        Version: %u\n", packet.version);
  printf("            IHL: %u\n", packet.IHL);
  printf("Type of Service: %u\n", packet.typeOfService);
  printf("   Total Length: %u\n", packet.totalLength);
  printf(" Identification: %u\n", packet.identification);
  printf("          Flags: %02X\n", packet.flags);
  printf("Fragment Offset: %u\n", packet.fragmentOffset);
  printf("   Time to live: %u\n", packet.timeToLive);
  printf("       Protocol: %u\n", packet.protocol);
  printf("Header Checksum: %04X\n", packet.headerChecksum);
  char pAddr[15];
  AddrToStr(pAddr, packet.sourceAddr, PADDR);
  printf(" Source Address: %s\n", pAddr);
  AddrToStr(pAddr, packet.targetAddr, PADDR);
  printf(" Target Address: %s\n", pAddr);
  printf("        Options: %04X\n", packet.options);
}
