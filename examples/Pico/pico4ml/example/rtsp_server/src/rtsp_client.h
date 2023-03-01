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

#ifndef _RTSP_H_
#define _RTSP_H_

#include "lwipopts.h"

#if LWIP_TCP

#include "lwip/tcp.h"

typedef enum State {
	INIT      = 0,
	OPTIONS   = 1,
	DESCRIBE  = 2,
	SETUP     = 3,
	PLAY      = 4,
	PAUSE     = 5,
	TEARDOWN  = 6
} State;

typedef struct RTPHeader {
	u8_t version;
	u8_t payload_type;
	u16_t sequence_number;
	u32_t timestamp;
	u32_t ssrc;
} RTPHeader;

typedef struct RTPSession {
	u16_t sequence_number;
	u32_t lost_packets;
} RTPSession;

typedef struct RTSPHeader {
	u16_t content_length;
	u16_t status_code;
	u8_t c_seq;
	long session_id;
} RTSPHeader;

typedef struct RTSPSession {
	u8_t c_seq;
	long session_id;
	State state;
	State requested_state;
	s8_t uri[256];
	s8_t url[256];
	s8_t control_url[256];
	ip4_addr_t ip;
	u16_t port;
	struct tcp_pcb *pcb;
	struct udp_pcb *rtp_pcb;
	struct RTPSession rtp_session;
} RTSPSession;

err_t rtsp_setup(RTSPSession *session, const char *uri);
err_t rtsp_play(RTSPSession *session);
err_t rtsp_pause(RTSPSession *session);
err_t rtsp_teardown(RTSPSession *session);

#endif /* LWIP_TCP */

#endif // _RTSP_H_