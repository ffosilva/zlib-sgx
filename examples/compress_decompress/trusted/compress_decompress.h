#ifndef _COMPRESS_DECOMPRESS_H_
#define _COMPRESS_DECOMPRESS_H_

#include <stdlib.h>
#include <assert.h>

#define CHECK_ERR(err, msg) { \
    if (err != Z_OK) { \
        print_string("%s error: %d\n", msg, err); \
        return err; \
    } \
}

#if defined(__cplusplus)
extern "C" {
#endif

void print_string(const char *fmt, ...);

#if defined(__cplusplus)
}
#endif

#endif /* !_COMPRESS_DECOMPRESS_H_ */
