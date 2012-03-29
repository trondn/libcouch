/* -*- Mode: C; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/*
 *     Copyright 2012 Couchbase, Inc.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#ifndef LIBCOUCH_TYPES_H
#define LIBCOUCH_TYPES_H 1

#ifndef LIBCOUCH_COUCH_H
#error "Include libcouch/couch.h instead"
#endif

#include <sys/types.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

    struct libcouch_st;
    typedef struct libcouch_st* libcouch_t;

    struct libcouch_document_st;
    typedef struct libcouch_document_st* libcouch_document_t;

    typedef enum {
        COUCH_OPEN_RDONLY,
        COUCH_OPEN_RW
    } libcouch_open_mode_t;

    typedef enum {
        COUCH_SUCCESS = 0x00,
        COUCH_ERROR_ENOMEM,
        COUCH_ERROR_EIO,
        COUCH_ERROR_EINVAL,
        COUCH_ERROR_INTERNAL,
        COUCH_ERROR_OPEN_FILE,
        COUCH_ERROR_PARSE_TERM,
        COUCH_ERROR_ENOENT,
        COUCH_ERROR_NO_HEADER,
        COUCH_ERROR_HEADER_VERSION,
        COUCH_ERROR_CHECKSUM_FAIL
    } libcouch_error_t;

#ifdef __cplusplus
}
#endif

#endif
