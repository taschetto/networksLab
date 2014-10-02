#include <cstring>
#include <iomanip>
#include <sstream>
#include <string>

#include "colors.h"
#include "ethernet.h"
#include "helpers.h"

using namespace Colors;

Ethernet::Ethernet(BYTE* by)
{
  MakeMAC(&by[0], &destination[0]);
  MakeMAC(&by[6], &source[0]);
  etherType = MakeWord(&by[12]);
}

Ethernet::~Ethernet()
{
}

int Ethernet::ToBuffer(BYTE* buffer) const
{
  int index = 0;
  memcpy(&buffer[index], &destination, HLEN);  index += HLEN;
  memcpy(&buffer[index], &source, HLEN);       index += HLEN;
  buffer[index++] = HiByte(etherType);
  buffer[index++] = LoByte(etherType);
  return index;
}

std::string Ethernet::ToString() const
{
  std::ostringstream oss;

  oss.fill('0');
  oss << blue << "# Ethernet Packet #" << std::endl;
  oss << blue << "Destination MAC: " << reset << MACToStr(destination) << std::endl;
  oss << blue << "     Source MAC: " << reset << MACToStr(source)      << std::endl;
  oss << blue << "     Ether Type: " << reset << std::setw(4) << std::hex << etherType << std::endl;

  return oss.str();
}
