#pragma once

#include "defines.h"
#include <string>

class Ethernet
{
public:
  Ethernet(BYTE*);
  ~Ethernet();

  std::string ToString() const;

  MAC destination;
  MAC source;
  DWORD etherType;
};
