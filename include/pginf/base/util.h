#ifndef _PGINF_BASE_UTIL_H_
#define _PGINF_BASE_UTIL_H_

#include <pginf/base/system_check.h>

#include <list>
#include <string.h>
#include <string>

#ifdef P_OS_WIN32
#ifndef PGINF_STATIC
// Windows platforms
#ifdef PGINF_EXPORTS
// From DLL side, we must export
#define PGINF_API __declspec(dllexport)
#else
// From client application side, we must import
#define PGINF_API __declspec(dllimport)
#endif
#else
// No specific directive needed for static build
#define PGINF_API
#endif
#else
// Other platforms don't need to define anything
#define PGINF_API
#endif

#ifdef __cplusplus
#define DLL_DECL extern "C" PGINF_API
#else
#define DLL_DECL PGINF_API
#endif

// Macros: Ext of plug-in
#ifdef P_OS_WIN32
#define PGINF_EXTENSION "dll"
#elif defined(P_OS_UNIX)
#define PGINF_EXTENSION "so"
#else
// unknown library file type
#error Unknown library file extension for this operating system.
#endif

#define POLITE_STR(X) #X
#define UNUSED(x) (void)(x)

namespace pginf {

inline PGINF_API void setZero(void* p, size_t n)
{
    memset(p, 0, n);
}

// Taken from google-protobuf stubs/common.h
//
// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
// http://code.google.com/p/protobuf/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Author: kenton@google.com (Kenton Varda) and others
//
// Contains basic types and utilities used by the rest of the library.

//
// Use implicit_cast as a safe version of static_cast or const_cast
// for upcasting in the type hierarchy (i.e. casting a pointer to Foo
// to a pointer to SuperclassOfFoo or casting a pointer to Foo to
// a const pointer to Foo).
// When you use implicit_cast, the compiler checks that the cast is safe.
// Such explicit implicit_casts are necessary in surprisingly many
// situations where C++ demands an exact type match instead of an
// argument type convertable to a target type.
//
// The From type can be inferred, so the preferred syntax for using
// implicit_cast is the same as for static_cast etc.:
//
//   implicit_cast<ToType>(expr)
//
// implicit_cast would have been part of the C++ standard library,
// but the proposal was submitted too late.  It will probably make
// its way into the language in the future.
template <typename To, typename From>
inline PGINF_API To implicit_cast(From const& f)
{
    return f;
}

namespace file {

    void PGINF_API listFiles(std::list<std::string>& list,
        const std::string& folder_path, const std::string& extension, bool recursive);

} // namespace file
} // namespace pginf

#endif // !_PGINF_BASE_UTIL_H_