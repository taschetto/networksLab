#include <stdio.h>
#include "helpers.h"

void MakeAddr(BYTE* by, BYTE* addr, BYTE size)
{
  int n;
  for (n = 0; n < size; n++)
    addr[n] = by[n];
}

WORD MakeWord(BYTE* by)
{
  return (by[0] << 8) | by[1];
}

DWORD MakeDWord(BYTE* by)
{
  WORD wHi = MakeWord(&by[0]);
  WORD wLo = MakeWord(&by[2]);

  return (wHi << 16) | wLo;
}

void AddrToStr(char* str, BYTE* addr, BYTE type)
{
  switch (type)
  {
    case HADDR:
      sprintf(str, "%02X:%02X:%02X:%02X:%02X:%02X", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
      break;
    
    case PADDR:
      sprintf(str, "%d.%d.%d.%d", addr[0], addr[1], addr[2], addr[3]);
      break;
  }
}

BYTE LoNibble(BYTE* by)
{
  return by[0] & 0x0F;
}

BYTE HiNibble(BYTE* by)
{
  return (by[0] & 0xF0) >> 4;
}

int AreEqual(BYTE* by1, BYTE* by2, BYTE length)
{
  int n;
  for (n = 0; n < length; n++)
  {
    if (by1[n] != by2[n])
      return 0; 
  }

  return 1;
}
