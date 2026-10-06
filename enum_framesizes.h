#ifndef ENUM_FRAMESIZES_H
#define ENUM_FRAMESIZES_H

#include <linux/videodev2.h>

int v4l2_enum_framesizes(
    int fd,
    __u32 pixel_format);

int v4l2_get_resolution(
    int fd,
    __u32 pixel_format,
    int choice,
    __u32 *width,
    __u32 *height);

#endif