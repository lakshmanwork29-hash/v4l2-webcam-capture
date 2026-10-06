#include <stdio.h>

#include "frame.h"

void print_buffer(
    struct v4l2_buffer *buf,
    enum v4l2_memory memory)
{
    printf("\n--------------------------------\n");
    printf("BUFFER INFORMATION\n");
    printf("--------------------------------\n");

    printf("index      : %u\n",
           buf->index);

    printf("type       : %u\n",
           buf->type);

    printf("bytesused  : %u\n",
           buf->bytesused);

    printf("flags      : 0x%08x\n",
           buf->flags);

    printf("field      : %u\n",
           buf->field);

    printf("timestamp  : %ld.%06ld\n",
           buf->timestamp.tv_sec,
           buf->timestamp.tv_usec);

    printf("sequence   : %u\n",
           buf->sequence);

    printf("memory     : %u\n",
           buf->memory);

    printf("length     : %u\n",
           buf->length);

    if (memory == V4L2_MEMORY_MMAP) {

        printf("m.offset   : %u\n",
               buf->m.offset);
    }

    else if (memory == V4L2_MEMORY_USERPTR) {

        printf("m.userptr  : 0x%lx\n",
               buf->m.userptr);
    }

    printf("--------------------------------\n");
}

int save_frame(
    void *frame_data,
    unsigned int bytesused,
    int frame_number,
    __u32 pixel_format)
{
    char filename[128];

    const char *extension;

    if (pixel_format == V4L2_PIX_FMT_YUYV)
        extension = "yuyv";
    else
        extension = "jpg";

    snprintf(
        filename,
        sizeof(filename),
        "frame_%03d.%s",
        frame_number,
        extension);

    FILE *file =
        fopen(filename, "wb");

    if (file == NULL) {

        perror("fopen");
        return -1;
    }

    size_t written =
        fwrite(
            frame_data,
            1,
            bytesused,
            file);

    fclose(file);

    if (written != bytesused) {

        fprintf(stderr,
                "Failed to write complete frame\n");

        return -1;
    }

    printf("Saved: %s\n",
           filename);

    return 0;
}