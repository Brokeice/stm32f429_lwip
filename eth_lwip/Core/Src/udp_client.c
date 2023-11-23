#include "udp_client.h"

/* 定义端口号 */
#define UDP_REMOTE_PORT    7 /* 远端端口 */
#define UDP_LOCAL_PORT     7 /* 本地端口 */

/* udp控制块 */
static struct udp_pcb *upcb_client;

/******************************************************************************
 * 描述  : 接收回调函数
 * 参数  : -
 * 返回  : 无
******************************************************************************/
static void udp_client_receive_callback(void *arg, struct udp_pcb *upcb,struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    uint32_t i;

    /* 数据回传 */
    udp_send(upcb, p);//回传设备本身
//    udp_sendto(upcb, p, addr, port);//

    /***打印端口***/
	  printf("ip_server:%d:%d:%d:%d port:%d:\n",
        *((uint8_t *)&addr->addr), *((uint8_t *)&addr->addr + 1),
        *((uint8_t *)&addr->addr + 2), *((uint8_t *)&addr->addr + 3), port);

	  /* 打印接收到的数据 */
    if (p != NULL)
    {
        struct pbuf *ptmp = p;

        while(ptmp != NULL)
        {
            for (i = 0; i < p->len; i++)
            {
                printf("%c", *((char *)p->payload + i));
            }
            ptmp = p->next;
        }
        printf("\r\n");
    }

    /* 释放缓冲区数据 */
    pbuf_free(p);
}

/******************************************************************************
 * 描述  : 发送udp数据
 * 参数  : (in)pData 发送数据的指针
 * 返回  : 无
******************************************************************************/
void udp_client_send(char *pData)
{
    struct pbuf *p;

    /* 分配缓冲区空间 */
    p = pbuf_alloc(PBUF_TRANSPORT, strlen(pData), PBUF_POOL);

    if (p != NULL)
    {
        /* 填充缓冲区数据 */
        pbuf_take(p, pData, strlen(pData));

        /* 发送udp数据 */
        udp_send(upcb_client, p);

        /* 释放缓冲区空间 */
        pbuf_free(p);
    }
}

/******************************************************************************
 * 描述  : 创建udp客户端
 * 参数  : 无
 * 返回  : 无
******************************************************************************/
void udp_client_init(void)
{
    ip_addr_t serverIP;
    err_t err;

    IP4_ADDR(&serverIP, 192, 168, 28, 100);

    /* 创建udp控制块 */
    upcb_client = udp_new();

    if (upcb_client!=NULL)
    {
        /* 配置本地端口 */
        upcb_client->local_port = UDP_LOCAL_PORT;

        /* 配置服务器IP和端口 */
        err= udp_connect(upcb_client, &serverIP, UDP_REMOTE_PORT);

        if (err == ERR_OK)
        {
            /* 注册接收回调函数 */
            udp_recv(upcb_client, udp_client_receive_callback, NULL);

            /* 发送udp数据 */
            udp_client_send((char *)"udp client connected");
            printf("udp client connected\r\n");
        }
        else
        {
            udp_remove(upcb_client);
            printf("can not connect udp pcb\r\n");
        }
    }
}

///******************************** END OF FILE ********************************/


