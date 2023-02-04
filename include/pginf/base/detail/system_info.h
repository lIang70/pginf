#ifndef _PGINF_BASE_UTIL_SYSTEM_INFO_H_
#define _PGINF_BASE_UTIL_SYSTEM_INFO_H_

#include <inttypes.h>
#include <string>

namespace pginf {
namespace util {

    extern std::string systemdir();
    extern int32_t pid();
    extern std::string hostname();

} // namespace util
} // namespace pginf

#endif // !_PGINF_BASE_UTIL_SYSTEM_INFO_H_