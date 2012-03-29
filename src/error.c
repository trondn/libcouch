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
#include "internal.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

libcouch_error_t couch_remap_error(couchstore_error_t in)
{
    switch (in) {
    case COUCHSTORE_SUCCESS: return COUCH_SUCCESS;
    case COUCHSTORE_ERROR_OPEN_FILE: return COUCH_ERROR_OPEN_FILE;
    case COUCHSTORE_ERROR_PARSE_TERM: return COUCH_ERROR_PARSE_TERM;
    case COUCHSTORE_ERROR_ALLOC_FAIL: return COUCH_ERROR_ENOMEM;
    case COUCHSTORE_ERROR_READ: return COUCH_ERROR_EIO;
    case COUCHSTORE_ERROR_DOC_NOT_FOUND: return COUCH_ERROR_ENOENT;
    case COUCHSTORE_ERROR_NO_HEADER: return COUCH_ERROR_NO_HEADER;
    case COUCHSTORE_ERROR_WRITE: return COUCH_ERROR_EIO;
    case COUCHSTORE_ERROR_HEADER_VERSION: return COUCH_ERROR_HEADER_VERSION;
    case COUCHSTORE_ERROR_CHECKSUM_FAIL: return COUCH_ERROR_CHECKSUM_FAIL;
    case COUCHSTORE_ERROR_INVALID_ARGUMENTS: return COUCH_ERROR_EINVAL;
    case COUCHSTORE_ERROR_NO_SUCH_FILE: return COUCH_ERROR_ENOENT;
    default:
        return COUCH_ERROR_INTERNAL;

    }
}

LIBCOUCH_API
const char *couch_strerror(libcouch_error_t err)
{
    switch (err) {
    case COUCH_SUCCESS:
        return "success";
    case COUCH_ERROR_ENOMEM:
        return "allocation failed";
    case COUCH_ERROR_EIO:
        return "io error";
    case COUCH_ERROR_EINVAL:
        return "invalid arguments";
    case COUCH_ERROR_OPEN_FILE:
        return "failed to open file";
    case COUCH_ERROR_PARSE_TERM:
        return "failed to parse term";
    case COUCH_ERROR_ENOENT:
        return "no entry";
    case COUCH_ERROR_NO_HEADER:
        return "no header";
    case COUCH_ERROR_HEADER_VERSION:
        return "illegal header version";
    case COUCH_ERROR_CHECKSUM_FAIL:
        return "checksum fail";
    case COUCH_ERROR_INTERNAL:
    default:
        return "Internal error";
    }
}
