#pragma once

typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned int   DWORD;

const WORD BUFFSIZE = 1518;

const BYTE HADDR = 0;
const BYTE PADDR = 1;

const BYTE HLEN = 6;
const BYTE PLEN = 4;

const WORD P_IPv4 = 0x0800;
const WORD P_ARP  = 0x0806;

typedef BYTE MAC[HLEN];
typedef BYTE IP[PLEN];
