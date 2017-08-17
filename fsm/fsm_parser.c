#include "fsm_esp8266.h"
#include "fcmprotocol.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lwip/def.h"
#include "netif/etharp.h"
#include "lwip/pbuf.h"

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
    printf("New Packet");
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
fsm_send_ip(struct netif *netif, char* datain,unsigned short len,const struct eth_addr *ethsrc_addr, const struct eth_addr *ethdst_addr)
{

	struct pbuf *p;
	err_t result = ERR_OK;
	struct eth_hdr *ethhdr;
	char *data;
	LWIP_ASSERT("netif != NULL", netif != NULL);
	 /* Выделяем буфер */
	p = pbuf_alloc(PBUF_RAW, SIZEOF_ETH_HDR+len, PBUF_RAM);
	/* could allocate a pbuf for an ARP request? */
	if (p == NULL) {
	LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_SERIOUS, ("etharp_raw: could not allocate pbuf for ARP request.\n"));
	return ERR_MEM;
	}
	LWIP_ASSERT("Check pbuf", (p->len >= SIZEOF_ETHARP_PACKET));
 
	ethhdr = (struct eth_hdr *)p->payload;
	data = (struct etharp_hdr *)((u8_t*)ethhdr + SIZEOF_ETH_HDR);
	memcpy(data,datain,len);


	/* Пишем MAC-Адресс*/
	ETHADDR16_COPY(&ethhdr->dest, ethdst_addr);
	ETHADDR16_COPY(&ethhdr->src, ethsrc_addr);
    ethhdr->type = htons(0x1996);
	/*  Отправка FSM пакета*/
	result = netif->linkoutput(netif, p);
	/* free ARP query packet */
	pbuf_free(p);
	p = NULL;
 
	return result;
}

void FSM_SendEthernetPacket(struct netif *netif,char* Data,unsigned short len)
{
    err_t err;
    printf("FSM Send Start\n");
    err = fsm_send_ip(netif, Data,len, (struct eth_addr*)(netif->hwaddr), &fsm_clbuf.dst);
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
    fsm_devreg.type=7;
    fsm_devreg.KodDevice = 1;
    fsm_devreg.PodVidDevice = 1;
    fsm_devreg.VidDevice = 1;
    memset(&fsm_clbuf.dst.addr,0xFF,ETHARP_HWADDR_LEN);
}