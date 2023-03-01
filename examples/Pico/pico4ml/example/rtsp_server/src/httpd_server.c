#include "http_server.h"
#include "pico/cyw43_arch.h"
#include "stdint.h"

#include "lwip/pbuf.h"
#include "lwip/tcp.h"


#include "sample.h"

#define TCP_PORT        8080
#define DEBUG_printf    printf
#define BUF_SIZE        2048
#define TEST_ITERATIONS 10
#define POLL_TIME_S     5

err_t tcp_server_send_data(void *arg, struct tcp_pcb *tpcb, const void *dataptr,
                           u16_t len);

typedef struct TCP_SERVER_T_ {
    struct tcp_pcb *server_pcb;
    struct tcp_pcb *client_pcb;
    bool complete;
    uint8_t buffer_sent[BUF_SIZE];
    uint8_t buffer_recv[BUF_SIZE];
    int sent_len;
    int recv_len;
    int run_count;
} TCP_SERVER_T;

/*
 * 函数名：http_recv
 * 描述  ：http接收到数据后的回调函数
 * 输入  ：tcp_arg设置的参数、tcp_pcb、pcb、err
 * 输出  ：err
 * 调用  ：内部调用
 */
const uint8_t home_page[] = "HTTP/1.1 200 OK\r\nContent-Length: "
                            "13\r\nConnection: close\r\n\r\nHello, world!";

static err_t http_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p,
                       err_t err) {
    char *data = NULL;
    char *UserName = NULL;
    char *PassWord = NULL;
    char *LED_CMD = NULL;
    char *ch = NULL;

    data = p->payload; // 把接收到的数据指针交给data
    DEBUG_printf("/n[rec]: %s\n", data);
    cyw43_arch_lwip_check();

    if (err == ERR_OK && p != NULL) // 判断是否非空
    {
        DEBUG_printf("1");
        if (strncmp(data, "GET", 3) == 0) {
                DEBUG_printf("1");

            // tcp_write(pcb, http_html_hdr, sizeof(http_html_hdr), 0);
            // tcp_write(pcb, login, sizeof(login), 0); /* 发送登录网页信息 */
            // if (strncmp(data + 3, "/ ", 2) == 0) {
                // const uint8_t response[] =
                //     "HTTP/1.1 200 OK\r\nContent-disposition: inline; "
                //     "filename=capture.jpg\r\nContent-type: image/jpeg\r\n\r\n";
                // tcp_write(pcb, response, sizeof(response), 0);
                // tcp_write(pcb, capture_jpg, capture_jpg_len,
                //           TCP_WRITE_FLAG_COPY);
            // } else if (strncmp(data + 3, "/jpeg ", 2) == 0) {
            //     /* code */
                tcp_write(pcb, home_page, sizeof(home_page), 0);
                tcp_output(pcb);
            // }
        }
        // err_t err = tcp_write(tpcb, dataptr, len, 0);
        // if (err != ERR_OK) {
        //     DEBUG_printf("Failed to write data %d\n", err);
        //     return tcp_server_result(arg, -1);
        // }
        pbuf_free(p); /* 释放该pbuf段 */
    }
    tcp_close(pcb); /* 关闭这个连接 */
    err = ERR_OK;

    return err;
}

static err_t tcp_server_close(void *arg) {
    TCP_SERVER_T *state = (TCP_SERVER_T *)arg;
    err_t err = ERR_OK;
    if (state->client_pcb != NULL) {
        tcp_arg(state->client_pcb, NULL);
        tcp_poll(state->client_pcb, NULL, 0);
        tcp_sent(state->client_pcb, NULL);
        tcp_recv(state->client_pcb, NULL);
        tcp_err(state->client_pcb, NULL);
        err = tcp_close(state->client_pcb);
        if (err != ERR_OK) {
            DEBUG_printf("close failed %d, calling abort\n", err);
            tcp_abort(state->client_pcb);
            err = ERR_ABRT;
        }
        state->client_pcb = NULL;
    }
    if (state->server_pcb) {
        tcp_arg(state->server_pcb, NULL);
        tcp_close(state->server_pcb);
        state->server_pcb = NULL;
    }
    return err;
}

