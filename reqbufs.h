#ifndef REQBUFS_H
#define REQBUFS_H

#include <linux/videodev2.h>

int v4l2_reqbufs(
    int fd,
    unsigned int count,
    enum v4l2_memory memory,
    struct v4l2_requestbuffers *req);

#endif