#ifndef __HELPERS_H__
#define __HELPERS_H__

#include "defines.h"

void MakeAddr(BYTE*, BYTE*, BYTE);
WORD MakeWord(BYTE*);
DWORD MakeDWord(BYTE*);
void AddrToStr(char*, BYTE*, BYTE);

BYTE LoNibble(BYTE*);
BYTE HiNibble(BYTE*);

int AreEqual(BYTE*, BYTE*, BYTE);
#endif
