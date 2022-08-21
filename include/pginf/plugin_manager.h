#pragma once
#ifndef _PGINF_PLUGIN_MANAGER_H_
#define _PGINF_PLUGIN_MANAGER_H_

#include <pginf/pginf_global.h>
#include <pginf/host.h>

PGINF_NAMESPACE_BEGIN()

class Manager_Impl;
class Provider;

/****************************************
 * @brief Manages loaded plugins.
 * 
 ****************************************/
class PGINF_API PluginManager {
    using _Version       = unsigned int;
    using _Provider_Map  = std::map<std::string, Provider*>;

private:
    Manager_Impl *  impl_       = nullptr;

protected:
    /****************************************
     * @brief Default constructor.
     * PluginManager cannot be publicly instantiated.
     ****************************************/
    PluginManager();

    /****************************************
     * @brief Register a provider type
     * 
     * @param type Provider type.
     * @param cur_version Current version of that provider type.
     * @param lowest_version Lowest compatible version of that provider type.
     ****************************************/
    void RegisterType(const std::string& type, _Version cur_version, _Version lowest_version) const;
    
    /****************************************
     * @brief Get providers of a certain type.
     * 
     * @param type Provider type.
     * @return Pointer to the list of providers of that \a type,
     * or nullptr if \a type is not registered.
     ****************************************/
    const _Provider_Map GetProviders(const std::string& type) const;

public:
    /****************************************
     * @brief Gets the folder where the current program resides.
     * 
     * @return The path of folder.
     ****************************************/
    static std::string GetCurrentFolder();

    /****************************************
     * @brief Destroy the plugin manager object
     * 
     ****************************************/
    ~PluginManager();

    /****************************************
     * @brief Load a plugin given it's path
     * 
     * @param path_of_xml Path for the plugin's xml, including plugin name. 
     * File extension may be included, but is discouraged for better cross platform code.
     * If file extension isn't present on the path, Pluma will deduce it from the operating system.
     * 
     * @return True if the plugin is successfully loaded.
     * 
     * @see Load(const std::string&, const std::string&)
     * @see LoadFromFolder
     * @see Unload
     * @see UnloadAll
     ****************************************/
    bool Load(const std::string& path_of_xml);

    /****************************************
     * @brief Load all plugins from a given folder
     * 
     * @param folder Path for the folder where the plug-ins are.
     * @param recursive If true it will search on sub-folders as well
     * 
     * @return Number of successfully loaded plug-ins.
     * 
     * @see Load(const std::string&, const std::string&)
     * @see Load(const std::string&)
     * @see Unload
     * @see UnloadAll
     ****************************************/
    int  LoadFromFolder(const std::string& folder, bool recursive = false);

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
     * @param plugin_identifiers A vector to fill with the plugins names.
     ****************************************/
    void GetLoadedPlugins(std::vector<std::string>& plugin_identifiers) const;

    /****************************************
     * @brief Check if a plug-in is loaded.
     * 
     * @param plugin_identifier the plug-in tname o check.
     * @return True if the plug-in is successfully loaded.
     ****************************************/
    bool IsLoaded(const std::string& plugin_identifier) const;

    /****************************************
     * @brief Set the logger path object
     * 
     * @param logger_path 
     * @return True if the path of log has been set.
     ****************************************/
    bool SetLogFilePath(const std::string& logger_path) const;

    /****************************************
     * @brief The type of provider accepted
     * 
     * @tparam Type of Registration
     ****************************************/
    template<typename _Provider_Type>
    void AcceptProviderType() {
        RegisterType (
            _Provider_Type::pginf_provider_type,
            _Provider_Type::pginf_inter_version,
            _Provider_Type::pginf_inter_lowest_version
        );
    }

    /****************************************
     * @brief Get the providers object
     * 
     * @tparam Type of Registration
     * @return Pointer to the list of providers of that \a type,
     * or nullptr if \a type is not registered.
     ****************************************/
    template<typename _Provider_Type>
    const _Provider_Map GetProviders() const {
        return GetProviders ( _Provider_Type::pginf_provider_type );
    }

};

PGINF_NAMESPACE_END()

#endif // !_PGINF_PLUGIN_MANAGER_H_