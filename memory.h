#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

#include "v4l2_common.h"

int allocate_userptr_buffers(
    struct buffer *buffers,
    unsigned int count,
    size_t buffer_size);

void free_userptr_buffers(
    struct buffer *buffers,
    unsigned int count);

int map_buffer(
    int fd,
    struct buffer *buffer,
    size_t length,
    off_t offset);

void unmap_buffers(
    struct buffer *buffers,
    unsigned int count);

#endif
