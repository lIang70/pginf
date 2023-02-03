#include <pginf/plugin/host.h>
#include <pginf/plugin/provider.h>

namespace pginf {

bool Provider::isCompatible(const Host* host) const
{
    if (!host)
        return false;

    // check compatibility with host
    const std::string& type = this->getProviderType();
    if (!host->checkType(type))
        return false;

    unsigned int lowest = host->getLowestVersion(type);
    unsigned int current = host->getVersion(type);
    unsigned int my_version = this->getInterVersion();

    return (lowest <= my_version) && (my_version <= current);
}

} // namespace pginf

#include <pginf/plugin/interface.hpp>
