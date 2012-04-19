#
#     Copyright 2012 Couchbase, Inc.
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#

ACLOCAL_AMFLAGS = -I m4 --force
lib_LTLIBRARIES = libcouch.la

pkginclude_HEADERS = \
                     include/libcouch/couch.h \
                     include/libcouch/types.h \
                     include/libcouch/visibility.h

libcouch_la_CPPFLAGS = $(AM_CPPFLAGS) -DLIBCOUCH_INTERNAL=1
libcouch_la_SOURCES = src/document.c src/error.c src/instance.c src/internal.h
libcouch_la_LDFLAGS = $(AM_LDFLAGS) -lcouchstore

check_PROGRAMS = tests/test_open_empty_filename \
                 tests/test_create_database tests/test_get_miss \
                 tests/test_store_single_document tests/test_get_hit \
                 tests/test_delete_document \
                 tests/test_delete_nonexistent_document \
                 tests/test_get_deleted_document \
                 tests/test_bulk_store_documents \
                 tests/test_changes_since

TESTS=${check_PROGRAMS}

tests_test_open_empty_filename_SOURCES = tests/testapp.c
tests_test_open_empty_filename_LDFLAGS = libcouch.la

tests_test_create_database_SOURCES = tests/testapp.c
tests_test_create_database_LDFLAGS = libcouch.la

tests_test_get_hit_SOURCES = tests/testapp.c
tests_test_get_hit_LDFLAGS = libcouch.la

tests_test_get_miss_SOURCES = tests/testapp.c
tests_test_get_miss_LDFLAGS = libcouch.la

tests_test_store_single_document_SOURCES = tests/testapp.c
tests_test_store_single_document_LDFLAGS = libcouch.la

tests_test_delete_document_SOURCES = tests/testapp.c
tests_test_delete_document_LDFLAGS = libcouch.la

tests_test_delete_nonexistent_document_SOURCES = tests/testapp.c
tests_test_delete_nonexistent_document_LDFLAGS = libcouch.la

tests_test_get_deleted_document_SOURCES = tests/testapp.c
tests_test_get_deleted_document_LDFLAGS = libcouch.la

tests_test_bulk_store_documents_SOURCES = tests/testapp.c
tests_test_bulk_store_documents_LDFLAGS = libcouch.la

tests_test_changes_since_SOURCES = tests/testapp.c
tests_test_changes_since_LDFLAGS = libcouch.la


LINTFLAGS=-Iinclude -b -c -errchk=%all \
          -erroff=E_INCL_NUSD,E_CAST_INT_TO_SMALL_INT,E_PTRDIFF_OVERFLOW  \
          -errtags=yes -errhdr=%user \
          -errsecurity=extended -fd -Ncheck=%all -Nlevel=4 -n -Xc99=none
lint:
	$(LINT.c) $(libcouch_la_SOURCES)

reformat:
	astyle --mode=c \
               --quiet \
               --style=1tbs \
               --indent=spaces=4 \
               --indent-namespaces \
               --indent-col1-comments \
               --max-instatement-indent=78 \
               --pad-oper \
               --pad-header \
               --add-brackets \
               --unpad-paren \
               --align-pointer=name \
               --align-reference=name \
               $(top_srcdir)/include/libcouch/*.[ch] \
               $(top_srcdir)/src/*.[ch] \
               $(top_srcdir)/tests/*.[ch]

