#ifndef _PGINF_CORE_UTIL_H_
#define _PGINF_CORE_UTIL_H_

#include <pginf/plugin/manage.h>

#include <memory>

namespace pginf {
namespace core {
    class Library;

    using Fn_ImportPlugin = bool(plugin::Manage&, std::string); // Plugin registration function

    extern std::shared_ptr<Library> loadPlugin(std::shared_ptr<plugin::Manage>& manage, const std::string& path);

} // namespace core
} // namespace plugin

#endif // !_PGINF_CORE_UTIL_H_