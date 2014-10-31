#pragma once

typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned int   DWORD;

const BYTE HLEN = 6;
const BYTE PLEN = 4;

typedef BYTE MAC[HLEN];
typedef BYTE IP[PLEN];
