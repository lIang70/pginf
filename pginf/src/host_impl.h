#pragma once
#ifndef _PGINF_HOST_IMPL_H_
#define _PGINF_HOST_IMPL_H_

#include "global.h"

PGINF_NAMESPACE_BEGIN()

/****************************************
 * @brief The impl of pginf::Host
 * 
 ****************************************/
class Host_Impl {
    friend class Manager_Impl;
    friend class Provider;
    friend class Host;

    using _Provider         = Provider;
    using _Version          = unsigned int;
    using _Provider_Map     = std::map<std::string, _Provider*>;
    using _Provider_Pair    = std::pair<std::string, _Provider*>;

    class ProviderInfo {
    public:
        _Version        cur_version_;
        _Version        lowest_version_;
        _Provider_Map   providers_{};
    };

    using _Providers_Map        = std::map<std::string, ProviderInfo>;
    using _Temp_Providers_Map   = std::map<std::string, std::list<_Provider_Pair>>;

private:
    _Providers_Map      known_type_provider_;   // 
    _Temp_Providers_Map request_for_addition_;  // Provisional Provider List

private:
    /****************************************
     * @brief Construct a new Host_Impl object.
     * New Host_Impl instances are not publicly allowed.
     ****************************************/
    Host_Impl();

    /****************************************
     * @brief Destroy the Host_Impl object.
     * Clears all hosted providers
     ****************************************/
    ~Host_Impl();

    /****************************************
     * @brief Clears all hosted providers.
     * 
     ****************************************/
    void ClearProviders();

    /****************************************
     * @brief Previous add calls are canceled.
     * Added providers are not stored.
     * 
     * @see add
     ****************************************/
    void CancelAddictions();

    /****************************************
     * @brief Previous add calls are confirmed.
     * Added providers are finally stored.
     * 
     * @return True if something was stored.
     * @see add
     ****************************************/
    bool ConfirmAddictions();
    
    /****************************************
     * @brief Check provider type and version.
     * 
     * @param provider 
     * @return True if the provider is acceptable.
     ****************************************/
    bool CheckProvider(_Provider * provider);

    /****************************************
     * @brief Ckeck if a provider type is registered.
     * 
     * @param type Provider type id.
     * @return True if the type is registered 
     ****************************************/
    bool CheckType(const std::string & type) const; 

    /****************************************
     * @brief Get lowest compatible version of a type of providers.
     * 
     * @param type Provider type.
     * @return The lowest compatible version of the provider type.
     ****************************************/
    _Version GetLowestVersion(const std::string& type) const;

    /****************************************
     * @brief Get providers of a certain type.
     * 
     * @param type Provider type.
     * @return Pointer to the list of providers of that \a type,
     * or nullptr if \a type is not registered.
     ****************************************/
    const _Provider_Map GetProviders(const std::string& type) const;

    /****************************************
     * 
     * @brief Get version of a type of providers.
     * 
     * @param type Provider type.
     * @return The version of the provider type.
     ****************************************/
    _Version GetVersion(const std::string& type) const;

    /****************************************
     * @brief Register a type of providers.
     * 
     * @param type Provider type.
     * @param version Current version of that provider type.
     * @param lowest_version Lowest compatible version of that provider type.
     ****************************************/
    void RegisterType(const std::string& type, _Version register_version, _Version lowest_version);

    /****************************************
     * @brief Add provider.
     * 
     * Provider type and version are checked. Only known and
     * valid provider types are accepted.
     * 
     * @param provider_identifier Provider's name.
     * @param provider Provider to be added.
     * @return True if the provider is accepted.
     ****************************************/
    bool Add(std::string provider_identifier, _Provider * provider);

    /****************************************
     * @brief Delete provider
     * 
     * @param type  Provider's type
     * @param provider_identifier Provider's name.
     ****************************************/
    void Delete(std::string type, std::string provider_identifier);
};

PGINF_NAMESPACE_END()

#endif // !_PGINF_HOST_IMPL_H_