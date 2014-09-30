#include <iomanip>
#include <sstream>
#include <string>
#include "ethernet.h"
#include "helpers.h"

Ethernet::Ethernet(BYTE* by)
{
  MakeMAC(&by[0], &destination[0]);
  MakeMAC(&by[6], &source[0]);
  etherType = MakeWord(&by[12]);
}

Ethernet::~Ethernet()
{
}

std::string Ethernet::ToString() const
{
  std::ostringstream oss;
  oss << "------------ ETHERNET PACKET -----------"   << std::endl;
  oss << "Destination MAC: " << MACToStr(destination) << std::endl;
  oss << "     Source MAC: " << MACToStr(source)      << std::endl;
  oss << "     Ether Type: " << std::setw(4) << std::hex << etherType << std::endl;

  return oss.str();
}
