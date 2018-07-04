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

#include <string.h>
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
#include "resp_proc.h"

static inline void response_header_add_status_code(char *resp_str, uint32_t resp_str_maxlen, uint32_t status_code)
{
    strcat_s(resp_str, resp_str_maxlen, "HTTP/1.1 ");
    switch (status_code) {
        case 200: strcat_s(resp_str, resp_str_maxlen, "200 OK \r\n"); break;
        case 404: strcat_s(resp_str, resp_str_maxlen, "404 NOT FOUND \r\n"); break;
        case 505: strcat_s(resp_str, resp_str_maxlen, "505 SERVER ERROR \r\n"); break;
        default : strcat_s(resp_str, resp_str_maxlen, "501 NOT IMPLEMENTED \r\n"); break;
    }
}

static inline void response_header_add_server(char *resp_str, uint32_t resp_str_maxlen)
{
    strcat_s(resp_str, resp_str_maxlen, "Server: Mini_httpd/0.1\r\n");
}

static inline void response_header_add_date(char *resp_str, uint32_t resp_str_maxlen)
{
    strcat_s(resp_str, resp_str_maxlen, "Date: 2018/6/30\r\n");
}

static inline void response_header_add_endline(char *resp_str, uint32_t resp_str_maxlen)
{
    strcat_s(resp_str, resp_str_maxlen, "\r\n");
}

static inline uint32_t response_header_create(char *resp_str, uint32_t resp_str_maxlen ,struct resp_header_info* p_resp_header_info)
{
    uint32_t resp_str_len = 0;
    if(resp_str == NULL) {
        return -1;
    }
    memset(resp_str, 0x0, resp_str_maxlen);

    response_header_add_status_code(resp_str, resp_str_maxlen, p_resp_header_info->status_code);
    response_header_add_server(resp_str, resp_str_maxlen);
    response_header_add_date(resp_str, resp_str_maxlen);
    response_header_add_endline(resp_str, resp_str_maxlen);
    
    return strlen(resp_str);
}

void response_procedure_handler(uint32_t connection_fd)
{
    char *header = (char*)malloc(RESP_HEADER_MAXSIZE);
    struct resp_header_info header_info;
    header_info.status_code = 200;

    response_header_create(header, RESP_HEADER_MAXSIZE, &header_info);

    send(connection_fd , header , strlen(header) , 0);
}
