#include "endian.h"
#include <stdlib.h>
#include <string.h>

void *inverser_ordre(const void *source, size_t size, size_t n) {
    if (!source || n == 0) {
        return NULL;
    }

    if (size != 2 && size != 4 && size != 8) {
        return NULL;
    }

    size_t total_size = size * n;
    char *dest_block = malloc(total_size);
    if (!dest_block) {
        return NULL;
    }

    const char *src_ptr = (const char*)source;
    char *dest_ptr = dest_block;

    for (size_t i = 0; i < n; i++) {
        const char *current_src = src_ptr + (i * size);
        char *current_dest = dest_ptr + (i * size);

        for (size_t j = 0; j < size; j++) {
            current_dest[j] = current_src[size - 1 - j];
        }
    }

    return dest_block;
}