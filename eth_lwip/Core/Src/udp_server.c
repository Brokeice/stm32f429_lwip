#include "udp_server.h"
#include <string.h>

#define UDP_SERVER_PORT    7   /* define the UDP local connection port (电脑ip端口) */
#define UDP_CLIENT_PORT    7   /* define the UDP remote connection port(设备ip端口) */

struct udp_pcb *upcb_server;
uint8_t tcp_demo_sendbuf[80]="udp_test2022\n";

void udp_server_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);

void udp_server_init(void)
{
   err_t err;
   
   /* Create a new UDP control block  */
   upcb_server = udp_new();
   
   if (upcb_server)
   {
     /* Bind the upcb to the UDP_PORT port */
     /* Using IP_ADDR_ANY allow the upcb to be used by any local interface */
      err = udp_bind(upcb_server, IP_ADDR_ANY, UDP_SERVER_PORT);//连接指定IP,指定端口
      
      if(err == ERR_OK)
      {
        /* Set a receive callback for the upcb */
        udp_recv(upcb_server, udp_server_receive_callback, NULL);//注册接收回调函数，只要接收到数据，这个回调函数会被lwip内核调用
      }
      else
      {
        udp_remove(upcb_server);//断开UDP连接 
      }
   }
}

void udp_server_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{

  /* Connect to the remote client */
  udp_connect(upcb, addr, UDP_CLIENT_PORT);
    
  /* Tell the client that we have accepted it */
  udp_send(upcb, p);

  /* free the UDP connection, so we can accept new clients */
  udp_disconnect(upcb);
	
  /* Free the p buffer */
  pbuf_free(p);
   
}

void udp_demo_senddata(struct udp_pcb *upcb)
{
	struct pbuf *ptr;
  ip_addr_t clientIP;

  IP4_ADDR(&clientIP, 192, 168, 28, 100);
	ptr=pbuf_alloc(PBUF_TRANSPORT,strlen((char*)tcp_demo_sendbuf),PBUF_POOL); //申请内存
	if(ptr)
	{
		/* Connect to the remote client */
    udp_connect(upcb, &clientIP, UDP_CLIENT_PORT);
    pbuf_take(ptr,(char*)tcp_demo_sendbuf,strlen((char*)tcp_demo_sendbuf)); //将tcp_demo_sendbuf中的数据打包进pbuf结构中
		udp_send(upcb,ptr);	//udp发送数据 
    /* free the UDP connection, so we can accept new clients */
    udp_disconnect(upcb);
		pbuf_free(ptr);//释放内存
	} 
	
} 

