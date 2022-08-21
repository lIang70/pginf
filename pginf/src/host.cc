#include <pginf/host.h>

#include "host_impl.h"

PGINF_NAMESPACE_BEGIN()

Host::Host() 
    : impl_(new Host_Impl) {

}

Host::~Host() {
    delete impl_;
}

bool 
Host::Add(std::string provider_identifier, Provider * provider) {
    return impl_->Add(provider_identifier, provider);
}

void
Host::Delete(std::string type, std::string provider_identifier) {
    impl_->Delete(type, provider_identifier);
}

PGINF_NAMESPACE_END()  
