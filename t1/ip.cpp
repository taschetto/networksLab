#include <iomanip>
#include <sstream>

#include "ip.h"
#include "helpers.h"

Ip::Ip(BYTE* by)
{
  version = HiNibble(&by[0]);
  IHL     = LoNibble(&by[0]);
  typeOfService = by[1];
  totalLength = MakeWord(&by[2]);
  identification = MakeWord(&by[4]);
  flags = 0;
  fragmentOffset = 0;
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
  
  oss << "--------------- IP PACKET --------------"           << std::endl;
  oss << "        Version: " << std::to_string(version)       << std::endl;
  oss << "            IHL: " << std::to_string(IHL)           << std::endl;
  oss << "Type of Service: " << std::to_string(typeOfService) << std::endl;
  oss << "   Total Length: " << std::to_string(totalLength)   << std::endl;
  oss << " Identification: " << std::to_string(identification) << std::endl;
  oss << "          Flags: " << std::setw(4) << std::hex << static_cast<int>(flags) << std::endl;
  oss << "Fragment Offset: " << std::to_string(fragmentOffset) << std::endl;
  oss << "   Time to live: " << std::to_string(timeToLive)    << std::endl;
  oss << "       Protocol: " << std::to_string(protocol)      << std::endl;
  oss << "Header Checksum: " << std::setw(4) << std::hex << static_cast<int>(headerChecksum) << std::endl;
  oss << " Source Address: " << IPToStr(sourceAddr)           << std::endl;
  oss << " Target Address: " << IPToStr(targetAddr)           << std::endl;
  oss << "        Options: " << std::setw(4) << std::hex << static_cast<int>(options) << std::endl;
  
  return oss.str();
}
