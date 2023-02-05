#include "core/host.h"

namespace pginf {
namespace core {

    bool Host::add(std::string& provider_id, std::shared_ptr<plugin::Provider> provider)
    {
        if (!provider) {
            LOG_ERROR() << "Trying to add a NULL provider.";
            return false;
        }
        if (!checkProvider(provider)) {
            auto p = provider.get();
            delete p;
            return false;
        }

        auto& list = request_for_addition_[provider->getProviderType()];
        list.push_back(std::make_pair(provider_id, provider));
        return true;
    }

    void Host::remove(std::string& type, const std::string& provider_id)
    {
        if (known_type_providers_.find(type) != known_type_providers_.end()) {
            known_type_providers_[type].providers_.erase(provider_id);
        }
    }

    void Host::registerType(const std::string& type,
        Version register_version, Version lowest_version)
    {
        if (!checkType(type)) {
            provide_manage pm;
            pm.cur_version_ = register_version;
            pm.lowest_version_ = lowest_version;
            known_type_providers_.insert(std::make_pair(type, std::move(pm)));
        } else {
            LOG_WARNING() << "Type[" << type << "] has already registered.";
        }
    }

    bool Host::confirmAddictions()
    {
        if (request_for_addition_.empty()) {
            LOG_WARNING() << "No provider needs to be added";
            return false;
        }

        for (auto& ppair : request_for_addition_) {
            auto list = ppair.second;
            for (auto& provide : list) {
                // Move the memory location of share_ptr from DLL to Host.
                decltype(provide.second) tmp(provide.second.get());
                known_type_providers_[ppair.first].providers_.insert(std::make_pair(provide.first, tmp));
            }
        }

        // clear map
        cancelAddictions();
        return true;
    }

    bool Host::checkProvider(std::shared_ptr<plugin::Provider>& provider)
    {
        const std::string& type = provider->getProviderType();
        if (!checkType(type)) {
            LOG_ERROR() << "provider type[" << type << "] isn't registered.";
            return false;
        }
        if (!isCompatible(provider)) {
            LOG_ERROR() << "Incompatible provider[" << type << "] version.";
            return false;
        }
        return true;
    }

    bool Host::checkType(const std::string& type) const
    {
        return known_type_providers_.find(type) != known_type_providers_.end();
    }

    Host::Version Host::getLowestVersion(const std::string& type) const
    {
        auto iter = known_type_providers_.find(type);
        if (iter != known_type_providers_.end())
            return iter->second.lowest_version_;
        return (0);
    }

    Host::Version Host::getVersion(const std::string& type) const
    {
        auto iter = known_type_providers_.find(type);
        if (iter != known_type_providers_.end())
            return iter->second.cur_version_;
        return (0);
    }

    bool Host::isCompatible(std::shared_ptr<Provider>& provider) const
    {
        if (!provider)
            return false;

        // check compatibility
        const std::string& type = provider->getProviderType();
        if (!checkType(type)) {
            LOG_WARNING() << "You need to register the plug-in type.";
            return false;
        }

        unsigned int lowest = getLowestVersion(type);
        unsigned int current = getVersion(type);
        unsigned int my_version = provider->getInterVersion();

        return (lowest <= my_version) && (my_version <= current);
    }

} // namespace core
} // namespace pginf
