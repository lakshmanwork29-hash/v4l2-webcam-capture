#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "v4l2_common.h"
#include "enum_fmt.h"

int v4l2_enum_formats(int fd)
{
    struct v4l2_fmtdesc fmt;
    int found = 0;

    printf("\n========================================\n");
    printf("SUPPORTED PIXEL FORMATS\n");
    printf("========================================\n");

    for (__u32 index = 0; ; index++) {

        memset(&fmt, 0, sizeof(fmt));

        fmt.index = index;
        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

        if (xioctl(fd, VIDIOC_ENUM_FMT, &fmt) == -1) {

            if (errno == EINVAL)
                break;

            perror("VIDIOC_ENUM_FMT");
            return -1;
        }

        found++;

        printf("%d. %s [%c%c%c%c]\n",
               found,
               fmt.description,
               fmt.pixelformat & 0xff,
               (fmt.pixelformat >> 8) & 0xff,
               (fmt.pixelformat >> 16) & 0xff,
               (fmt.pixelformat >> 24) & 0xff);
    }

    if (found == 0) {
        printf("No pixel formats found.\n");
        return -1;
    }

    printf("========================================\n");

    return found;
}

int v4l2_get_format_by_choice(
    int fd,
    int choice,
    __u32 *pixel_format)
{
    struct v4l2_fmtdesc fmt;
    int current = 0;

    for (__u32 index = 0; ; index++) {

        memset(&fmt, 0, sizeof(fmt));

        fmt.index = index;
        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

        if (xioctl(fd, VIDIOC_ENUM_FMT, &fmt) == -1) {

            if (errno == EINVAL)
                break;

            perror("VIDIOC_ENUM_FMT");
            return -1;
        }

        current++;

        if (current == choice) {

            *pixel_format = fmt.pixelformat;

            return 0;
        }
    }

    return -1;
}