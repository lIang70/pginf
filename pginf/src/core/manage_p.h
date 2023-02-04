#ifndef _PGINF_PLUGIN_MANAGE_P_H_
#define _PGINF_PLUGIN_MANAGE_P_H_

#include <pginf/plugin/manage.h>

#include "core/host.h"
#include "core/library.h"

#include <map>

namespace pginf {
namespace plugin {

    class ManagePrivate {
    public:
        struct library_info {
            std::string id_ {};
            std::string type_ {};
            std::shared_ptr<core::Library> library_ {};

            library_info() = default;
            library_info(const library_info&) = delete;
            library_info(library_info&& info)
            {
                id_.swap(info.id_);
                type_.swap(info.type_);
                library_.swap(info.library_);
            }
        };

        std::map<std::string, library_info> libraries_info_ {};
        core::Host main_host_ {};
        uint32_t plugin_cnt_ { 0 };
    };

} // namespace plugin
} // namespace pginf

#endif // _PGINF_PLUGIN_MANAGE_P_H_