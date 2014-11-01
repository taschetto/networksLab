#pragma once

#include <net/if.h>

int initSignal(void (*handler)(int));
int initSocket(int&);
int closeSocket(int&);
int initInterface(const int&, ifreq&);
int resetInterface(const int, ifreq&);
