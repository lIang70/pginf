#include "host_impl.h"

#include <pginf/provider.h>

PGINF_NAMESPACE_BEGIN()

Host_Impl::Host_Impl() 
    : known_type_provider_(_Providers_Map()),
    request_for_addition_(_Temp_Providers_Map()) {
    
}

Host_Impl::~Host_Impl() {
    ClearProviders();
    CancelAddictions();
    // map frees itself
}

void 
Host_Impl::ClearProviders() {
    for (auto providers_map_iter = known_type_provider_.begin(); 
         providers_map_iter != known_type_provider_.end(); 
         ++providers_map_iter) {
        auto & Providers = providers_map_iter->second.providers_;
        for (auto providers_iter = Providers.begin();
             providers_iter != Providers.end(); 
             ++providers_iter) {
            delete (providers_iter->second);
        }
        _Provider_Map().swap(Providers);
    }
}

void 
Host_Impl::CancelAddictions() {
    _Temp_Providers_Map::iterator temp_providers_map_iter;
    for (temp_providers_map_iter = request_for_addition_.begin(); 
         temp_providers_map_iter != request_for_addition_.end(); 
         ++temp_providers_map_iter) {
        std::list<_Provider_Pair> List = temp_providers_map_iter->second;
        for (auto provider_iter = List.begin(); 
             provider_iter != List.end(); 
             ++provider_iter) {
            delete (provider_iter->second);
        }
    }
    // clear map
    request_for_addition_.clear();
}

bool 
Host_Impl::ConfirmAddictions() {
    if (request_for_addition_.empty()) 
        return false;
    _Temp_Providers_Map::iterator temp_providers_map_iter;
    for (temp_providers_map_iter = request_for_addition_.begin(); 
         temp_providers_map_iter != request_for_addition_.end(); 
         ++temp_providers_map_iter) {
        std::list<_Provider_Pair> List = temp_providers_map_iter->second;
        for (auto provider_iter = List.begin();
             provider_iter != List.end(); 
             ++provider_iter) {
            known_type_provider_[temp_providers_map_iter->first].providers_.insert(*provider_iter);
        }
    }
    // clear map
    _Temp_Providers_Map().swap(request_for_addition_);
    return true;
}

bool 
Host_Impl::CheckProvider(_Provider * provider) {
    const std::string & type = provider->get_provider_type();
    if (!CheckType(type)) {
        LOGGER("[Error] %s provider type isn't registered.", type.c_str());
        return false;
    }
    if (!provider->IsCompatible(this)){
        LOGGER("[Error] Incompatible %s provider version.", type.c_str());
        return false;
    }
    return true;
}

bool 
Host_Impl::CheckType(const std::string & type) const { 
    return known_type_provider_.find(type) != known_type_provider_.end();
}

Host_Impl::_Version 
Host_Impl::GetLowestVersion(const std::string& type) const {
    auto iter = known_type_provider_.find(type);
    if (iter != known_type_provider_.end())
        return iter->second.lowest_version_;
    return 0;
}

const Host_Impl::_Provider_Map 
Host_Impl::GetProviders(const std::string& type) const {
    auto iter = known_type_provider_.find(type);
    if (iter != known_type_provider_.end())
        return iter->second.providers_;
    return _Provider_Map();
}

Host_Impl::_Version 
Host_Impl::GetVersion(const std::string& type) const {
    auto iter = known_type_provider_.find(type);
    if (iter != known_type_provider_.end())
        return iter->second.cur_version_;
    return 0;
}

void 
Host_Impl::RegisterType(const std::string& type, _Version register_version, _Version lowest_version) {
    if (!CheckType(type)) {
        ProviderInfo pi;
        pi.cur_version_     = register_version;
        pi.lowest_version_  = lowest_version;
        known_type_provider_[type] = pi;
    }
}

bool 
Host_Impl::Add(std::string provider_identifier, _Provider * provider) {
    if (!provider) {
        LOGGER("[Error] Trying to add a null provider.");
        return false;
    }
    if (!CheckProvider(provider)) {
        delete provider;
        return false;
    }

    auto & list = request_for_addition_[provider->get_provider_type()];
    list.emplace_back(std::make_pair(provider_identifier, provider));
    return true;
}

void
Host_Impl::Delete(std::string type, std::string provider_identifier) {
    if (known_type_provider_.find(type) != known_type_provider_.end()) {
        known_type_provider_[type].providers_.erase(provider_identifier);
    }
}

PGINF_NAMESPACE_END()