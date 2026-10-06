#include <stdio.h>

#include "v4l2_common.h"
#include "qbuf.h"

int v4l2_qbuf(
    int fd,
    struct v4l2_buffer *buf)
{
    if (xioctl(
            fd,
            VIDIOC_QBUF,
            buf) == -1) {

        perror("VIDIOC_QBUF");
        return -1;
    }

    return 0;
}