#include <stdio.h>

#include "v4l2_common.h"
#include "s_fmt.h"

int v4l2_s_fmt(
    int fd,
    struct v4l2_format *fmt)
{
    if (xioctl(fd, VIDIOC_S_FMT, fmt) == -1) {

        perror("VIDIOC_S_FMT");
        return -1;
    }

    return 0;
}

void print_format(struct v4l2_format *fmt)
{
    char format[5];

    fourcc_to_string(
        fmt->fmt.pix.pixelformat,
        format);

    printf("\n========================================\n");
    printf("VIDIOC_S_FMT\n");
    printf("========================================\n");

    printf("Driver accepted format:\n");

    printf("width         : %u\n",
           fmt->fmt.pix.width);

    printf("height        : %u\n",
           fmt->fmt.pix.height);

    printf("pixelformat   : %s\n",
           format);

    printf("field         : %u\n",
           fmt->fmt.pix.field);

    printf("bytesperline  : %u\n",
           fmt->fmt.pix.bytesperline);

    printf("sizeimage     : %u\n",
           fmt->fmt.pix.sizeimage);

    printf("colorspace    : %u\n",
           fmt->fmt.pix.colorspace);
}