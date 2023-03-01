/*
 * Copyright (c) 2017 Murat Seker.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the authors nor the names of the contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#include "rtsp_client.h"
#include "lwip/udp.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#if LWIP_TCP

#ifndef RTSP_DEBUG
#define RTSP_DEBUG     LWIP_DBG_ON
#endif

#define CRLF           "\r\n"

#define RTSP_STATUS_OK 200
#define RTP_PORT       55852

static err_t tcp_send_packet(RTSPSession *session, const char *packet);
static u8_t starts_with(const char *str, const char *pfx, const char **ptr);
static const char *find_line_break(const char *buffer);
static void rtsp_parse_line(RTSPHeader *reply, const char *buf);
static const char *rtsp_parse_response(RTSPHeader *reply, const char *response);
static err_t process_response(RTSPSession *session, char *server_reply);
static err_t parse_url(RTSPSession *session, const char *uri);
static err_t rtsp_connected_clbk(void *arg, struct tcp_pcb *tpcb, err_t err);
static err_t rtsp_recvd_clbk(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static void rtsp_error_clbk(void *arg, err_t err);
static err_t disconnect_server(RTSPSession *session);
static err_t connect_server(RTSPSession *session);

static err_t tcp_send_packet(RTSPSession *session, const char *packet) {
    err_t result;

    LWIP_ASSERT("session != NULL", session != NULL);

    if ((result = tcp_write(session->pcb, packet, strlen(packet), TCP_WRITE_FLAG_COPY)) != ERR_OK) {
        LWIP_DEBUGF(RTSP_DEBUG, ("tcp_send_packet: Write error %d\n", result));
        return result;
    }

    if ((result = tcp_output(session->pcb)) != ERR_OK) {
        LWIP_DEBUGF(RTSP_DEBUG, ("tcp_send_packet: Output error %d\n", result));
        return result;
    }

    LWIP_DEBUGF(RTSP_DEBUG, ("%s\n", packet));

    return ERR_OK;
}

static u8_t starts_with(const char *str, const char *pfx, const char **ptr) {
    while (*pfx && toupper(*pfx) == toupper(*str)) {
        pfx++;
        str++;
    }

    if (!*pfx && ptr)
        *ptr = str;

    return !*pfx;
}

static const char *find_line_break(const char *buffer) {
    const char *pt = buffer;

    LWIP_ASSERT("buffer != NULL", buffer != NULL);

    while(*pt != '\0' && strncmp(pt, CRLF, 2) != 0) {
        ++pt;
    }

    if(*pt == '\0')
        return NULL;

    return pt + 2;
}

static void rtsp_parse_line(RTSPHeader *reply, const char *buf) {
    const char *p;

    LWIP_ASSERT("reply != NULL", reply != NULL);

    if (starts_with(buf, "Session:", &p)) {
        reply->session_id = strtol(p, NULL, 10);
    } else if (starts_with(buf, "Content-Length:", &p)) {
        reply->content_length = strtol(p, NULL, 10);
    } else if (starts_with(buf, "CSeq:", &p)) {
        reply->c_seq = strtol(p, NULL, 10);
    } else if (starts_with(buf, "RTSP/1.0", &p)) {
        reply->status_code = strtol(p, NULL, 10);
    }
}

static const char *rtsp_parse_response(RTSPHeader *reply, const char *response) {
    const char *line_break;
    const char *pt = response;

    LWIP_ASSERT("reply != NULL", reply != NULL);
    LWIP_ASSERT("response != NULL", response != NULL);

    while(pt) {
        rtsp_parse_line(reply, pt);
        line_break = find_line_break(pt);

        if(line_break == pt + 2) // Empty line
            return line_break;

        pt = line_break;
    }

    return NULL;
}

static void rtp_parse_header(RTPHeader *header, const char *payload) {
    LWIP_ASSERT("header != NULL", header != NULL);

    if(payload == NULL) return;

    header->version         = (payload[0] & 0xc0) >> 6;
    header->payload_type    = (payload[1] & 0x7F);
    header->sequence_number = (payload[3])  | (payload[2]  << 8);
    header->timestamp       = (payload[7])  | (payload[6]  << 8) | (payload[5] << 16) | (payload[4] << 24);
    header->ssrc            = (payload[11]) | (payload[10] << 8) | (payload[9] << 16) | (payload[8] << 24);
}

static err_t rtp_process_header(RTPSession *session, RTPHeader *header) {
    LWIP_ASSERT("header != NULL", header != NULL);
    LWIP_ASSERT("session != NULL", session != NULL);

    u32_t lost_packets = (header->sequence_number - session->sequence_number) - 1;
    if(lost_packets != 0) {
        session->lost_packets += lost_packets;
    }

    session->sequence_number = header->sequence_number;

    return ERR_OK;
}

static void rtp_recvd_clbk(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port) {
    RTPHeader header;
    RTPSession *session = (RTPSession *)arg;

    LWIP_ASSERT("session != NULL", session != NULL);

    if(p == NULL) return;

    rtp_parse_header(&header, (const char *)p->payload);
    rtp_process_header(session, &header);

    // TODO : Handle RTP data

    pbuf_free(p);
}

static err_t rtp_setup(RTSPSession *session) {
    err_t res;

    LWIP_DEBUGF(RTSP_DEBUG, ("rtp_setup: Opening RTP port %d\n", RTP_PORT));

    session->rtp_pcb = udp_new();
    udp_recv(session->rtp_pcb, rtp_recvd_clbk, &session->rtp_session);

    if((res = udp_bind(session->rtp_pcb, IP_ADDR_ANY, RTP_PORT)) != ERR_OK) {
        LWIP_DEBUGF(RTSP_DEBUG, ("rtp_setup: Bind error\n"));
    }

    return res;
}

static err_t rtsp_send_cmd(RTSPSession *session, State state) {
    err_t res = ERR_OK;
    char packet[256];
    char *pt = packet;

    LWIP_ASSERT("session != NULL", session != NULL);

    session->requested_state = state;

    switch(session->requested_state) {
    case OPTIONS:
        pt += sprintf(pt, "OPTIONS %s", session->url);
        break;
    case DESCRIBE:
        pt += sprintf(pt, "DESCRIBE %s", session->url);
        break;
    case SETUP:
        pt += sprintf(pt, "SETUP %s/%s", session->url, session->control_url);
        break;
    case PLAY:
        pt += sprintf(pt, "PLAY %s/%s", session->url, session->control_url);
        break;
    case TEARDOWN:
        pt += sprintf(pt, "TEARDOWN %s", session->url);
        break;
    default:
        res = ERR_VAL;
    }

    if(res == ERR_OK) {
        pt += sprintf(pt, " RTSP/1.0" CRLF);

        if(session->requested_state == SETUP) {
            pt += sprintf(pt, "Transport: RTP/AVP;unicast;client_port=%d-%d" CRLF, RTP_PORT, RTP_PORT + 1);
        }

        pt += sprintf(pt, "CSeq: %d" CRLF CRLF, ++session->c_seq);

        res = tcp_send_packet(session, packet);
    }

    return res;
}

static err_t rtsp_parse_sdp(RTSPSession *session, const char *sdp) {
    const char *pt = sdp;
    const char *url;

    if(sdp == NULL) return ERR_ARG;

    while(*pt != '\0') {
        if(strncmp(pt, "a=control:", 10) == 0) {
            pt += 10;
            url = pt;

            while(*pt != '\0' && strncmp(pt, CRLF, 2) != 0) {
                ++pt;
            }

            if(*pt != '\0') {
                memcpy(session->control_url, url, pt - url);
                session->control_url[pt - url] = '\0';
            }
        }
        ++pt;
    }

    return ERR_OK;
}

err_t check_reply(RTSPSession *session, RTSPHeader *reply) {
    if(reply->c_seq != session->c_seq) {
        /* Maybe server sent something without our request ? */
        LWIP_DEBUGF(RTSP_DEBUG, ("check_reply: Sequence numbers do not match\n"));
        return -1;
    }

    if(reply->status_code != RTSP_STATUS_OK) {
        LWIP_DEBUGF(RTSP_DEBUG, ("check_reply: Server denied %d\n", reply->status_code));
        return -1;
    }

    if(session->state > DESCRIBE) {
        if(session->session_id != reply->session_id) {
            LWIP_DEBUGF(RTSP_DEBUG, ("check_reply: Invalid session ID\n"));
            return -1;
        }
    }

    return ERR_OK;
}

