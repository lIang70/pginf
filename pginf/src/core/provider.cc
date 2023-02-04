#include <pginf/base/logging.h>
#include <pginf/plugin/host.h>
#include <pginf/plugin/provider.h>

namespace pginf {
namespace plugin {

    bool Provider::isCompatible(const Host* host) const
    {
        if (!host)
            return false;

        // check compatibility with host
        const std::string& type = this->getProviderType();
        if (!host->checkType(type)) {
            LOG_WARNING() << "You need to register the plug-in type.";
            return false;
        }

        unsigned int lowest = host->getLowestVersion(type);
        unsigned int current = host->getVersion(type);
        unsigned int my_version = this->getInterVersion();

        return (lowest <= my_version) && (my_version <= current);
    }

} // namespace plugin
} // namespace pginf
