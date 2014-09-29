#pragma once

#include <string>
#include "defines.h"

class Arp
{
public:
	Arp(BYTE*);
	~Arp();

	std::string ToString() const;

	WORD hType;
	WORD pType;
	BYTE hLen;
	BYTE pLen;
	WORD operation;
	BYTE senderHAddr[HLEN];
	BYTE senderPAddr[PLEN];
	BYTE targetHAddr[HLEN];
	BYTE targetPAddr[PLEN];
};
