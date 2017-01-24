#include "fsm_esp8266.h"
#include "fcmprotocol.h"
#include "FreeRTOS.h"
#include "task.h"

#include <string.h>
struct FSM_ClientBuf fsm_clbuf;
struct FSM_DeviceRegistr fsm_devreg;
//static err_t low_level_output(struct netif *netif, struct pbuf *p);
void FSM_Process(void *pvParameters)
{
    
    // DHT sensors that come mounted on a PCB generally have
    // pull-up resistors on the data pin.  It is recommended
    // to provide an external pull-up resistor otherwise...
    while(1) {
        printf("FSM Send\n");
        if(fsm_clbuf.reg == 0) FSM_SendEthernetPacket(pvParameters,(char*)&fsm_devreg,sizeof(struct FSM_DeviceRegistr));
        // Three second delay...
        vTaskDelay(1000);
    }
}
void FSM_ParsePacket(struct pbuf *p,struct netif *netif)
{
    
}

/*etharp_send_ip(struct netif *netif, struct pbuf *p, struct eth_addr *src, struct eth_addr *dst)
{
  struct eth_hdr *ethhdr = (struct eth_hdr *)p->payload;

  LWIP_ASSERT("netif->hwaddr_len must be the same as ETHARP_HWADDR_LEN for etharp!",
              (netif->hwaddr_len == ETHARP_HWADDR_LEN));
  ETHADDR32_COPY(&ethhdr->dest, dst);
  ETHADDR16_COPY(&ethhdr->src, src);
  ethhdr->type = PP_HTONS(ETHTYPE_IP);
  LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_send_ip: sending packet %p\n", (void *)p));
  return netif->linkoutput(netif, p);
}
 * */
static err_t
fsm_send_ip(struct netif *netif, struct pbuf *p, struct eth_addr *src, struct eth_addr *dst)
{
  struct eth_hdr *ethhdr = (struct eth_hdr *)p->payload;
  ETHADDR32_COPY(&ethhdr->dest, dst);
  ETHADDR16_COPY(&ethhdr->src, src);
  ethhdr->type = PP_HTONS(0x1996);
  /* send the packet */
  return netif->linkoutput(netif, p);
}

void FSM_SendEthernetPacket(struct netif *netif,char* Data,unsigned short len)
{
    err_t err;
    printf("FSM Send Start\n");
    struct pbuf *p;
    p = pbuf_alloc(PBUF_RAW, len, PBUF_RAM);
    memcpy(p->payload,Data,len);
    err = fsm_send_ip(netif, p, (struct eth_addr*)(netif->hwaddr), &fsm_clbuf.dst);
    pbuf_free(p);
    printf("FSM Send Done %i\n",err);
}
void FSM_Init(struct netif *netif)
{
   //fsm_clbuf.netif=netif;
     xTaskCreate(FSM_Process, "FSM_Process", 256, netif, 2, NULL);
}
void FSM_Main_init(unsigned short id)
{
    fsm_clbuf.id = id;
    fsm_clbuf.reg = 0;
    fsm_devreg.IDDevice = id;
    fsm_devreg.opcode=RegDevice;
    memset(&fsm_clbuf.dst.addr,0xFF,ETHARP_HWADDR_LEN);
}