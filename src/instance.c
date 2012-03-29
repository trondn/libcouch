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

#include <stdlib.h>


LIBCOUCH_API
libcouch_error_t couch_open_handle(const char *name,
                                   libcouch_open_mode_t mode,
                                   libcouch_t *handle)
{
    couchstore_error_t err;
    uint64_t flags;
    libcouch_t ret;

    ret = calloc(1, sizeof(*ret));
    if (ret == NULL) {
        return COUCH_ERROR_ENOMEM;
    }

    ret->mode = mode;
    if (mode == COUCH_OPEN_RDONLY) {
        flags = COUCHSTORE_OPEN_FLAG_RDONLY;
    } else {
        flags = COUCHSTORE_OPEN_FLAG_CREATE;
    }

    err = couchstore_open_db(name, flags, &ret->couchstore_handle);
    if (err != COUCHSTORE_SUCCESS) {
        free(ret);
        return couch_remap_error(err);
    }

    *handle = ret;
    return COUCH_SUCCESS;
}

LIBCOUCH_API
void couch_close_handle(libcouch_t handle)
{
    if (handle->mode != COUCH_OPEN_RDONLY) {
        (void)couch_commit(handle);
    }

    couchstore_close_db(handle->couchstore_handle);
    free(handle);
}

LIBCOUCH_API
libcouch_error_t couch_get_document(libcouch_t handle,
                                    const void *id,
                                    size_t nid,
                                    libcouch_document_t *doc)
{
    libcouch_document_t ret = calloc(1, sizeof(*ret));
    couchstore_error_t err;

    if (ret == NULL) {
        return COUCH_ERROR_ENOMEM;
    }

    err = couchstore_docinfo_by_id(handle->couchstore_handle, id, nid, &ret->info);
    if (err != COUCHSTORE_SUCCESS) {
        couch_document_release(ret);
        return couch_remap_error(err);
    }

    err = couchstore_open_doc_with_docinfo(handle->couchstore_handle,
                                           ret->info,
                                           &ret->doc, 0);
    if (err != COUCHSTORE_SUCCESS || ret->info->deleted) {
        couch_document_release(ret);
        return couch_remap_error(err);
    }

    *doc = ret;
    return COUCH_SUCCESS;
}

LIBCOUCH_API
libcouch_error_t couch_store_document(libcouch_t handle,
                                      libcouch_document_t doc)
{
    return couch_store_documents(handle, &doc, 1);
}

LIBCOUCH_API
libcouch_error_t couch_store_documents(libcouch_t handle,
                                       libcouch_document_t *doc,
                                       size_t ndocs)
{
    Doc **docs;
    DocInfo **info;
    size_t ii;
    couchstore_error_t err;

    if (handle->mode == COUCH_OPEN_RDONLY) {
        return COUCH_ERROR_EINVAL;
    }

    docs = calloc(ndocs, sizeof(Doc*));
    info = calloc(ndocs, sizeof(DocInfo*));
    if (docs == NULL || info == NULL) {
        free(docs);
        free(info);
        return COUCH_ERROR_ENOMEM;
    }

    for (ii = 0; ii < ndocs; ++ii) {
        docs[ii] = doc[ii]->doc;
        info[ii] = doc[ii]->info;
    }

    err = couchstore_save_documents(handle->couchstore_handle, docs, info,
                                    ndocs, 0);
    free(docs);
    free(info);

    return couch_remap_error(err);
}

LIBCOUCH_API
libcouch_error_t couch_commit(libcouch_t handle)
{
    if (handle->mode == COUCH_OPEN_RDONLY) {
        return COUCH_ERROR_EINVAL;
    }
    return couch_remap_error(couchstore_commit(handle->couchstore_handle));
}
