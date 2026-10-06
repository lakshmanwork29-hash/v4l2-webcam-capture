#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

#include "memory.h"

int allocate_userptr_buffers(
    struct buffer *buffers,
    unsigned int count,
    size_t buffer_size)
{
    long page_size =
        sysconf(_SC_PAGESIZE);

    if (page_size <= 0) {

        perror("sysconf");
        return -1;
    }

    printf("\nAllocating USERPTR buffers...\n");

    for (unsigned int i = 0;
         i < count;
         i++) {

        void *ptr = NULL;

        int ret =
            posix_memalign(
                &ptr,
                (size_t)page_size,
                buffer_size);

        if (ret != 0) {

            fprintf(stderr,
                    "posix_memalign failed: %s\n",
                    strerror(ret));

            return -1;
        }

        buffers[i].start = ptr;
        buffers[i].length = buffer_size;

        memset(
            buffers[i].start,
            0,
            buffers[i].length);

        printf("USERPTR buffer %u: %p (%zu bytes)\n",
               i,
               buffers[i].start,
               buffers[i].length);
    }

    return 0;
}

void free_userptr_buffers(
    struct buffer *buffers,
    unsigned int count)
{
    for (unsigned int i = 0;
         i < count;
         i++) {

        free(buffers[i].start);

        buffers[i].start = NULL;
        buffers[i].length = 0;
    }
}

int map_buffer(
    int fd,
    struct buffer *buffer,
    size_t length,
    off_t offset)
{
    buffer->length = length;

    buffer->start =
        mmap(
            NULL,
            length,
            PROT_READ | PROT_WRITE,
            MAP_SHARED,
            fd,
            offset);

    if (buffer->start == MAP_FAILED) {

        buffer->start = NULL;

        perror("mmap");
        return -1;
    }

    return 0;
}

void unmap_buffers(
    struct buffer *buffers,
    unsigned int count)
{
    for (unsigned int i = 0;
         i < count;
         i++) {

        if (buffers[i].start != NULL) {

            munmap(
                buffers[i].start,
                buffers[i].length);

            buffers[i].start = NULL;
            buffers[i].length = 0;
        }
    }
}