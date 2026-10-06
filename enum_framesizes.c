#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "v4l2_common.h"
#include "enum_framesizes.h"

int v4l2_enum_framesizes(
    int fd,
    __u32 pixel_format)
{
    struct v4l2_frmsizeenum size;

    char format[5];

    fourcc_to_string(pixel_format, format);

    printf("\n========================================\n");
    printf("SUPPORTED RESOLUTIONS FOR %s\n", format);
    printf("========================================\n");

    int found = 0;

    for (__u32 index = 0; ; index++) {

        memset(&size, 0, sizeof(size));

        size.index = index;
        size.pixel_format = pixel_format;

        if (xioctl(
                fd,
                VIDIOC_ENUM_FRAMESIZES,
                &size) == -1) {

            if (errno == EINVAL)
                break;

            perror("VIDIOC_ENUM_FRAMESIZES");
            return -1;
        }

        found++;

        if (size.type == V4L2_FRMSIZE_TYPE_DISCRETE) {

            printf("%d. %ux%u\n",
                   found,
                   size.discrete.width,
                   size.discrete.height);
        }

        else if (size.type == V4L2_FRMSIZE_TYPE_STEPWISE) {

            printf("%d. STEPWISE\n", found);

            printf("   Min : %ux%u\n",
                   size.stepwise.min_width,
                   size.stepwise.min_height);

            printf("   Max : %ux%u\n",
                   size.stepwise.max_width,
                   size.stepwise.max_height);

            printf("   Step: %ux%u\n",
                   size.stepwise.step_width,
                   size.stepwise.step_height);
        }

        else if (size.type == V4L2_FRMSIZE_TYPE_CONTINUOUS) {

            printf("%d. CONTINUOUS\n", found);

            printf("   Min : %ux%u\n",
                   size.stepwise.min_width,
                   size.stepwise.min_height);

            printf("   Max : %ux%u\n",
                   size.stepwise.max_width,
                   size.stepwise.max_height);
        }
    }

    if (found == 0) {

        printf("No resolutions found.\n");
        return -1;
    }

    printf("========================================\n");

    return found;
}

int v4l2_get_resolution(
    int fd,
    __u32 pixel_format,
    int choice,
    __u32 *width,
    __u32 *height)
{
    struct v4l2_frmsizeenum size;

    int current = 0;

    for (__u32 index = 0; ; index++) {

        memset(&size, 0, sizeof(size));

        size.index = index;
        size.pixel_format = pixel_format;

        if (xioctl(
                fd,
                VIDIOC_ENUM_FRAMESIZES,
                &size) == -1) {

            if (errno == EINVAL)
                break;

            perror("VIDIOC_ENUM_FRAMESIZES");
            return -1;
        }

        if (size.type == V4L2_FRMSIZE_TYPE_DISCRETE) {

            current++;

            if (current == choice) {

                *width = size.discrete.width;
                *height = size.discrete.height;

                return 0;
            }
        }
    }

    return -1;
}