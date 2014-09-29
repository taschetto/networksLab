#pragma once

#include <string>
#include "defines.h"

class Ip
{
public:
  Ip(BYTE*);
  ~Ip();
  
  std::string ToString() const;
  
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
};
