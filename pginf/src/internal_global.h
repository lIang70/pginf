#pragma once
#ifndef _INTERNAL_GLOBAL_H_
#define _INTERNAL_GLOBAL_H_

#include <pginf/pginf_global.h>
#include <pginf/pflags.h>

// Macros: Ext of plug-in
#ifdef C_OS_WIN
#   define PGINF_EXTENSION "dll"
#elif defined(C_OS_MACOS)
#   define PGINF_EXTENSION "dylib"
#elif defined(C_OS_UNIX) || defined(C_OS_FREEBSD)
#   define PGINF_EXTENSION "so"
#else
   // unknown library file type
#   error Unknown library file extension for this operating system
#endif

// Predefined Header file
#include <condition_variable>
#include <cstdlib>
#include <future>
#include <io.h>
#include <list>
#include <mutex>
#include <thread>

#ifdef C_OS_WIN
#include <Windows.h>
#endif

PGINF_NAMESPACE_BEGIN()

extern FILE * g_log_file;
// Macros: Outputting Log Information
#define LOGGER(format, ...) \
    do { \
        fprintf(g_log_file, format##"\n", __VA_ARGS__); \
        fflush(g_log_file); \
    } while (0)

/****** plugin xml example ******
 * <plugin type="interface" id="${name}.${version}">
 *   ... define some plugin parameters         
 *   <name>${name}</name>
 *   <version>${version}</version>
 *   <uninstall_need_reload>true</uninstall_need_reload>
 *   <need_load>true</need_load>
 * </plugin>
 ********************************/

typedef enum _Plugin_Flag {
    PLUGIN_DEFAULT          = 0,
    UNINSTALL_NEED_RELOAD   = (1<<0),
    PLUGIN_NEED_LOAD        = (1<<1),
} Plugin_Flag;

P_DECLARE_FLAGS(PluginFlag, Plugin_Flag)

/****************************************
 * @brief The information of plugin read
 * from XML.
 ****************************************/
typedef struct _Plugin_XML_Info {
    std::string     identifier_{};
    std::string     type_{};
    std::string     name_{};
    std::string     folder_{};
    std::string     xml_path_{};
    std::string     absolute_path_{};
    std::string     version_{};
    PluginFlag      plugin_flag_ = Plugin_Flag::PLUGIN_DEFAULT;
} PluginXMLInfo;

// assert
static 
void p_assert(const char *assertion, const char *file, int line) noexcept {
    LOGGER("ASSERT: \"%s\" in file %s, line %d", assertion, file, line);
#ifdef C_OS_WIN
    TerminateProcess(GetCurrentProcess(), STATUS_FATAL_APP_EXIT);
#else
    std::abort();
#endif
}

#ifdef PGINF_DEBUG
#   define PGINF_ASSERT(condition) ((condition) ? static_cast<void>(0) : pginf::p_assert(#condition, __FILE__, __LINE__))
#else
#   define PGINF_ASSERT(condition) static_cast<void>(false && (condition))
#endif // PGINF_DEBUG

PGINF_NAMESPACE_END()


#endif // !_INTERNAL_GLOBAL_H_