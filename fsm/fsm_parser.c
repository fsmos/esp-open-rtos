#include "fsm_esp8266.h"
#include <string.h>
struct FSM_ClientBuf fsm_clbuf;

//static err_t low_level_output(struct netif *netif, struct pbuf *p);

void FSM_ParsePacket(struct pbuf *p,struct netif *netif)
{
    
}

void FSM_SendEthernetPacket(char* Data,unsigned short len)
{
    struct pbuf *p;
    struct FSM_Ethernet_Pack* eth_p;
    p = pbuf_alloc(PBUF_RAW, 14+len, PBUF_RAM);
    eth_p=((struct FSM_Ethernet_Pack*)p->payload);
    memcpy(eth_p->MAC_dst,fsm_clbuf.DstMac,6);
    memcpy(eth_p->MAC_src,fsm_clbuf.netif->hwaddr,fsm_clbuf.netif->hwaddr_len);
    eth_p->ethtype=0x1996;
    memcpy(eth_p->Data,Data,len);
    fsm_clbuf.netif->linkoutput(fsm_clbuf.netif,p);
}
void FSM_Init(struct netif *netif)
{
   fsm_clbuf.netif=netif;
    
}