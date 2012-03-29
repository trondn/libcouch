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
#ifndef LIBCOUCH_VISIBILITY_H
#define LIBCOUCH_VISIBILITY_H 1

#if defined(LIBCOUCH_INTERNAL)

#ifdef __SUNPRO_C
#define LIBCOUCH_API __global
#elif defined(HAVE_VISIBILITY) && HAVE_VISIBILITY
#define LIBCOUCH_API __attribute__ ((visibility("default")))
#elif defined(_MSC_VER)
#define LIBCOUCH_API extern __declspec(dllexport)
#else
#define LIBCOUCH_API
#endif

#else

#ifdef _MSC_VER
#define LIBCOUCH_API extern __declspec(dllimport)
#else
#define LIBCOUCH_API
#endif

#endif

#endif
