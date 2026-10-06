#include <stdio.h>
#include <string.h>

#include "v4l2_common.h"
#include "reqbufs.h"

int v4l2_reqbufs(
    int fd,
    unsigned int count,
    enum v4l2_memory memory,
    struct v4l2_requestbuffers *req)
{
    memset(req, 0, sizeof(*req));

    req->count = count;
    req->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req->memory = memory;

    if (xioctl(fd, VIDIOC_REQBUFS, req) == -1) {

        perror("VIDIOC_REQBUFS");
        return -1;
    }

    return 0;
}