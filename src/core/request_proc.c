/*
 * MIT License
 *
 * Copyright (c) 2018 LwpDaRren
 *
 Copyright (c) 2018-present, LwpDaRren

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
 *     Initial:        2018/07/05        LwpDaRren
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
#include <string.h>
#include <fcntl.h>
#include "common.h"
#include "request_proc.h"

static inline int32_t request_method_paser(char* request_line_buf, struct request_info_s *request_info)
{
    if(request_line_buf == NULL) {
        return -1;
    }
    request_info->request_method = strsep(&request_line_buf," ");
    request_info->request_file_path = strsep(&request_line_buf," ");

    if(request_info->request_method == NULL || request_info->request_file_path == NULL) {
        return -1;
    }

    if(strcmp(request_info->request_method, "GET") != 0) {
        return -1;
    }

    return 0;
}

static inline int32_t request_syntax_parser(char* request_buf, uint32_t request_buf_len, struct request_info_s *request_info)
{
    char *request_each_line;
    // Parse the first line, the first word should be the method
    request_each_line = strsep(&request_buf,"\r\n");

    if(request_method_paser(request_each_line,request_info) != 0) {
        goto syntax_parse_error;
    }

    request_info->request_validatied = Request_Validate_Ok;
    return 0;
syntax_parse_error:
    request_info->request_validatied = Request_Validate_Failed;
    return -1;
}

void request_procedure_handler(char *request_buf_in, uint32_t request_buf_len, 
                            uint32_t connection_fd, struct request_info_s *request_info)
{
    request_syntax_parser(request_buf_in, request_buf_len, request_info);
}