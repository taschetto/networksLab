#ifndef __PACKETS_H__
#define __PACKETS_H__

#include "defines.h"

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

typedef struct
{
  BYTE version;
  BYTE IHL;
  BYTE typeOfService;
  BYTE totalLength;
  BYTE identification;
  BYTE flags;
  WORD fragmentOffset;
  BYTE timeToLive;
  BYTE protocol;
  WORD headerChecksum;
  BYTE sourceAddr[PLEN];
  BYTE targetAddr[PLEN];
  DWORD options;
} IP;

void MakeEthernet(BYTE*, Ethernet*);
void PrintEthernet(Ethernet);

void MakeARP(BYTE*, ARP*);
void PrintARP(ARP);

void MakeIP(BYTE*, IP*);
void PrintIP(IP);

#endif
