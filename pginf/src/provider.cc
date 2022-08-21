#include <pginf/provider.h>

#include "host_impl.h"

PGINF_NAMESPACE_BEGIN()

bool 
Provider::IsCompatible(const Host_Impl* host_impl) const {
    if (!host_impl)
        return false;

    // check compatibility with host
    const std::string & type = this->get_provider_type();
    if (!host_impl->CheckType(type))
        return false;

    unsigned int lowest = host_impl->GetLowestVersion(type);
    unsigned int current = host_impl->GetVersion(type);
    unsigned int my_version = this->get_inter_version();

    return (lowest <= my_version) && (my_version <= current);
}

PGINF_NAMESPACE_END()  
