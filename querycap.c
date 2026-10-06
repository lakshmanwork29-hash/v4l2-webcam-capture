#include <stdio.h>
#include "v4l2_common.h"
#include "querycap.h"

int v4l2_querycap(int fd, struct v4l2_capability *cap)
{
    if (xioctl(fd, VIDIOC_QUERYCAP, cap) == -1) {
        perror("VIDIOC_QUERYCAP");
        return -1;
    }

    return 0;
}

void print_capabilities(struct v4l2_capability *cap)
{
    printf("\n==============================\n");
    printf("DEVICE CAPABILITIES\n");
    printf("==============================\n");

    printf("Driver       : %s\n", cap->driver);
    printf("Card         : %s\n", cap->card);
    printf("Bus info     : %s\n", cap->bus_info);

    printf("Version      : %u.%u.%u\n",
           (cap->version >> 16) & 0xff,
           (cap->version >> 8) & 0xff,
           cap->version & 0xff);

    printf("Capabilities : 0x%08x\n",
           cap->capabilities);

    printf("Device caps  : 0x%08x\n",
           cap->device_caps);

    __u32 caps;

    if (cap->capabilities & V4L2_CAP_DEVICE_CAPS)
        caps = cap->device_caps;
    else
        caps = cap->capabilities;

    printf("\nCapability Flags:\n");

    if (caps & V4L2_CAP_VIDEO_CAPTURE)
        printf("  V4L2_CAP_VIDEO_CAPTURE\n");

    if (caps & V4L2_CAP_STREAMING)
        printf("  V4L2_CAP_STREAMING\n");

    if (caps & V4L2_CAP_READWRITE)
        printf("  V4L2_CAP_READWRITE\n");

    printf("==============================\n");
}

int check_capture_streaming_capabilities(
    struct v4l2_capability *cap)
{
    __u32 caps;

    if (cap->capabilities & V4L2_CAP_DEVICE_CAPS)
        caps = cap->device_caps;
    else
        caps = cap->capabilities;

    if (!(caps & V4L2_CAP_VIDEO_CAPTURE)) {

        fprintf(stderr,
                "Device does not support VIDEO_CAPTURE\n");

        return -1;
    }

    if (!(caps & V4L2_CAP_STREAMING)) {

        fprintf(stderr,
                "Device does not support STREAMING\n");

        return -1;
    }

    return 0;
}