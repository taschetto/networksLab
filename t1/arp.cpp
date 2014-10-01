#include <iomanip>
#include <sstream>

#include "arp.h"
#include "helpers.h"

Arp::Arp(BYTE* by)
{
  hType = MakeWord(&by[0]);
  pType = MakeWord(&by[2]);
  hLen  = by[4];
  pLen  = by[5];
  operation = MakeWord(&by[6]);
  MakeMAC(&by[8], &senderHAddr[0]);
  MakeIP(&by[14], &senderPAddr[0]);
  MakeMAC(&by[18], &targetHAddr[0]);
  MakeIP(&by[24], &targetPAddr[0]);
}

Arp::~Arp()
{
}

std::string Arp::ToString() const
{
  std::ostringstream oss;
  
  oss << "-------------- ARP PACKET --------------" << std::endl;
  oss << "Hardware Type: " << std::to_string(hType) << std::endl;
  oss << "Protocol Type: " << std::setw(4) << std::hex << static_cast<int>(pType) << std::endl;
  oss << " HAddr Length: " << std::to_string(hLen)  << std::endl;
  oss << " PAddr Length: " << std::to_string(pLen)  << std::endl;
  oss << "    Operation: " << std::to_string(operation) << std::endl;
  oss << "          SHA: " << MACToStr(senderHAddr) << std::endl;
  oss << "          SPA: " << IPToStr(senderPAddr)  << std::endl;
  oss << "          THA: " << MACToStr(targetHAddr) << std::endl;
  oss << "          TPA: " << IPToStr(targetPAddr)  << std::endl;
  
  return oss.str();
}
