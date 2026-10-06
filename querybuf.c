#include <stdio.h>
#include <string.h>

#include "v4l2_common.h"
#include "querybuf.h"

int v4l2_querybuf(
    int fd,
    enum v4l2_memory memory,
    unsigned int index,
    struct v4l2_buffer *buf)
{
    memset(buf, 0, sizeof(*buf));

    buf->type =
        V4L2_BUF_TYPE_VIDEO_CAPTURE;

    buf->memory = memory;

    buf->index = index;

    if (xioctl(
            fd,
            VIDIOC_QUERYBUF,
            buf) == -1) {

        perror("VIDIOC_QUERYBUF");
        return -1;
    }

    return 0;
}