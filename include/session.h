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
