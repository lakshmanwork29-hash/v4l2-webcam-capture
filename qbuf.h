#ifndef QBUF_H
#define QBUF_H

#include <linux/videodev2.h>

int v4l2_qbuf(
    int fd,
    struct v4l2_buffer *buf);

#endif