static err_t tcp_server_result(void *arg, int status) {
    TCP_SERVER_T *state = (TCP_SERVER_T *)arg;
    if (status == 0) {
        DEBUG_printf("test success\n");
    } else {
        DEBUG_printf("test failed %d\n", status);
    }
    state->complete = true;
    return tcp_server_close(arg);
}

err_t tcp_server_send_data(void *arg, struct tcp_pcb *tpcb, const void *dataptr,
                           u16_t len) {
    TCP_SERVER_T *state = (TCP_SERVER_T *)arg;
    state->sent_len = 0;
    DEBUG_printf("Writing %ld bytes to client\n", BUF_SIZE);
    // this method is callback from lwIP, so cyw43_arch_lwip_begin is not
    // required, however you can use this method to cause an assertion in debug
    // mode, if this method is called when cyw43_arch_lwip_begin IS needed
    cyw43_arch_lwip_check();
    err_t err = tcp_write(tpcb, dataptr, len, TCP_WRITE_FLAG_COPY);
    if (err != ERR_OK) {
        DEBUG_printf("Failed to write data %d\n", err);
        return tcp_server_result(arg, -1);
    }
    return ERR_OK;
}

// err_t tcp_server_send_data(void *arg, struct tcp_pcb *tpcb) {
//     TCP_SERVER_T *state = (TCP_SERVER_T *)arg;
//     for (int i = 0; i < BUF_SIZE; i++) {
//         state->buffer_sent[i] = rand();
//     }

//     state->sent_len = 0;
//     DEBUG_printf("Writing %ld bytes to client\n", BUF_SIZE);
//     // this method is callback from lwIP, so cyw43_arch_lwip_begin is not
//     // required, however you can use this method to cause an assertion in
//     debug
//     // mode, if this method is called when cyw43_arch_lwip_begin IS needed
//     cyw43_arch_lwip_check();
//     err_t err =
//         tcp_write(tpcb, state->buffer_sent, BUF_SIZE, TCP_WRITE_FLAG_COPY);
//     if (err != ERR_OK) {
//         DEBUG_printf("Failed to write data %d\n", err);
//         return tcp_server_result(arg, -1);
//     }
//     return ERR_OK;
// }

static err_t tcp_server_sent(void *arg, struct tcp_pcb *tpcb, u16_t len) {
    TCP_SERVER_T *state = (TCP_SERVER_T *)arg;
    DEBUG_printf("tcp_server_sent %u\n", len);
    state->sent_len += len;

    if (state->sent_len >= BUF_SIZE) {
        // We should get the data back from the client
        state->recv_len = 0;
        DEBUG_printf("Waiting for buffer from client\n");
    }
    return ERR_OK;
}

// err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p,
//                       err_t err) {
//     TCP_SERVER_T *state = (TCP_SERVER_T *)arg;
//     if (!p) {
//         return tcp_server_result(arg, -1);
//     }
//     // this method is callback from lwIP, so cyw43_arch_lwip_begin is not
//     // required, however you can use this method to cause an assertion in
//     debug
//     // mode, if this method is called when cyw43_arch_lwip_begin IS needed
//     cyw43_arch_lwip_check();
//     if (p->tot_len > 0) {
//         DEBUG_printf("tcp_server_recv %d/%d err %d\n", p->tot_len,
//                      state->recv_len, err);

//         // Receive the buffer
//         const uint16_t buffer_left = BUF_SIZE - state->recv_len;
//         state->recv_len += pbuf_copy_partial(
//             p, state->buffer_recv + state->recv_len,
//             p->tot_len > buffer_left ? buffer_left : p->tot_len, 0);
//         tcp_recved(tpcb, p->tot_len);
//     }
//     pbuf_free(p);

