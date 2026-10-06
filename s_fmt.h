#ifndef S_FMT_H
#define S_FMT_H

#include <linux/videodev2.h>

int v4l2_s_fmt(
    int fd,
    struct v4l2_format *fmt);

void print_format(struct v4l2_format *fmt);

#endif