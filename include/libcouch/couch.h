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

#ifndef LIBCOUCH_COUCH_H
#define LIBCOUCH_COUCH_H 1

#include <libcouch/visibility.h>
#include <libcouch/types.h>


#ifdef __cplusplus
extern "C" {
#endif

    LIBCOUCH_API
    couch_error_t couch_open_handle(const char *name,
                                    libcouch_open_mode_t mode,
                                    libcouch_t *handle);

    /**
     * couch_close_handle release all allocated resources for the handle
     * and invalidates it.
     */
    LIBCOUCH_API
    void couch_close_handle(libcouch_t handle);

    LIBCOUCH_API
    couch_error_t couch_create_empty_document(libcouch_t handle,
                                              libcouch_document_t *doc);

    /**
     * This is a helper function to avoid having to call
     * couch_document_release() followed by
     * couch_create_empty_document(). Please note that you can
     * <b>not</b> call couch_document_reinitialize() on a document
     * returned from couch_get_document() (that have undefined behaviour,
     * but will most likely crash your process)
     */
    LIBCOUCH_API
    void couch_document_reinitialize(libcouch_document_t doc);

    LIBCOUCH_API
    couch_error_t couch_document_set_id(libcouch_document_t doc,
                                        const void *id,
                                        size_t nid,
                                        int allocate);

    LIBCOUCH_API
    couch_error_t couch_document_set_meta(libcouch_document_t doc,
                                          const void *meta,
                                          size_t nmeta,
                                          int allocate);


    LIBCOUCH_API
    couch_error_t couch_document_set_revision(libcouch_document_t doc, uint64_t revno);


    LIBCOUCH_API
    couch_error_t couch_document_set_deleted(libcouch_document_t doc, int deleted);

    LIBCOUCH_API
    couch_error_t couch_document_set_value(libcouch_document_t doc,
                                           const void *value,
                                           size_t nvalue,
                                           int allocate);

    LIBCOUCH_API
    couch_error_t couch_document_set_content_type(libcouch_document_t doc,
                                                  uint8_t content_type);


    LIBCOUCH_API
    couch_error_t couch_document_get_id(libcouch_document_t doc,
                                        const void **id,
                                        size_t *nid);

    LIBCOUCH_API
    couch_error_t couch_document_get_meta(libcouch_document_t doc,
                                          const void **meta,
                                          size_t *nmeta);


    LIBCOUCH_API
    couch_error_t couch_document_get_revision(libcouch_document_t doc,
                                              uint64_t *revno);


    LIBCOUCH_API
    couch_error_t couch_document_get_deleted(libcouch_document_t doc, int *deleted);

    LIBCOUCH_API
    couch_error_t couch_document_get_value(libcouch_document_t doc,
                                           const void **value,
                                           size_t *nvalue);

    LIBCOUCH_API
    couch_error_t couch_document_get_content_type(libcouch_document_t doc,
                                                  uint8_t *content_type);

    LIBCOUCH_API
    couch_error_t couch_get_document(libcouch_t handle,
                                     const void *id,
                                     size_t nid,
                                     libcouch_document_t *doc);

    LIBCOUCH_API
    couch_error_t couch_store_document(libcouch_t handle,
                                       libcouch_document_t doc);

    LIBCOUCH_API
    couch_error_t couch_store_documents(libcouch_t handle,
                                        libcouch_document_t *doc,
                                        size_t ndocs);

    LIBCOUCH_API
    void couch_document_release(libcouch_document_t doc);

    LIBCOUCH_API
    couch_error_t couch_commit(libcouch_t handle);

    LIBCOUCH_API
    const char *couch_strerror(couch_error_t err);

#ifdef __cplusplus
}
#endif

#endif
