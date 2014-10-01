#pragma once

#include <string>
#include "defines.h"

class Ethernet
{
public:
  Ethernet() {}
  Ethernet(BYTE*);
  ~Ethernet();

  std::string ToString() const;

  MAC destination;
  MAC source;
  DWORD etherType;
};
