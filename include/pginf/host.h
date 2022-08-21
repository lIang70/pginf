#pragma once
#ifndef _PGINF_HOST_H_
#define _PGINF_HOST_H_

#include <pginf/pginf_global.h>

PGINF_NAMESPACE_BEGIN()

class Host_Impl;

/****************************************
 * @brief Manages providers.
 * 
 ****************************************/
class PGINF_API Host {
    friend class Manager_Impl;
    friend class Provider;

private:
    Host_Impl* impl_ = nullptr;

private:
    /****************************************
     * @brief Construct a new Host object.
     * New Host instances are not publicly allowed.
     ****************************************/
    Host();

    /****************************************
     * @brief Destroy the Host object.
     * Clears all hosted providers
     ****************************************/
    ~Host();

public:
    /****************************************
     * @brief Add provider.
     * 
     * Provider type and version are checked. Only known and
     * valid provider types are accepted.
     * 
     * @param provider_identifier Provider's identifier.
     * @param provider Provider to be added.
     * @return True if the provider is accepted.
     ****************************************/
    bool Add(std::string provider_identifier, Provider* provider);

    /****************************************
     * @brief Delete provider
     * 
     * @param type  Provider's type
     * @param provider_identifier Provider's identifier.
     ****************************************/
    void Delete(std::string type, std::string provider_identifier);

};

PGINF_NAMESPACE_END()

#endif // !_PGINF_HOST_H_