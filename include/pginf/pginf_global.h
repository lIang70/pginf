#pragma once
#ifndef _PGINF_GLOBAL_H_
#define _PGINF_GLOBAL_H_

// Macros: OS Check
#if !defined(SAG_COM) && (!defined(WINAPI_FAMILY) || WINAPI_FAMILY==WINAPI_FAMILY_DESKTOP_APP) && (defined(WIN64) || defined(_WIN64) || defined(__WIN64__))
#   define C_OS_WIN32
#   define C_OS_WIN64
#elif !defined(SAG_COM) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
#   if defined(WINAPI_FAMILY)
#       ifndef WINAPI_FAMILY_PC_APP
#           define WINAPI_FAMILY_PC_APP WINAPI_FAMILY_APP
#       endif
#       if defined(WINAPI_FAMILY_PHONE_APP) && WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
#           define C_OS_WINRT
#       elif WINAPI_FAMILY == WINAPI_FAMILY_PC_APP
#           define C_OS_WINRT
#       else
#           define C_OS_WIN32
#       endif
#   else
#       define C_OS_WIN32
#   endif
#elif defined(__linux__) || defined(__linux)
#   define C_OS_LINUX
#elif defined(__APPLE__) || defined(MACOSX) || defined(macintosh) || defined(Macintosh)
#   define C_OS_MACOS
#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
#   define C_OS_FREEBSD
#else
#   error "not support this OS"
#endif

#if defined(C_OS_WIN32) || defined(C_OS_WIN64) || defined(C_OS_WINRT)
#   define C_OS_WIN
#   if defined(_MSC_VER)
#       pragma warning (disable : 4710)
#   endif
#endif

// Macros: Define portable import/export
#if defined(C_OS_WIN)
#   ifndef PGINF_STATIC
// Windows platforms
#       ifdef PGINF_EXPORTS
// From DLL side, we must export
#           define PGINF_API __declspec(dllexport)
#       else
// From client application side, we must import
#           define PGINF_API __declspec(dllimport)
#       endif
#   else
// No specific directive needed for static build
#define PGINF_API
#   endif
#else
// Other platforms don't need to define anything
#   define PGINF_API
#endif

// Macros: Portable import/export for plug-in registration
#if defined(C_OS_WIN)
#   ifndef PGINF_STATIC_PLUGIN
// Windows platforms
#       ifndef PLUGIN_IMPORTS
// From DLL side, we must export
#           ifdef __cplusplus
#               define PLUGIN_CONNECTOR extern "C" __declspec(dllexport)
#           else
#               define PLUGIN_CONNECTOR __declspec(dllexport)
#           endif
#       else
// From client application side, we must import
#           ifdef __cplusplus
#               define PLUGIN_CONNECTOR extern "C" __declspec(dllimport)
#           else
#               define PLUGIN_CONNECTOR __declspec(dllimport)
#           endif
#       endif
#   else
// Only define the extern "C" for static build
#       ifdef __cplusplus
#           define PLUGIN_CONNECTOR extern "C"
#       else
#           define PLUGIN_CONNECTOR
#       endif
#   endif
#else
// Only define the extern "C" for other platforms
#   ifdef __cplusplus
#       define PLUGIN_CONNECTOR extern "C"
#   else
#       define PLUGIN_CONNECTOR
#   endif
#endif

// Macros: namespace
#define NAMESPACE_BEGIN(NAME)   namespace NAME {
#define NAMESPACE_END()         }

#ifndef PGINF_USING
#define PGINF_NAMESPACE_BEGIN()
#define PGINF_NAMESPACE_END()  
#else
#define PGINF_NAMESPACE_BEGIN() NAMESPACE_BEGIN(pginf)
#define PGINF_NAMESPACE_END()   NAMESPACE_END()
#endif

// Header
#include <array>
#include <map>
#include <memory>
#include <string>
#include <typeinfo>
#include <type_traits>
#include <vector>

#endif // !_PGINF_GLOBAL_H_