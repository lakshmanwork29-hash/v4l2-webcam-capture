#ifndef DQBUF_H
#define DQBUF_H

#include <linux/videodev2.h>

int v4l2_dqbuf(
    int fd,
    enum v4l2_memory memory,
    struct v4l2_buffer *buf);

#endif