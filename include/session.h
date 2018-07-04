/*
 * MIT License
 *
 * Copyright (c) 2018 LwpDaRren
 *
 MIT License

Copyright (c) 2015-present, LwpDaRren

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. 
 */

/*
 * Revision History:
 *     Initial:        2018/07/04        LwpDaRren
 */

#ifndef __SESSION_H__
#define __SESSION_H__

#include "common.h"

#define MINI_HTTPD_LISTEN_PORT  80
#define LISTEN_BACKLOG          50

#define CLIENT_CONNECTION_RECV_TIMEOUT_S    0
#define CLIENT_CONNECTION_RECV_TIMEOUT_MS   100

#define CLIENT_MSG_BUF_SIZE     (1*1024)

struct stServerInfo
{
    uint32_t server_socks_fd;
    uint8_t server_status;
};

extern uint32_t creat_server_sock_session();
extern void client_connection_handler();
extern void close_server_session();

#endif
