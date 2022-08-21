#pragma once
#ifndef _PGINF_PROVIDER_H_
#define _PGINF_PROVIDER_H_

#include <pginf/pginf_global.h>

PGINF_NAMESPACE_BEGIN()

/****************************************
 * @brief Interface to provide applications with objects from plugins.
 * 
 ****************************************/
class PGINF_API Provider {
    friend class Host_Impl;

    using _Version  = unsigned int;

private:
    /****************************************
     * @brief Get object type.
     * Each provider defined on the Host application is identified by
     * a unique type. Those types are automatically managed internally by
     * pginf.
     * 
     * @return Provider type id.
     ****************************************/
    virtual const std::string get_provider_type() const = 0;
    
public:
    /****************************************
     * @brief Construct a new provider object
     * 
     ****************************************/
    Provider() = default;

    /****************************************
     * @brief Destructor for provider object
     * 
     ****************************************/
    virtual ~Provider() = default;

    /****************************************
     * @brief Get provider version.
     * 
     * @return Version number.
     ****************************************/
    virtual const _Version get_inter_version() const = 0;

    /****************************************
     * @brief Check compatibility with Host.
     * 
     * The same provider may be compiled with different versions
     * on Host side and on plugins side. This function checks if
     * a plugin provider is compatible with the current version of
     * the same provider type on the Host side.
     * 
     * @param Host Host, proxy of Host application.
     * @return True if it's compatible with \a Host.
     ****************************************/
    bool IsCompatible(const Host_Impl* host_impl) const;
};

PGINF_NAMESPACE_END()  

#endif // !_PGINF_PROVIDER_H_