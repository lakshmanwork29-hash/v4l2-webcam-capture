#ifndef QUERYCAP_H
#define QUERYCAP_H

#include <linux/videodev2.h>

int v4l2_querycap(int fd, struct v4l2_capability *cap);

void print_capabilities(struct v4l2_capability *cap);

int check_capture_streaming_capabilities(
    struct v4l2_capability *cap);

#endif