static err_t process_reply(RTSPSession *session, RTSPHeader *reply, const char *payload) {
    session->state = session->requested_state;

    if(session->state > DESCRIBE) {
        if(session->session_id != reply->session_id) {
            if(session->session_id == 0) {
                session->session_id = reply->session_id;
            }
        }
    }

    if(session->state == DESCRIBE) {
        rtsp_parse_sdp(session, payload);
        rtp_setup(session);
    }

    return ERR_OK;
}

static err_t process_response(RTSPSession *session, char *server_reply) {
    err_t res;
    RTSPHeader reply;
    const char *payload;

    LWIP_ASSERT("session != NULL", session != NULL);
    LWIP_ASSERT("server_reply != NULL", server_reply != NULL);

    memset(&reply, 0, sizeof(RTSPHeader));

    if((payload = rtsp_parse_response(&reply, server_reply)) == NULL) {
        LWIP_DEBUGF(RTSP_DEBUG, ("process_response: Parse error\n"));
        return ERR_VAL;
    }

    if((res = check_reply(session, &reply)) != ERR_OK) {
        LWIP_DEBUGF(RTSP_DEBUG, ("process_response: Reply is not valid\n"));
        return res;
    }

    if((res = process_reply(session, &reply, payload)) != ERR_OK) {
        LWIP_DEBUGF(RTSP_DEBUG, ("process_response: Reply is not valid\n"));
        return res;
    }

    return ERR_OK;
}

