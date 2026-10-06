#ifndef ENUM_FMT_H
#define ENUM_FMT_H

#include <linux/videodev2.h>

int v4l2_enum_formats(int fd);

int v4l2_get_format_by_choice(
    int fd,
    int choice,
    __u32 *pixel_format);

#endif


