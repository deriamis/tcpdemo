#ifndef COMMON_H_
#define COMMON_H_

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UNREACHABLE(s) assert(0 && (s))
#define UNUSED(x) (void)(x)

#define ERROR_BUF_LENGTH 1024
char error_buf[ERROR_BUF_LENGTH];
#define FATAL(s) {                                                         \
    strerror_r(errno, error_buf, ERROR_BUF_LENGTH);                        \
    fprintf(stderr, "%s:%d: %s: %s\n", __FILE__, __LINE__, s, error_buf);  \
    exit(EXIT_FAILURE);                                                               \
}

#define DEFAULT_PORT 2323
#define MAX_MSG_LENGTH 1024

#endif  // COMMON_H_
