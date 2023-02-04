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
        std::map<std::string, std::shared_ptr<core::Library>> libraries_;
        core::Host main_host_;

    };

} // namespace plugin
} // namespace pginf

#endif // _PGINF_PLUGIN_MANAGE_P_H_