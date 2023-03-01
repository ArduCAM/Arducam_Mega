#ifndef __HTTP_SEVER_H
#define __HTTP_SEVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lwip/tcp.h"


#include <string.h>

static err_t tcpd_accept(void *arg, struct tcp_pcb *pcb, err_t err);
static err_t tcpd_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p,
                       err_t err);
void httpd_init(void);

#ifdef __cplusplus
}
#endif

#endif /*__HTTP_SEVER_H*/