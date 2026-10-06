#ifndef FRAME_H
#define FRAME_H

#include <linux/videodev2.h>

#include "v4l2_common.h"

void print_buffer(
    struct v4l2_buffer *buf,
    enum v4l2_memory memory);

int save_frame(
    void *frame_data,
    unsigned int bytesused,
    int frame_number,
    __u32 pixel_format);

#endif