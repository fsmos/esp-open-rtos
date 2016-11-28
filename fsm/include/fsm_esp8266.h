#ifndef FSM_ESP_8266
#define FSM_ESP_8266
#include "lwip/pbuf.h"
#include "lwip/netif.h"

void FSM_ParsePacket(struct pbuf *p,struct netif *netif);
void FSM_SendEthernetPacket(char* Data,unsigned short len);
void FSM_Init(struct netif *netif);

struct FSM_ClientBuf
{
   struct netif *netif;
   unsigned short id;
   unsigned char DstMac[6];
   
};

struct FSM_Ethernet_Pack
{
    unsigned char MAC_src[6];
    unsigned char MAC_dst[6];
    unsigned short ethtype;
    char Data[1024];
    
};
#endif