#pragma once

#include <string>
#include "defines.h"

class Ethernet
{
public:
  Ethernet() {}
  Ethernet(BYTE*);
  ~Ethernet();

  int ToBuffer(BYTE*) const;
  std::string ToString() const;

  MAC destination;
  MAC source;
  WORD etherType;
};
