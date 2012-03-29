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

LIBCOUCH_API
void couch_document_release(libcouch_document_t doc)
{
    if (doc->info != NULL) {
        couchstore_free_docinfo(doc->info);
    }
    if (doc->doc) {
        couchstore_free_document(doc->doc);
    }
    free(doc->tmp_alloc_id);
    free(doc->tmp_alloc_meta);
    free(doc->tmp_alloc_bp);
    free(doc);
}

LIBCOUCH_API
libcouch_error_t couch_create_empty_document(libcouch_document_t *doc)
{
    libcouch_document_t ret = calloc(1, sizeof(*ret));
    *doc = ret;
    return (*doc == NULL) ? COUCH_ERROR_ENOMEM : COUCH_SUCCESS;
}

LIBCOUCH_API
libcouch_error_t couch_document_set_id(libcouch_document_t doc,
                                       const void *id,
                                       size_t nid,
                                       int allocate)
{
    /* The couchstore API got the const wrong here.. */
    void *ptr = (void *)id;
    assert(doc);

    if (doc->doc == NULL) {
        if ((doc->doc = calloc(1, sizeof(*doc->doc))) == NULL) {
            return COUCH_ERROR_ENOMEM;
        }
    }

    if (doc->info == NULL) {
        if ((doc->info = calloc(1, sizeof(*doc->info))) == NULL) {
            return COUCH_ERROR_ENOMEM;
        }
    }

    if (allocate) {
        free(doc->tmp_alloc_id);
        if ((doc->tmp_alloc_id = ptr = malloc(nid)) == NULL) {
            return COUCH_ERROR_ENOMEM;
        }
        memcpy(ptr, id, nid);
    }

    doc->info->id.buf = doc->doc->id.buf = ptr;
    doc->info->id.size = doc->doc->id.size = nid;

    return COUCH_SUCCESS;
}


LIBCOUCH_API
libcouch_error_t couch_document_set_meta(libcouch_document_t doc,
                                         const void *meta,
                                         size_t nmeta,
                                         int allocate)
{
    /* The couchstore API got the const wrong here.. */
    void *ptr = (void *)meta;
    assert(doc);

    if (doc->info == NULL) {
        if ((doc->info = calloc(1, sizeof(*doc->info))) == NULL) {
            return COUCH_ERROR_ENOMEM;
        }
    }

    if (allocate) {
        free(doc->tmp_alloc_meta);
        if ((doc->tmp_alloc_meta = ptr = malloc(nmeta)) == NULL) {
            return COUCH_ERROR_ENOMEM;
        }
        memcpy(ptr, meta, nmeta);
    }

    doc->info->rev_meta.buf = ptr;
    doc->info->rev_meta.size = nmeta;

    return COUCH_SUCCESS;
}

LIBCOUCH_API
libcouch_error_t couch_document_set_revision(libcouch_document_t doc, uint64_t revno)
{
    assert(doc);

    if (doc->info == NULL) {
        if ((doc->info = calloc(1, sizeof(*doc->info))) == NULL) {
            return COUCH_ERROR_ENOMEM;
        }
    }

    doc->info->rev_seq = revno;
    return COUCH_SUCCESS;
}

LIBCOUCH_API
libcouch_error_t couch_document_set_deleted(libcouch_document_t doc, int deleted)
{
    assert(doc);

    if (doc->info == NULL) {
        if ((doc->info = calloc(1, sizeof(*doc->info))) == NULL) {
            return COUCH_ERROR_ENOMEM;
        }
    }

    doc->info->deleted = deleted;
    return COUCH_SUCCESS;
}

LIBCOUCH_API
libcouch_error_t couch_document_set_value(libcouch_document_t doc,
                                          const void *value,
                                          size_t nvalue,
                                          int allocate)
{
    /* The couchstore API got the const wrong here.. */
    void *ptr = (void *)value;
    assert(doc);

    if (doc->doc == NULL) {
        if ((doc->doc = calloc(1, sizeof(*doc->doc))) == NULL) {
            return COUCH_ERROR_ENOMEM;
        }
    }

    if (doc->info == NULL) {
        if ((doc->info = calloc(1, sizeof(*doc->info))) == NULL) {
            return COUCH_ERROR_ENOMEM;
        }
    }

    if (allocate) {
        free(doc->tmp_alloc_bp);
        if ((doc->tmp_alloc_bp = ptr = malloc(nvalue)) == NULL) {
            return COUCH_ERROR_ENOMEM;
        }
        memcpy(ptr, value, nvalue);
    }

    doc->doc->data.buf = ptr;
    doc->info->size = doc->doc->data.size = nvalue;

    return COUCH_SUCCESS;
}

LIBCOUCH_API
libcouch_error_t couch_document_set_content_type(libcouch_document_t doc,
                                                 uint8_t content_type)
{
    assert(doc);

    if (doc->info == NULL) {
        if ((doc->info = calloc(1, sizeof(*doc->info))) == NULL) {
            return COUCH_ERROR_ENOMEM;
        }
    }

    /* @todo verify the content type */
    doc->info->content_meta = content_type;
    return COUCH_SUCCESS;
}

LIBCOUCH_API
libcouch_error_t couch_document_get_id(libcouch_document_t doc,
                                       const void **id,
                                       size_t *nid)
{
    if (doc == NULL || (doc->doc == NULL && doc->info == NULL)) {
        return COUCH_ERROR_EINVAL;
    }

    if (doc->doc != NULL) {
        *id = doc->doc->id.buf;
        *nid = doc->doc->id.size;
    } else {
        *id = doc->info->id.buf;
        *nid = doc->info->id.size;
    }

    return COUCH_SUCCESS;
}

LIBCOUCH_API
libcouch_error_t couch_document_get_meta(libcouch_document_t doc,
                                         const void **meta,
                                         size_t *nmeta)
{
    if (doc == NULL || doc->info == NULL) {
        return COUCH_ERROR_EINVAL;
    }

    *meta = doc->info->rev_meta.buf;
    *nmeta = doc->info->rev_meta.size;

    return COUCH_SUCCESS;
}

LIBCOUCH_API
libcouch_error_t couch_document_get_revision(libcouch_document_t doc, uint64_t *revno)
{
    if (doc == NULL || doc->info == NULL) {
        return COUCH_ERROR_EINVAL;
    }

    *revno = doc->info->rev_seq;
    return COUCH_SUCCESS;
}

LIBCOUCH_API
libcouch_error_t couch_document_get_deleted(libcouch_document_t doc, int *deleted)
{
    if (doc == NULL || doc->info == NULL) {
        return COUCH_ERROR_EINVAL;
    }

    *deleted = doc->info->deleted;
    return COUCH_SUCCESS;
}

LIBCOUCH_API
libcouch_error_t couch_document_get_value(libcouch_document_t doc,
                                          const void **value,
                                          size_t *nvalue)
{
    if (doc == NULL || doc->doc == NULL) {
        return COUCH_ERROR_EINVAL;
    }

    *value = doc->doc->data.buf;
    *nvalue = doc->doc->data.size;

    return COUCH_SUCCESS;
}

LIBCOUCH_API
libcouch_error_t couch_document_get_content_type(libcouch_document_t doc,
                                                 uint8_t *content_type)
{
    if (doc == NULL || doc->info == NULL) {
        return COUCH_ERROR_EINVAL;
    }

    *content_type = doc->info->content_meta;
    return COUCH_SUCCESS;
}
