#include <stdio.h>
#include <errno.h>
#include <sys/ioctl.h>

#include "v4l2_common.h"

int xioctl(int fd, unsigned long request, void *arg)
{
    int ret;

    do {
        ret = ioctl(fd, request, arg);
    } while (ret == -1 && errno == EINTR);

    return ret;
}

void fourcc_to_string(__u32 pixelformat, char *str)
{
    str[0] = pixelformat & 0xff;
    str[1] = (pixelformat >> 8) & 0xff;
    str[2] = (pixelformat >> 16) & 0xff;
    str[3] = (pixelformat >> 24) & 0xff;
    str[4] = '\0';
}