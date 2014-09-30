#include <iomanip>
#include <sstream>
#include <string>
#include "helpers.h"

void MakeMAC(const BYTE* by, BYTE* addr)
{
  for (int n = 0; n < HLEN; n++)
    addr[n] = by[n];
}

void MakeIP(const BYTE* by, BYTE* addr)
{
  for (int n = 0; n < PLEN; n++)
    addr[n] = by[n];
}

WORD MakeWord(const BYTE* by)
{
  return (by[0] << 8) | by[1];
}

DWORD MakeDWord(const BYTE* by)
{
  WORD wHi = MakeWord(&by[0]);
  WORD wLo = MakeWord(&by[2]);

  return (wHi << 16) | wLo;
}

std::string MACToStr(const BYTE mac[])
{
  std::ostringstream oss;
  char oldFill = oss.fill('0');

  oss << std::setw(2) << std::hex << static_cast<unsigned int>(mac[0] & 0xFF);
  for (int n = 1; n < HLEN; n++)
    oss << ':' << std::setw(2) << std::hex << static_cast<unsigned int>(mac[n] & 0xFF);

  oss.fill(oldFill);;

  return oss.str();
}

std::string IPToStr(const BYTE ip[])
{
  std::ostringstream oss;

  oss << static_cast<unsigned int>(ip[0]);
  for (int n = 1; n < PLEN; n++)
    oss << '.' << static_cast<unsigned int>(ip[n]);

  return oss.str();
}

BYTE LoNibble(const BYTE* by)
{
  return by[0] & 0x0F;
}

BYTE HiNibble(const BYTE* by)
{
  return (by[0] & 0xF0) >> 4;
}

bool CompareIP(const BYTE* ip1, const BYTE* ip2)
{
  int n;
  for (n = 0; n < PLEN; n++)
  {
    if (ip1[n] != ip2[n])
      return false;
  }

  return true;
}
