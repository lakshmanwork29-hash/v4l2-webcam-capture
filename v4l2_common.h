#ifndef V4L2_COMMON_H
#define V4L2_COMMON_H

#include <stddef.h>
#include <linux/videodev2.h>

#define DEVICE "/dev/video0"
#define BUFFER_COUNT 4

struct buffer {
    void *start;
    size_t length;
};

/* Retry ioctl if interrupted by a signal */
int xioctl(int fd, unsigned long request, void *arg);

/* Convert FOURCC to printable string */
void fourcc_to_string(__u32 pixelformat, char *str);

#endif