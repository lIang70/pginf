#pragma once
#ifndef _PGINF_INTERFACE_HPP_
#define _PGINF_INTERFACE_HPP_

#include <pginf/host.h>
#include <pginf/provider.h>
#include <pginf/pipe.h>

// Macro: Convert any parameter to string
#define PGINF_PARA2STRING(X) #X

PGINF_NAMESPACE_BEGIN()
    class PluginManager;
    class Core_Impl;
PGINF_NAMESPACE_END()

// Macro: Create Provider.
#define PGINF_PROVIDER_CREATE(object) \
class object##_Provider: public pginf::Provider { \
    friend class pginf::PluginManager; \
    friend class object; \
    using _Version  = unsigned int; \
    using _Type     = std::string; \
    using _Object   = object; \
private: \
    static  const _Version  pginf_inter_version; \
    static  const _Version  pginf_inter_lowest_version; \
    static  const _Type     pginf_provider_type; \
public: \
    inline  const _Type     get_provider_type() const override { return pginf_provider_type; } \
    inline  const _Version  get_inter_version() const override { return pginf_inter_version; } \
    virtual       _Object * Create() const = 0; \
};

// Marcro: Init Provider type, version and lowest_version
#define PGINF_PROVIDER_INIT(object_type, type, version, lower_version) \
const object_type##_Provider::_Version object_type##_Provider::pginf_inter_version         = version; \
const object_type##_Provider::_Version object_type##_Provider::pginf_inter_lowest_version  = lower_version; \
const object_type##_Provider::_Type    object_type##_Provider::pginf_provider_type         = PGINF_PARA2STRING(type); \

// Marcro: Help plugins generating their Provider implementations
#define PGINF_PROVIDER_INHERIT(specialized_object, base_object) \
class specialized_object##_Provider: public base_object##_Provider{ \
public: base_object * Create() const override { return new specialized_object; } \
};

PGINF_NAMESPACE_BEGIN()

/****************************************
 * @brief STI of pginf.
 * 
 ****************************************/
class Interface {

public:
    /****************************************
     * @brief Destroy the Interface object.
     * 
     ****************************************/
    virtual ~Interface() = default;

    /****************************************
     * @brief Get the description of interface.
     * 
     * @return Description of interface. 
     ****************************************/
    virtual std::string get_description() const = 0;

    /****************************************
     * @brief Get the name of interface.
     * 
     * @return Name of interface.
     ****************************************/
    virtual std::string get_name() const = 0;

};

PGINF_PROVIDER_CREATE(Interface)
PGINF_PROVIDER_INIT(Interface, interface, 1, 1)

PGINF_NAMESPACE_END()

// Macro: Create connect of plug-in
#define PGINF_CONNECTOR_CREATE(specialized_object) \
PLUGIN_CONNECTOR bool pginf_import(pginf::Host& host, std::string plugin_name) { return host.Add(plugin_name, new specialized_object##_Provider()); };

// Macro: Define identifier of plug-in
#define PGINF_DEFINE_PLUGINID(specialized_object_id) \
PLUGIN_CONNECTOR void get_id(std::string& plugin_identifier) { plugin_identifier = PGINF_PARA2STRING(specialized_object_id); }

// Macro: Define initialization of plug-in's core
#define PGINF_CORE_INITIALIZE() \
PLUGIN_CONNECTOR void core_init(pginf::Core_Impl * core_impl) { pginf::Pipe::PulginInitImpl(core_impl); }

#endif // !_PGINF_INTERFACE_HPP_