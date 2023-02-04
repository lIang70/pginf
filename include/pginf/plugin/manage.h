#ifndef _PGINF_PLUGIN_MANAGE_H_
#define _PGINF_PLUGIN_MANAGE_H_

#include <pginf/base/util.h>
#include <pginf/plugin/provider.h>

#include <map>
#include <memory>
#include <vector>

namespace pginf {
namespace plugin {

    class ManagePrivate;
    class PGINF_API Manage : public std::enable_shared_from_this<Manage> {
        ManagePrivate* p_ { nullptr };

    public:
        virtual ~Manage();

        //!
        //! @brief Load a plugin given it's path
        //!
        //! @param path Path for the plug-in.
        //!
        //! @return True if the plugin is successfully loaded.
        //!
        //! @see load(const std::string&, const std::string&)
        //! @see loadFromFolder
        //! @see unload
        //! @see unloadAll
        //!
        bool loadPlugin(const std::string& path);

        //!
        //! @brief Unload a plugin.
        //!
        //! @param plugin_id identifier of the plugin.
        //!
        //! @return True if the plugin is successfully unloaded,
        //!  false if no such plugin exists on the manager.
        //!
        //! @see load(const std::string&, const std::string&)
        //! @see load(const std::string&)
        //! @see loadFromFolder
        //! @see unloadAll
        //!
        bool unload(const std::string& plugin_id);

        //!
        //! @brief Unload all loaded plugins.
        //!
        //! @see load(const std::string&, const std::string&)
        //! @see load(const std::string&)
        //! @see loadFromFolder
        //! @see unload
        //!
        void unloadAll();

        //!
        //! @brief Get the loaded plugins object
        //!
        //! @param plugin_ids A vector filled with the plugin's id.
        //!
        void getLoadedPlugins(std::vector<std::string>& plugin_ids) const;

        //!
        //! @brief Check if a plug-in is loaded.
        //!
        //! @param plugin_id the plug-in's id.
        //! @return True if the plug-in is successfully loaded.
        //!
        bool isLoaded(const std::string& plugin_id) const;

        //!
        //! @brief Import provider.
        //!
        //! Provider type and version are checked. Only known and
        //! valid provider types are accepted.
        //!
        //! @param provider_id Provider's identifier.
        //! @param provider Provider to be added.
        //! @return True if the provider is accepted.
        //!
        bool importProvider(std::string& plugin_id, std::shared_ptr<Provider> provider);

        //!
        //! @brief The type of provider accepted
        //!
        //! @tparam Type of Registration
        //!
        template <typename Provider>
        void acceptProviderType()
        {
            registerType(
                Provider::PGINF_PROVIDER_TYPE,
                Provider::PGINF_INTER_VERSION,
                Provider::PGINF_INTER_LOWEST_VERSION);
        }

        //!
        //! @brief Get the providers object
        //!
        //! @tparam Type of Registration
        //! @return Pointer to the list of providers of that \a type,
        //! or nullptr if \a type is not registered.
        //!
        template <typename Provider>
        const std::map<std::string, std::shared_ptr<Provider>> GetProviders() const
        {
            return getProviders(Provider::PGINF_PROVIDER_TYPE);
        }

    protected:
        //!
        //! @brief Default constructor.
        //!  PluginManager cannot be publicly instantiated.
        //!
        Manage();

        //!
        //! @brief Register a provider type
        //!
        //! @param type Provider type.
        //! @param cur_version Current version of that provider type.
        //! @param lowest_version Lowest compatible version of that provider type.
        //!
        void registerType(const std::string& type, Provider::Version cur_version, Provider::Version lowest_version) const;

        //!
        //! @brief Get providers of a certain type.
        //!
        //! @param type Provider type.
        //! @return Pointer to the list of providers of that \a type,
        //!  or nullptr if \a type is not registered.
        //!
        const std::map<std::string, std::shared_ptr<Provider>> getProviders(const std::string& type) const;
    };

} // namespace plugin
} // namespace pginf

#endif // !_PGINF_PLUGIN_MANAGE_H_