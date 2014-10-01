#include <iomanip>
#include <sstream>

#include "arp.h"
#include "colors.h"
#include "helpers.h"

using namespace Colors;

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
  
  oss.fill('0');
  oss << blue << "# ARP PACKET #" << std::endl;
  oss << blue << "    HType: " << reset << std::to_string(hType) << "\t\t\t";
  oss << blue << "Length: " << reset << std::to_string(hLen)  << std::endl;
  oss << blue << "    PType: " << reset << std::setw(4) << std::hex << static_cast<int>(pType) << "\t\t\t";
  oss << blue << "Length: " << reset << std::to_string(pLen)  << std::endl;
  oss << blue << "Operation: " << reset << std::to_string(operation) << std::endl;
  oss << blue << "      SHA: " << reset << MACToStr(senderHAddr) << "\t";
  oss << blue << "   THA: " << reset << MACToStr(targetHAddr) << std::endl;
  oss << blue << "      SPA: " << reset << IPToStr(senderPAddr)  << "\t";
  oss << blue << "   TPA: " << reset << IPToStr(targetPAddr)  << std::endl;
  
  return oss.str();
}
