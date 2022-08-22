#pragma once
#ifndef _PGINF_MANAGER_IMPL_H_
#define _PGINF_MANAGER_IMPL_H_

#include "internal_global.h"
#include "host_impl.h"

#include <pginf/host.h>

PGINF_NAMESPACE_BEGIN()

class Library;
class Provider;
class Core_Impl;

/****************************************
 * @brief The impl of pginf::PluginManager
 * 
 ****************************************/
class Manager_Impl {
    friend class PluginManager;

    typedef struct _Library_Handle {
        PluginXMLInfo   xml_info_{};
        Library*        library_ = nullptr;
    } Library_Handle;

    using _Host             = Host;
    using _Provider         = Provider;
    using _Version          = unsigned int;
    using _Provider_Map     = std::map<std::string, _Provider*>;
    using _Library_Map      = std::map<std::string, _Library_Handle>;

    typedef bool _Fn_Register_Plugin(_Host&, std::string&); // Plugin registration function
    typedef void _Fn_Init_Core(Core_Impl*);                 // Plugin Pipe Init

    _Library_Map    libraries_;     // Map of the library
    _Host           manager_host_;  // Providers management for administrators

protected:
    /****************************************
     * @brief Default constructor.
     * Manager_Impl cannot be publicly instantiated.
     ****************************************/
    Manager_Impl();

    /****************************************
     * @brief Register a provider type
     * 
     * @param type Provider type.
     * @param cur_version Current version of that provider type.
     * @param lowest_version Lowest compatible version of that provider type.
     * @see Host::RegisterType()
     ****************************************/
    void RegisterType(const std::string& type, _Version cur_version, _Version lowest_version) {
        manager_host_.impl_->RegisterType(type, cur_version, lowest_version);
    }
    
    /****************************************
     * @brief Get providers of a certain type.
     * 
     * @param type Provider type.
     * @return Pointer to the list of providers of that \a type,
     * or nullptr if \a type is not registered.
     * @see Host::GetProviders()
     ****************************************/
    const _Provider_Map GetProviders(const std::string& type) const {
        return manager_host_.impl_->GetProviders(type);
    }

public:
    /****************************************
     * @brief Destroy the manager_impl object
     * 
     ****************************************/
    ~Manager_Impl();

    /****************************************
     * @brief Load a plugin given it's path
     * 
     * @param path_of_plugin Path for the plugin, including plugin name.
     * File extension may be included, but is discouraged for better cross platform code.
     * If file extension isn't present on the path, Pluma will deduce it from the operating system.
     * @param type_of_plugin the type of plugin
     * 
     * @return True if the plugin is successfully loaded.
     * 
     * @see Load(const std::string&, const std::string&)
     * @see LoadFromFolder
     * @see Unload
     * @see UnloadAll
     ****************************************/
    bool Load(const PluginXMLInfo& plugin_xml_info);

    /****************************************
     * @brief Unload a plugin.
     * 
     * @param plugin_name Name or path of the plugin.
     * 
     * @return True if the plugin is successfully unloaded,
     * false if no such plugin exists on the manager.
     * 
     * @see Load(const std::string&, const std::string&)
     * @see Load(const std::string&)
     * @see LoadFromFolder
     * @see UnloadAll
     ****************************************/
    bool Unload(const std::string& plugin_identifier);

    /****************************************
     * @brief Unload all loaded plugins.
     * 
     * @see Load(const std::string&, const std::string&)
     * @see Load(const std::string&)
     * @see LoadFromFolder
     * @see Unload
     ****************************************/
    void UnloadAll();

    /****************************************
     * @brief Get the loaded plugins object
     * 
     * @param plugin_names A vector to fill with the plugins names.
     ****************************************/
    void GetLoadedPlugins(std::vector<std::string>& plugin_identifiers) const;

    /****************************************
     * @brief Check if a plug-in is loaded.
     * 
     * @param plugin_name the plug-in tname o check.
     * @return True if the plug-in is successfully loaded.
     ****************************************/
    bool IsLoaded(const std::string& plugin_identifier) const;
    
};

PGINF_NAMESPACE_END()

#endif // !_PGINF_MANAGER_IMPL_H_