//     // Have we have received the whole buffer
//     if (state->recv_len == BUF_SIZE) {
//         // check it matches
//         if (memcmp(state->buffer_sent, state->buffer_recv, BUF_SIZE) != 0) {
//             DEBUG_printf("buffer mismatch\n");
//             return tcp_server_result(arg, -1);
//         }
//         DEBUG_printf("tcp_server_recv buffer ok\n");

//         // Test complete?
//         state->run_count++;
//         if (state->run_count >= TEST_ITERATIONS) {
//             tcp_server_result(arg, 0);
//             return ERR_OK;
//         }

//         // Send another buffer
//         return tcp_server_send_data(arg, state->client_pcb);
//     }
//     return ERR_OK;
// }

static err_t tcp_server_poll(void *arg, struct tcp_pcb *tpcb) {
    DEBUG_printf("tcp_server_poll_fn\n");
    return tcp_server_result(arg, -1); // no response is an error?
}

static void tcp_server_err(void *arg, err_t err) {
    if (err != ERR_ABRT) {
        DEBUG_printf("tcp_client_err_fn %d\n", err);
        tcp_server_result(arg, err);
    }
}

static err_t tcp_server_accept(void *arg, struct tcp_pcb *client_pcb,
                               err_t err) {
    TCP_SERVER_T *state = (TCP_SERVER_T *)arg;
    if (err != ERR_OK || client_pcb == NULL) {
        DEBUG_printf("Failure in accept\n");
        tcp_server_result(arg, err);
        return ERR_VAL;
    }
    DEBUG_printf("Client connected\n");

    state->client_pcb = client_pcb;
    tcp_arg(client_pcb, state);
    // tcp_sent(client_pcb, tcp_server_sent);
    tcp_recv(client_pcb, http_recv);
    tcp_poll(client_pcb, tcp_server_poll, POLL_TIME_S * 2);
    tcp_err(client_pcb, tcp_server_err);

    return ERR_OK; //tcp_server_send_data(arg, state->client_pcb, home_page, sizeof(home_page));
}

static bool tcp_server_open(void *arg) {
    TCP_SERVER_T *state = (TCP_SERVER_T *)arg;
    DEBUG_printf("Starting server at %s on port %u\n",
                 ip4addr_ntoa(netif_ip4_addr(netif_list)), TCP_PORT);

    struct tcp_pcb *pcb = tcp_new_ip_type(IPADDR_TYPE_ANY);
    if (!pcb) {
        DEBUG_printf("failed to create pcb\n");
        return false;
    }

    err_t err = tcp_bind(pcb, NULL, TCP_PORT);
    if (err) {
        DEBUG_printf("failed to bind to port %d\n");
        return false;
    }

    state->server_pcb = tcp_listen_with_backlog(pcb, 1);
    if (!state->server_pcb) {
        DEBUG_printf("failed to listen\n");
        if (pcb) {
            tcp_close(pcb);
        }
        return false;
    }

    tcp_arg(state->server_pcb, state);
    tcp_accept(state->server_pcb, tcp_server_accept);

    return true;
}

static TCP_SERVER_T *tcp_server_init(void) {
    TCP_SERVER_T *state = calloc(1, sizeof(TCP_SERVER_T));
    if (!state) {
        DEBUG_printf("failed to allocate state\n");
        return NULL;
    }
    return state;
}

/*
 * 函数名：httpd_init
 * 描述  ：初始化web server，初始化后才能显示网页
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void httpd_init(void) {
    TCP_SERVER_T *state = tcp_server_init();
    if (!state) {
        return;
    }
    if (!tcp_server_open(state)) {
        tcp_server_result(state, -1);
        return;
    }
    while (!state->complete) {
        cyw43_arch_poll();
        sleep_ms(1);
    }
    free(state);
}


//   tcp_accept(pcb,http_accept);             /* 设置有连接请求时的回调函数 */
// }
