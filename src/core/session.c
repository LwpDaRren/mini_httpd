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
#include <pthread.h>

#include "common.h"
#include "session.h"
#include "resp_proc.h"

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
        perror("server socket file discriptor init failed!\n");
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
        perror("server listen port failed!\n");
        return Server_Status_FAIL;   
    }

    return Server_Status_OK;
}

int32_t server_accept_client_session()
{
    struct sockaddr_in client_addr_info;
    int32_t client_addr_info_len;
    int32_t client_connection_fd;
    client_addr_info_len = sizeof(client_addr_info);
    client_connection_fd = accept(serverInfo.server_socks_fd, &client_addr_info, &client_addr_info_len);
    if(client_connection_fd == -1) {
        perror("server_accept_client_session");
        exit(1);
    }

    //printf("client addr %s\n", inet_ntoa(client_addr_info.sin_addr));
    //printf("client port %d\n", client_addr_info.sin_port);  

    return client_connection_fd;
}

struct session_procedure_thread_arg_s {
    void *this;
    uint32_t client_connection_fd;
    uint32_t client_msg_buf_len;
    char *p_client_msg_buf;
    struct request_info_s *request_info;
};

void session_procedure_thread(struct session_procedure_thread_arg_s *arg)
{
    int32_t recv_buf_len;
    int32_t client_connection_fd = arg->client_connection_fd;
    char *p_client_msg_buf = (char *)malloc(CLIENT_MSG_BUF_SIZE);
    if(p_client_msg_buf == NULL) {
        printf("client_msg_buf malloc failed! \n");
        return;
    }
    memset(p_client_msg_buf, 0x0, CLIENT_MSG_BUF_SIZE);

    do{  
        //printf("recv fd %d\r\n", client_connection_fd);
        recv_buf_len = recv(client_connection_fd, p_client_msg_buf + arg->client_msg_buf_len, 
           CLIENT_MSG_BUF_SIZE - arg->client_msg_buf_len, 0);
        
        //fwrite(p_client_msg_buf,recv_buf_len,1,log_fd);
        //printf("recv_buf_len=%d\n",recv_buf_len);
        if(recv_buf_len == -1) {
            break;
        } else if(recv_buf_len == 0) {
            //printf("client session closed\n");
            break;
        }
        arg->client_msg_buf_len += recv_buf_len;
    }while(1);

    //printf("fd = %d \r\n%s",client_connection_fd, p_client_msg_buf);

    request_procedure_handler(p_client_msg_buf, arg->client_msg_buf_len, client_connection_fd, arg->request_info);
    response_procedure_handler(client_connection_fd, arg->request_info);
    
    close(client_connection_fd);
    free(arg->this);
    free(p_client_msg_buf);
    pthread_detach(pthread_self());
}

void client_connection_handler()
{
    uint32_t client_connection_fd;
    char *p_client_msg_buf = NULL;
    struct request_info_s request_info;
    int32_t recv_buf_len , client_msg_buf_len = 0, ret;
    struct timeval recv_timeout = {CLIENT_CONNECTION_RECV_TIMEOUT_S, CLIENT_CONNECTION_RECV_TIMEOUT_MS};

    pthread_t session_procedure_thread_id;
    pthread_attr_t session_procedure_thread_attr;

    struct session_procedure_thread_arg_s *session_procedure_thread_arg;
    session_procedure_thread_arg = (void*)malloc(sizeof(struct session_procedure_thread_arg_s));

    //FILE *log_fd;
    //log_fd = fopen("recv.log", "aw+");

    client_connection_fd = server_accept_client_session();
    //printf("accept fd %d\r\n", client_connection_fd);

    setsockopt(client_connection_fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&recv_timeout, sizeof(struct timeval));
    
    session_procedure_thread_arg->this = session_procedure_thread_arg;
    session_procedure_thread_arg->client_connection_fd = client_connection_fd;
    session_procedure_thread_arg->client_msg_buf_len = client_msg_buf_len;
    //session_procedure_thread_arg.p_client_msg_buf = p_client_msg_buf;
    session_procedure_thread_arg->request_info = &request_info;
    pthread_attr_init(&session_procedure_thread_attr);
    pthread_attr_setdetachstate(&session_procedure_thread_attr, PTHREAD_CREATE_DETACHED);
    ret = pthread_create(&session_procedure_thread_id, &session_procedure_thread_attr, 
                            (void*)session_procedure_thread, session_procedure_thread_arg);

    //printf("%s",p_client_msg_buf);

#if 0
    //send(client_connection_fd , header , strlen(header) , 0);
    request_procedure_handler(p_client_msg_buf, client_msg_buf_len, client_connection_fd, &request_info);
    response_procedure_handler(client_connection_fd, request_info);
    
    close(client_connection_fd);
    //close(serverInfo.server_socks_fd);
    //printf("close ok\n");
    
    //fwrite(p_client_msg_buf,sizeof(p_client_msg_buf),1,log_fd);
    //fclose(log_fd);
    free(p_client_msg_buf);
#endif

    return;
}

void close_server_session()
{
    close(serverInfo.server_socks_fd);

    return;
}