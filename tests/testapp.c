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
#include <stdlib.h>
#include <stdio.h>
#include <libcouch/couch.h>
#include <errno.h>
#include <string.h>

const char *dbfile = "testcase.couch";

typedef int (*testcase)(void);
struct test {
    const char *d;
    testcase t;
};

static int open_empty_filename(void)
{
    libcouch_t handle;
    libcouch_error_t err = couch_open_handle("", COUCH_OPEN_RDONLY, &handle);
    if (err != COUCH_ERROR_ENOENT) {
        return 1;
    }

    return 0;
}

static int create_database(void)
{
    libcouch_t handle;
    libcouch_error_t err = couch_open_handle(dbfile,
                                             COUCH_OPEN_RDONLY,
                                             &handle);
    if (err != COUCH_ERROR_ENOENT) {
        return 1;
    }

    err = couch_open_handle(dbfile, COUCH_OPEN_RW, &handle);
    if (err != COUCH_SUCCESS) {
        return 1;
    }

    couch_close_handle(handle);
    err = couch_open_handle(dbfile, COUCH_OPEN_RDONLY, &handle);
    if (err != COUCH_SUCCESS) {
        return 1;
    }
    couch_close_handle(handle);

    return 0;
}

static int get_miss(void)
{
    libcouch_t handle;
    libcouch_document_t doc;
    libcouch_error_t err;

    err = couch_open_handle(dbfile, COUCH_OPEN_RW, &handle);
    if (err != COUCH_SUCCESS) {
        return 1;
    }

    err = couch_get_document(handle, "hi-there", sizeof("hi-there"), &doc);
    if (err != COUCH_ERROR_ENOENT) {
        couch_close_handle(handle);
        return 1;
    }

    couch_close_handle(handle);
    return 0;
}

static int store_document(void)
{
    libcouch_t handle;
    libcouch_document_t doc;
    libcouch_error_t err;

    err = couch_open_handle(dbfile, COUCH_OPEN_RW, &handle);
    if (err != COUCH_SUCCESS) {
        return 1;
    }

    err = couch_create_empty_document(&doc);
    if (err != COUCH_SUCCESS) {
        return 1;
    }

    err = couch_document_set_id(doc, "hi-there", sizeof("hi-there"), 0);
    if (err != COUCH_SUCCESS) {
        return 1;
    }

    err = couch_document_set_revision(doc, 0);
    if (err != COUCH_SUCCESS) {
        return 1;
    }

    err = couch_document_set_value(doc, "hei", 3, 0);
    if (err != COUCH_SUCCESS) {
        return 1;
    }

    err = couch_store_document(handle, doc);
    if (err != COUCH_SUCCESS) {
        couch_close_handle(handle);
        return 1;
    }

    couch_commit(handle);
    couch_document_release(doc);

    err = couch_get_document(handle, "hi-there", sizeof("hi-there"), &doc);
    if (err != COUCH_SUCCESS) {
        couch_close_handle(handle);
        return 1;
    }

    couch_close_handle(handle);
    return 0;
}



static void remove_dbfiles(void)
{
    if (remove(dbfile) == -1 && errno != ENOENT) {
        fprintf(stderr, "Failed to remove test case files: %s\n",
                strerror(errno));
        exit(EXIT_FAILURE);
    }
}

int main(void)
{
    static struct test tests[] = {
        { .d = "open empty filename", .t = open_empty_filename },
        { .d = "test create database", .t = create_database },
        { .d = "get miss database", .t = get_miss },
        { .d = "store document", .t = store_document },
        { .d = NULL, .t = NULL }
    };
    char line[80];
    int ii = 0;
    int total;

    memset(line, ' ', sizeof(line));
    line[sizeof(line) - 1] = '\0';

    while (tests[ii].d != NULL) {
        ++ii;
    }

    total = ii;
    remove_dbfiles();
    for (ii = 0; ii < total; ++ii) {
        fprintf(stdout, "\r%s\r%s - [%d/%d]: ", line, tests[ii].d,
                ii + 1, total);
        fflush(stdout);
        if (tests[ii].t() != 0) {
            fprintf(stdout, "FAILED\n");
        }
        remove_dbfiles();
    }
    fprintf(stdout, "SUCCESS\n");
    exit(EXIT_SUCCESS);
}
