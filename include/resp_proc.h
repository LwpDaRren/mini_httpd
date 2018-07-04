#ifndef __RESP_PROC__
#define __RESP_PROC__

#define RESP_HEADER_MAXSIZE     (4*1024) 

struct resp_header_info
{
    uint32_t status_code;
};

extern void response_procedure_handler(uint32_t connection_fd);

#endif
