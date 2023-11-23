#ifndef __ECHO_H__
#define __ECHO_H__

#include "main.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/igmp.h"
#include "lwipopts.h"

extern struct udp_pcb *upcb_server;
extern uint8_t tcp_demo_sendbuf[80];

void udp_server_init(void);
void udp_demo_senddata(struct udp_pcb *upcb);

#endif

