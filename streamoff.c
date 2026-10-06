#include <stdio.h>
#include <linux/videodev2.h>

#include "v4l2_common.h"
#include "streamoff.h"

int v4l2_streamoff(int fd)
{
    enum v4l2_buf_type type =
        V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (xioctl(
            fd,
            VIDIOC_STREAMOFF,
            &type) == -1) {

        perror("VIDIOC_STREAMOFF");
        return -1;
    }

    return 0;
}