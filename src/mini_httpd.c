#include "stdio.h"
#include <signal.h>

#include "session.h"
#include "common.h"

void sigint_handler()
{
    close_server_session();
    exit(1);
}

void main()
{
    signal(SIGINT,sigint_handler);
    creat_server_sock_session();
    while(1) {
        client_connection_handler();
    }
    
    return;
}