#ifndef _PGINF_CORE_UTIL_H_
#define _PGINF_CORE_UTIL_H_

#include "base/types.h"

namespace pginf {
namespace core {
    class Host;
    class Library;

    using Fn_ImportPlugin = bool(Host&, std::string); // Plugin registration function

    extern std::shared_ptr<Library> loadPlugin(std::shared_ptr<Host>& host, const std::string& path);

} // namespace core
} // namespace plugin

#endif // !_PGINF_CORE_UTIL_H_