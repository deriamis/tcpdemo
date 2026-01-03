/*
   Copyright 2026 Ryan Egesdahl

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef COMMON_H_
#define COMMON_H_

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
