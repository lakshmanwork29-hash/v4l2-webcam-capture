#include <stdio.h>
#include <linux/videodev2.h>

#include "v4l2_common.h"
#include "streamon.h"

int v4l2_streamon(int fd)
{
    enum v4l2_buf_type type =
        V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (xioctl(
            fd,
            VIDIOC_STREAMON,
            &type) == -1) {

        perror("VIDIOC_STREAMON");
        return -1;
    }

    return 0;
}