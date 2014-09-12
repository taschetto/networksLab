#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <net/if.h>             // ifr struct
#include <netinet/ether.h>      // header ethernet
#include <netinet/in.h>         // protocol definitions
#include <arpa/inet.h>          // manipulação de endereços IP
#include <netinet/in_systm.h>   // tipos de dados (???)

#define BUFFSIZE 1518

int createSocket();

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    printf("Usage: spoofer INTERFACE\n");
    exit(-1); 
  }

  int sockd;
  if ((sockd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
  {
    printf("Erro na criação do socket (%s).\n", strerror(errno));
    exit(-1);
  }

  return 0;
}

