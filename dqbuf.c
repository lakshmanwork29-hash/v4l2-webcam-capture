#include <stdio.h>
#include <string.h>

#include "v4l2_common.h"
#include "dqbuf.h"

int v4l2_dqbuf(
    int fd,
    enum v4l2_memory memory,
    struct v4l2_buffer *buf)
{
    memset(buf, 0, sizeof(*buf));

    buf->type =
        V4L2_BUF_TYPE_VIDEO_CAPTURE;

    buf->memory = memory;

    if (xioctl(
            fd,
            VIDIOC_DQBUF,
            buf) == -1) {

        return -1;
    }

    return 0;
}