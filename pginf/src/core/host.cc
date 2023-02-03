#include "core/host_p.h"

namespace pginf {

bool Host::add(std::string provider_identifier, std::shared_ptr<Provider> provider)
{
    if (!provider) {
        LOG_ERROR() << "Trying to add a null provider.";
        return false;
    }
    if (!checkProvider(provider)) {
        return false;
    }

    auto& list = p_->request_for_addition_[provider->getProviderType()];
    list.emplace_back(std::make_pair(provider_identifier, provider));
    return true;
}

void Host::remove(std::string type, std::string provider_identifier)
{
    if (p_->known_type_providers_.find(type) != p_->known_type_providers_.end()) {
        p_->known_type_providers_[type].providers_.erase(provider_identifier);
    }
}

bool Host::checkProvider(std::shared_ptr<Provider>& provider)
{
    const std::string& type = provider->getProviderType();
    if (!checkType(type)) {
        LOG_ERROR() << "provider type[" << type << "] isn't registered.";
        return false;
    }
    if (!provider->isCompatible(this)) {
        LOG_ERROR() << "Incompatible provider[" << type << "] version.";
        return false;
    }
    return true;
}

void Host::registerType(const std::string& type,
    Provider::Version register_version, Provider::Version lowest_version)
{
}

bool Host::checkType(const std::string& type) const
{
    return p_->known_type_providers_.find(type) != p_->known_type_providers_.end();
}

Provider::Version Host::getLowestVersion(const std::string& type) const
{
    auto iter = p_->known_type_providers_.find(type);
    if (iter != p_->known_type_providers_.end())
        return iter->second.lowest_version_;
    return (0);
}

Provider::Version Host::getVersion(const std::string& type) const
{
    auto iter = p_->known_type_providers_.find(type);
    if (iter != p_->known_type_providers_.end())
        return iter->second.cur_version_;
    return (0);
}

Host::Host()
    : p_(new HostPrivate)
{
}

Host::~Host()
{
    delete p_;
}

} // namespace pginf
