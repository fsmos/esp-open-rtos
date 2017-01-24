#ifndef FSM_ESP_8266
#define FSM_ESP_8266
#include "lwip/pbuf.h"
#include "lwip/netif.h"
#include "netif/etharp.h"

#define ETHTYPE_FSM  0x1996U 

void FSM_ParsePacket(struct pbuf *p,struct netif *netif);
void FSM_SendEthernetPacket(struct netif *netif,char* Data,unsigned short len);
void FSM_Init(struct netif *netif);
void FSM_Main_init(unsigned short id);

struct FSM_ClientBuf
{
   //struct netif *netif;
   unsigned short id;
   struct eth_addr dst;
   char reg;
};

struct FSM_Ethernet_Pack
{
    unsigned char MAC_src[6];
    unsigned char MAC_dst[6];
    unsigned short ethtype;
    char Data[1024];
    
};
#endif