err_t rtsp_play(RTSPSession *session) {
    LWIP_ASSERT("session != NULL", session != NULL);

    if(session->state != SETUP && session->state != PAUSE) {
        LWIP_DEBUGF(RTSP_DEBUG, ("rtsp_play: Session is not ready\n"));
        return -1;
    }

    if(rtsp_send_cmd(session, PLAY) < 0) {
        LWIP_DEBUGF(RTSP_DEBUG, ("rtsp_play: Send error\n"));
        return -1;
    }

    return ERR_OK;
}

err_t rtsp_pause(RTSPSession *session) {
    LWIP_ASSERT("session != NULL", session != NULL);

    if(session->state != PLAY) {
        LWIP_DEBUGF(RTSP_DEBUG, ("rtsp_pause: Session is not playing\n"));
        return -1;
    }

    if(rtsp_send_cmd(session, PAUSE) < 0) {
        LWIP_DEBUGF(RTSP_DEBUG, ("rtsp_pause: Send error\n"));
        return -1;
    }

    return ERR_OK;
}

err_t rtsp_teardown(RTSPSession *session) {
    LWIP_ASSERT("session != NULL", session != NULL);

    if(session->state < PLAY || session->state > PAUSE) {
        LWIP_DEBUGF(RTSP_DEBUG, ("rtsp_teardown: Session is not ready\n"));
        return -1;
    }

    if(rtsp_send_cmd(session, TEARDOWN) < 0) {
        LWIP_DEBUGF(RTSP_DEBUG, ("rtsp_teardown: Send error\n"));
        return -1;
    }

    return disconnect_server(session);
}

