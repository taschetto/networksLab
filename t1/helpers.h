#pragma once

#include <string>
#include "defines.h"

void MakeMAC(const BYTE*, BYTE*);
void MakeIP(const BYTE*, BYTE*);

WORD MakeWord(const BYTE*);
DWORD MakeDWord(const BYTE*);

std::string AddrToStr(const BYTE*, BYTE);

std::string MACToStr(const BYTE*);
std::string IPToStr(const BYTE*);

BYTE LoNibble(const BYTE*);
BYTE HiNibble(const BYTE*);

int AreEqual(const BYTE*, const BYTE*, const BYTE);
