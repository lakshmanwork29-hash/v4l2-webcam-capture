#ifndef QUERYBUF_H
#define QUERYBUF_H

#include <linux/videodev2.h>

int v4l2_querybuf(
    int fd,
    enum v4l2_memory memory,
    unsigned int index,
    struct v4l2_buffer *buf);

#endif