static err_t parse_url(RTSPSession *session, const char *uri) {
    /* Need a real parser ! */
    err_t res;
    u16_t scanned;

    LWIP_ASSERT("session != NULL", session != NULL);

    char ip_address[16];

    scanned = sscanf(uri, "rtsp://%99[^:]:%99hu/%99[^\n]",
            ip_address,
            &session->port,
            session->uri);

    if(scanned != 3) {
        LWIP_DEBUGF(RTSP_DEBUG, ("parse_url: Malformed URL\n"));

        return ERR_ARG;
    }

    res = ipaddr_aton(ip_address, &session->ip);
    if(res != 1) {
        LWIP_DEBUGF(RTSP_DEBUG, ("parse_url: Malformed URL\n"));

        return ERR_ARG;
    }

    memcpy(session->url, uri, strlen(uri));

    return ERR_OK;
}

static err_t rtsp_connected_clbk(void *arg, struct tcp_pcb *tpcb, err_t err) {
    RTSPSession *session = (RTSPSession *)arg;

    LWIP_ASSERT("session != NULL", session != NULL);
    LWIP_DEBUGF(RTSP_DEBUG, ("Connection Established.\n"));

    rtsp_send_cmd(session, OPTIONS);

    return ERR_OK;
}

static err_t rtsp_recvd_clbk(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    RTSPSession *session = (RTSPSession *)arg;

    LWIP_ASSERT("session != NULL", session != NULL);

    if (p) {
        LWIP_DEBUGF(RTSP_DEBUG, ("%s\n", (char *)p->payload));

        struct pbuf *q;
        char *text = malloc(p->tot_len + 1);
        char *pt = text;

        for (q = p; q != NULL; q = q->next) {
            memcpy(pt, q->payload, q->len);
            pt += q->len;
        }
        *pt = '\0';

        /* Inform TCP that we have taken the data */
        tcp_recved(session->pcb, p->tot_len);

        process_response(session, text);

        free(text);
        pbuf_free(p);

        rtsp_send_cmd(session, session->state + 1);
    } else {
        LWIP_DEBUGF(RTSP_DEBUG, ("rtsp_recvd_clbk: Host closed the connection\n"));

        disconnect_server(session);

        return ERR_ABRT;
    }

    return ERR_OK;
}

static void rtsp_error_clbk(void *arg, err_t err) {
    RTSPSession *session = (RTSPSession *)arg;

    LWIP_ASSERT("session != NULL", session != NULL);

    disconnect_server(session);
}

static err_t disconnect_server(RTSPSession *session) {
    tcp_arg(session->pcb, NULL);
    tcp_sent(session->pcb, NULL);
    tcp_recv(session->pcb, NULL);
    tcp_close(session->pcb);

    memset(session, 0, sizeof(RTSPSession));

    return ERR_OK;
}

static err_t connect_server(RTSPSession *session) {
    LWIP_ASSERT("session != NULL", session != NULL);

    tcp_err(session->pcb, rtsp_error_clbk);
    tcp_recv(session->pcb, rtsp_recvd_clbk);
    tcp_connect(session->pcb, &session->ip, session->port, rtsp_connected_clbk);

    return ERR_OK;
}

err_t rtsp_setup(RTSPSession *session, const char *uri) {
    err_t res;

    LWIP_ASSERT("session != NULL", session != NULL);

    if(session->state > INIT) {
        LWIP_DEBUGF(RTSP_DEBUG, ("rtsp_setup: Already connected\n"));
        return ERR_ISCONN;
    }

    memset(session, 0, sizeof(RTSPSession));

    session->pcb = tcp_new();
    tcp_arg(session->pcb, session);

    if((res = parse_url(session, uri)) != ERR_OK) {
        LWIP_DEBUGF(RTSP_DEBUG, ("start_rtsp: Invalid address : %d\n", res));
        return res;
    }

    if((res = connect_server(session)) != ERR_OK) {
        LWIP_DEBUGF(RTSP_DEBUG, ("start_rtsp: Connect error : %d\n", res));
        return res;
    }

    return ERR_OK;
}

#endif /* LWIP_TCP */