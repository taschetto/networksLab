#include <iomanip>
#include <sstream>

#include "colors.h"
#include "ip.h"
#include "helpers.h"

using namespace Colors;

Ip::Ip(BYTE* by)
{
  version = HiNibble(&by[0]);
  IHL     = LoNibble(&by[0]);
  typeOfService = by[1];
  totalLength = MakeWord(&by[2]);
  identification = MakeWord(&by[4]);
  flags = (by[6] & 0xe0) >> 5;
  fragmentOffset = MakeWord(&by[6]) & 0x1FFF;
  timeToLive = by[8];
  protocol = by[9];
  headerChecksum = MakeWord(&by[10]);
  MakeIP(&by[12], &sourceAddr[0]);
  MakeIP(&by[16], &targetAddr[0]);
  options = IHL > 5 ? MakeDWord(&by[20]) : 0;
}

Ip::~Ip()
{
}

std::string Ip::ToString() const
{
  std::ostringstream oss;

  oss.fill('0');
  
  oss << blue << "# IP PACKET #" << std::endl;
  
  oss << blue << "Version: "  << reset << std::to_string(version)       << "\t";
  oss << blue << "Type: "     << reset << std::to_string(typeOfService) << "\t\t";
  oss << blue << "Length: "   << reset << std::to_string(totalLength)   << std::endl;

  oss << blue << "Ident: "    << reset << std::to_string(identification) << "\t";
  oss << blue << "Flags: "    << reset << std::setw(4) << std::hex << static_cast<int>(flags) << "\t";
  oss << blue << "Frag Off: " << reset <<  std::to_string(fragmentOffset) << std::endl;

  oss << blue << "TTL: "      << reset << std::to_string(timeToLive)    << "\t\t";
  oss << blue << "Protocol: " << reset << std::to_string(protocol)      << "\t";
  oss << blue << "Chksum: "   << reset << std::setw(4) << std::hex << static_cast<int>(headerChecksum) << std::endl;

  oss << blue << "Source Addr: " << reset << IPToStr(sourceAddr)           << "\t";
  oss << blue << "Target Addr: " << reset << IPToStr(targetAddr)           << std::endl;
  
  return oss.str();
}
