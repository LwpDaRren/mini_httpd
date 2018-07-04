#include <string.h>
#include "common.h"

uint32_t strcat_s(char *str_dest, uint32_t str_dest_maxlen, char *str_src)
{
    uint32_t str_len= strlen(str_dest) + strlen(str_dest) + 1;
    if(str_len > str_dest_maxlen) {
        return -1;
    } else {
        strcat(str_dest,str_src);
    }
    return str_len;
}