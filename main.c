#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/videodev2.h>

#include "v4l2_common.h"

#include "querycap.h"
#include "enum_fmt.h"
#include "enum_framesizes.h"
#include "s_fmt.h"
#include "reqbufs.h"
#include "querybuf.h"
#include "qbuf.h"
#include "dqbuf.h"
#include "streamon.h"
#include "streamoff.h"

#include "memory.h"
#include "frame.h"

int main(void)
{
    int fd = -1;

    struct v4l2_capability cap;
    struct v4l2_format fmt;
    struct v4l2_requestbuffers req;

    struct buffer buffers[BUFFER_COUNT];

    memset(
        buffers,
        0,
        sizeof(buffers));

    /* ===================================================== */
    /* OPEN DEVICE                                            */
    /* ===================================================== */

    fd = open(
        DEVICE,
        O_RDWR);

    if (fd == -1) {

        perror("open");
        return EXIT_FAILURE;
    }

    printf("Device opened successfully\n");
    printf("Device           : %s\n", DEVICE);
    printf("File descriptor  : %d\n", fd);

    /* ===================================================== */
    /* VIDIOC_QUERYCAP                                        */
    /* ===================================================== */

    memset(&cap, 0, sizeof(cap));

    if (v4l2_querycap(fd, &cap) == -1) {

        close(fd);
        return EXIT_FAILURE;
    }

    print_capabilities(&cap);

    if (check_capture_streaming_capabilities(&cap) == -1) {

        close(fd);
        return EXIT_FAILURE;
    }

    /* ===================================================== */
    /* VIDIOC_ENUM_FMT                                        */
    /* ===================================================== */

    if (v4l2_enum_formats(fd) <= 0) {

        close(fd);
        return EXIT_FAILURE;
    }

    int format_choice;

    printf("\nEnter pixel format number: ");

    if (scanf("%d", &format_choice) != 1) {

        fprintf(stderr,
                "Invalid input\n");

        close(fd);
        return EXIT_FAILURE;
    }

    __u32 selected_pixelformat;

    if (v4l2_get_format_by_choice(
            fd,
            format_choice,
            &selected_pixelformat) == -1) {

        fprintf(stderr,
                "Invalid pixel format choice\n");

        close(fd);
        return EXIT_FAILURE;
    }

    char selected_format[5];

    fourcc_to_string(
        selected_pixelformat,
        selected_format);

    printf("\nSelected format: %s\n",
           selected_format);

    /* ===================================================== */
    /* VIDIOC_ENUM_FRAMESIZES                                 */
    /* ===================================================== */

    int resolution_count =
        v4l2_enum_framesizes(
            fd,
            selected_pixelformat);

    if (resolution_count <= 0) {

        close(fd);
        return EXIT_FAILURE;
    }

    int resolution_choice;

    printf("\nEnter resolution number: ");

    if (scanf(
            "%d",
            &resolution_choice) != 1) {

        fprintf(stderr,
                "Invalid input\n");

        close(fd);
        return EXIT_FAILURE;
    }

    if (resolution_choice < 1 ||
        resolution_choice > resolution_count) {

        fprintf(stderr,
                "Invalid resolution choice\n");

        close(fd);
        return EXIT_FAILURE;
    }

    __u32 width;
    __u32 height;

    if (v4l2_get_resolution(
            fd,
            selected_pixelformat,
            resolution_choice,
            &width,
            &height) == -1) {

        fprintf(stderr,
                "Could not obtain selected resolution\n");

        close(fd);
        return EXIT_FAILURE;
    }

    printf("\nSelected resolution: %ux%u\n",
           width,
           height);

    /* ===================================================== */
    /* FRAME COUNT                                            */
    /* ===================================================== */

    int frame_count;

    printf("\nEnter number of frames to capture: ");

    if (scanf("%d", &frame_count) != 1) {

        fprintf(stderr,
                "Invalid input\n");

        close(fd);
        return EXIT_FAILURE;
    }

    if (frame_count <= 0) {

        fprintf(stderr,
                "Frame count must be greater than 0\n");

        close(fd);
        return EXIT_FAILURE;
    }

    /* ===================================================== */
    /* MEMORY METHOD                                          */
    /* ===================================================== */

    int memory_choice;

    printf("\n========================================\n");
    printf("CHOOSE MEMORY METHOD\n");
    printf("========================================\n");

    printf("1. MMAP\n");
    printf("2. USERPTR\n");

    printf("Enter choice: ");

    if (scanf(
            "%d",
            &memory_choice) != 1) {

        fprintf(stderr,
                "Invalid input\n");

        close(fd);
        return EXIT_FAILURE;
    }

    enum v4l2_memory memory_type;

    if (memory_choice == 1) {

        memory_type =
            V4L2_MEMORY_MMAP;
    }

    else if (memory_choice == 2) {

        memory_type =
            V4L2_MEMORY_USERPTR;
    }

    else {

        fprintf(stderr,
                "Invalid memory method choice\n");

        close(fd);
        return EXIT_FAILURE;
    }

    printf("\nSelected memory method: ");

    if (memory_type == V4L2_MEMORY_MMAP)
        printf("MMAP\n");
    else
        printf("USERPTR\n");

    /* ===================================================== */
    /* VIDIOC_S_FMT                                           */
    /* ===================================================== */

    memset(
        &fmt,
        0,
        sizeof(fmt));

    fmt.type =
        V4L2_BUF_TYPE_VIDEO_CAPTURE;

    fmt.fmt.pix.width =
        width;

    fmt.fmt.pix.height =
        height;

    fmt.fmt.pix.pixelformat =
        selected_pixelformat;

    fmt.fmt.pix.field =
        V4L2_FIELD_ANY;

    if (v4l2_s_fmt(
            fd,
            &fmt) == -1) {

        close(fd);
        return EXIT_FAILURE;
    }

    print_format(&fmt);

    /* ===================================================== */
    /* VIDIOC_REQBUFS                                         */
    /* ===================================================== */

    if (v4l2_reqbufs(
            fd,
            BUFFER_COUNT,
            memory_type,
            &req) == -1) {

        if (memory_type ==
            V4L2_MEMORY_USERPTR) {

            printf("\n");
            printf("The driver may not support USERPTR.\n");
        }

        close(fd);
        return EXIT_FAILURE;
    }

    printf("\n========================================\n");
    printf("VIDIOC_REQBUFS\n");
    printf("========================================\n");

    printf("Requested buffers : %d\n",
           BUFFER_COUNT);

    printf("Driver returned   : %u\n",
           req.count);

    printf("Memory method     : %u\n",
           req.memory);

    if (req.count < 2) {

        fprintf(stderr,
                "Insufficient buffer memory\n");

        close(fd);
        return EXIT_FAILURE;
    }

    if (req.count > BUFFER_COUNT) {

        fprintf(stderr,
                "Driver returned more buffers "
                "than BUFFER_COUNT\n");

        close(fd);
        return EXIT_FAILURE;
    }

    /* ===================================================== */
    /* MMAP                                                   */
    /* ===================================================== */

    if (memory_type ==
        V4L2_MEMORY_MMAP) {

        printf("\n========================================\n");
        printf("SETTING UP MMAP BUFFERS\n");
        printf("========================================\n");

        for (unsigned int i = 0;
             i < req.count;
             i++) {

            struct v4l2_buffer buf;

            /* --------------------------------------------- */
            /* VIDIOC_QUERYBUF                               */
            /* --------------------------------------------- */

            if (v4l2_querybuf(
                    fd,
                    V4L2_MEMORY_MMAP,
                    i,
                    &buf) == -1) {

                unmap_buffers(
                    buffers,
                    req.count);

                close(fd);
                return EXIT_FAILURE;
            }

            printf("\nQUERYBUF buffer %u\n",
                   i);

            printf("length : %u\n",
                   buf.length);

            printf("offset : %u\n",
                   buf.m.offset);

            /* --------------------------------------------- */
            /* mmap()                                        */
            /* --------------------------------------------- */

            if (map_buffer(
                    fd,
                    &buffers[i],
                    buf.length,
                    buf.m.offset) == -1) {

                unmap_buffers(
                    buffers,
                    i);

                close(fd);
                return EXIT_FAILURE;
            }

            printf("mapped address : %p\n",
                   buffers[i].start);

            /* --------------------------------------------- */
            /* VIDIOC_QBUF                                   */
            /* --------------------------------------------- */

            if (v4l2_qbuf(
                    fd,
                    &buf) == -1) {

                unmap_buffers(
                    buffers,
                    req.count);

                close(fd);
                return EXIT_FAILURE;
            }
        }
    }

    /* ===================================================== */
    /* USERPTR                                                */
    /* ===================================================== */

    else {

        printf("\n========================================\n");
        printf("SETTING UP USERPTR BUFFERS\n");
        printf("========================================\n");

        size_t user_buffer_size =
            fmt.fmt.pix.sizeimage;

        if (user_buffer_size == 0) {

            fprintf(stderr,
                    "Invalid USERPTR buffer size\n");

            close(fd);
            return EXIT_FAILURE;
        }

        if (allocate_userptr_buffers(
                buffers,
                req.count,
                user_buffer_size) == -1) {

            close(fd);
            return EXIT_FAILURE;
        }

        for (unsigned int i = 0;
             i < req.count;
             i++) {

            struct v4l2_buffer buf;

            memset(
                &buf,
                0,
                sizeof(buf));

            buf.type =
                V4L2_BUF_TYPE_VIDEO_CAPTURE;

            buf.memory =
                V4L2_MEMORY_USERPTR;

            buf.index = i;

            buf.m.userptr =
                (unsigned long)
                buffers[i].start;

            buf.length =
                buffers[i].length;

            if (v4l2_qbuf(
                    fd,
                    &buf) == -1) {

                free_userptr_buffers(
                    buffers,
                    req.count);

                close(fd);
                return EXIT_FAILURE;
            }
        }
    }

    /* ===================================================== */
    /* VIDIOC_STREAMON                                        */
    /* ===================================================== */

    printf("\n========================================\n");
    printf("VIDIOC_STREAMON\n");
    printf("========================================\n");

    if (v4l2_streamon(fd) == -1) {

        if (memory_type ==
            V4L2_MEMORY_MMAP) {

            unmap_buffers(
                buffers,
                req.count);
        }
        else {

            free_userptr_buffers(
                buffers,
                req.count);
        }

        close(fd);
        return EXIT_FAILURE;
    }

    printf("Streaming started.\n");

    /* ===================================================== */
    /* CAPTURE LOOP                                           */
    /* ===================================================== */

    printf("\n========================================\n");
    printf("CAPTURING FRAMES\n");
    printf("========================================\n");

    for (int frame_number = 1;
         frame_number <= frame_count;
         frame_number++) {

        struct v4l2_buffer buf;

        /* ------------------------------------------------- */
        /* VIDIOC_DQBUF                                     */
        /* ------------------------------------------------- */

        if (v4l2_dqbuf(
                fd,
                memory_type,
                &buf) == -1) {

            if (errno == EAGAIN) {

                printf("No frame available yet\n");

                frame_number--;

                continue;
            }

            perror("VIDIOC_DQBUF");
            break;
        }

        printf("\n========================================\n");
        printf("FRAME %d\n", frame_number);
        printf("========================================\n");

        print_buffer(
            &buf,
            memory_type);

        if (buf.index >= req.count) {

            fprintf(stderr,
                    "Invalid buffer index: %u\n",
                    buf.index);

            break;
        }

        if (buf.bytesused >
            buffers[buf.index].length) {

            fprintf(stderr,
                    "Invalid bytesused: %u\n",
                    buf.bytesused);

            break;
        }

        void *frame_data =
            buffers[buf.index].start;

        printf("Frame data address : %p\n",
               frame_data);

        printf("Frame size         : %u bytes\n",
               buf.bytesused);

        /* ------------------------------------------------- */
        /* SAVE FRAME                                        */
        /* ------------------------------------------------- */

        save_frame(
            frame_data,
            buf.bytesused,
            frame_number,
            selected_pixelformat);

        /* ------------------------------------------------- */
        /* USERPTR needs pointer/length restored             */
        /* ------------------------------------------------- */

        if (memory_type ==
            V4L2_MEMORY_USERPTR) {

            buf.m.userptr =
                (unsigned long)
                buffers[buf.index].start;

            buf.length =
                buffers[buf.index].length;
        }

        /* ------------------------------------------------- */
        /* VIDIOC_QBUF                                       */
        /* ------------------------------------------------- */

        if (v4l2_qbuf(
                fd,
                &buf) == -1) {

            break;
        }

        printf("Buffer %u returned to driver.\n",
               buf.index);
    }

    /* ===================================================== */
    /* VIDIOC_STREAMOFF                                      */
    /* ===================================================== */

    printf("\n========================================\n");
    printf("VIDIOC_STREAMOFF\n");
    printf("========================================\n");

    if (v4l2_streamoff(fd) == -1) {

        fprintf(stderr,
                "Failed to stop streaming\n");
    }
    else {

        printf("Streaming stopped.\n");
    }

    /* ===================================================== */
    /* CLEANUP                                                */
    /* ===================================================== */

    printf("\nCleaning up...\n");

    if (memory_type ==
        V4L2_MEMORY_MMAP) {

        unmap_buffers(
            buffers,
            req.count);
    }

    else {

        free_userptr_buffers(
            buffers,
            req.count);
    }

    close(fd);

    printf("Device closed.\n");
    printf("Done.\n");

    return EXIT_SUCCESS;
}