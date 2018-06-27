#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <strings.h>
#include <fcntl.h>

#include "common.h"
#include "session.h"

struct stServerInfo serverInfo;

static void client_sockaddr_init(struct sockaddr_in *p_client_addr_info)
{
    bzero(p_client_addr_info,sizeof(p_client_addr_info));
    p_client_addr_info->sin_family = AF_INET;  
    p_client_addr_info->sin_addr.s_addr = htons(INADDR_ANY);  
    p_client_addr_info->sin_port = htons(MINI_HTTPD_LISTEN_PORT); 
}

uint32_t creat_server_sock_session()
{
    int32_t on = 1;
    struct sockaddr_in client_addr_info;  
    client_sockaddr_init(&client_addr_info);

    serverInfo.server_socks_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == serverInfo.server_socks_fd) {
        printf("server socket file discriptor init failed!\n");
        return Server_Status_FAIL;
    }

    setsockopt(serverInfo.server_socks_fd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
    if( bind(serverInfo.server_socks_fd ,
            (struct sockaddr *)&client_addr_info,
            sizeof(client_addr_info)) == -1 ) {
        perror("server bind port failed!\n");
        return Server_Status_FAIL;        
    }

    if (listen(serverInfo.server_socks_fd, LISTEN_BACKLOG) == -1) {
        printf("server listen port failed!\n");
        return Server_Status_FAIL;   
    }

    return Server_Status_OK;
}

int32_t server_accept_client_session()
{
    struct sockaddr client_addr_info;
    int32_t client_addr_info_len;
    int32_t client_connection_fd;
    client_addr_info_len = sizeof(client_addr_info);
    client_connection_fd = accept(serverInfo.server_socks_fd, &client_addr_info, &client_addr_info_len);
    if(client_connection_fd == -1) {
        perror("server_accept_client_session");
        exit(1);
    }

    return client_connection_fd;
}

uint32_t tmp;
char send_string[] = "hello world";
void client_connection_handler()
{
    uint32_t client_connection_fd;
    char *p_client_msg_buf = NULL;
    int32_t recv_buf_len, nready;
    struct timeval timeout = {0, 100};

    FILE *log_fd;
    log_fd = fopen("recv.log", "aw+");

    client_connection_fd = server_accept_client_session();
    tmp = client_connection_fd;

    setsockopt(client_connection_fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));
    p_client_msg_buf = (char *)malloc(CLIENT_MSG_BUF_SIZE);
    if(p_client_msg_buf == NULL) {
        printf("client_msg_buf malloc failed! \n");
        return;
    }
    
    do{
        memset(p_client_msg_buf,0x0,CLIENT_MSG_BUF_SIZE);
        recv_buf_len = recv(client_connection_fd, p_client_msg_buf, 
           sizeof(p_client_msg_buf), 0);
        printf("%s",p_client_msg_buf);
        //fwrite(p_client_msg_buf,recv_buf_len,1,log_fd);
        //printf("recv_buf_len=%d\n",recv_buf_len);
        if(recv_buf_len == -1) {
            break;
        } else if(recv_buf_len == 0) {
            printf("client session closed\n");
            break;
        }
    }while(1);

    send(client_connection_fd , send_string , strlen(send_string) , 0);
    
    close(client_connection_fd);
    //close(serverInfo.server_socks_fd);
    //printf("close ok\n");
    
    //fwrite(p_client_msg_buf,sizeof(p_client_msg_buf),1,log_fd);
    fclose(log_fd);
    free(p_client_msg_buf);

    return;
}

void close_server_session()
{
    close(tmp);
    close(serverInfo.server_socks_fd);

    return;
}