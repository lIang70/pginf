#ifndef _PGINF_PLUGIN_HOST_H_
#define _PGINF_PLUGIN_HOST_H_

#include <pginf/base/logging.h>
#include <pginf/base/util.h>
#include <pginf/plugin/provider.h>

#include <list>
#include <map>
#include <memory>
#include <string>

namespace pginf {
namespace core {

    class Host : public std::enable_shared_from_this<Host> {
        using Provider = plugin::Provider;
        using Version = Provider::Version;

        struct provide_manage {
            Version cur_version_ {};
            Version lowest_version_ {};
            std::map<std::string,
                std::shared_ptr<Provider>>
                providers_ {};

            provide_manage() = default;
            provide_manage(const provide_manage&) = delete;
            provide_manage(provide_manage&& t)
            {
                std::swap(cur_version_, t.cur_version_);
                std::swap(lowest_version_, t.lowest_version_);
                providers_.swap(t.providers_);
            }
        };

        std::map<std::string, provide_manage> known_type_providers_ {};
        std::map<std::string,
            std::list<std::pair<std::string,
                std::shared_ptr<Provider>>>>
            request_for_addition_ {};

    public:
        //!
        //! @brief Construct a new Host object.
        //! New Host instances are not publicly allowed.
        //!
        Host() = default;

        //!
        //! @brief Destroy the Host object.
        //! Clears all hosted providers
        //!
        ~Host()
        {
            clearProviders();
            cancelAddictions();
        }

        //!
        //! @brief Add provider.
        //!
        //! Provider type and version are checked. Only known and
        //! valid provider types are accepted.
        //!
        //! @param provider_id Provider's identifier.
        //! @param provider Provider to be added.
        //! @return True if the provider is accepted.
        //!
        bool add(std::string& provider_id, std::shared_ptr<Provider> provider);

        //!
        //! @brief Delete provider
        //!
        //! @param type  Provider's type
        //! @param provider_id Provider's identifier.
        //!
        void remove(std::string& type, const std::string& provider_id);

        //!
        //! @brief Register a type of providers.
        //!
        //! @param type Provider type.
        //! @param version Current version of that provider type.
        //! @param lowest_version Lowest compatible version of that provider type.
        //!
        void registerType(const std::string& type,
            Version register_version, Version lowest_version);

        //!
        //! @brief Previous add calls are confirmed.
        //!  Added providers are finally stored.
        //!
        //! @return True if something was stored.
        //! @see add(std::string provider_id, std::shared_ptr<Provider>& provider);
        //!
        bool confirmAddictions();

        //!
        //! @brief Clears all hosted providers.
        //!
        inline void clearProviders()
        {
            for (auto providers_map_iter = known_type_providers_.begin();
                 providers_map_iter != known_type_providers_.end();
                 ++providers_map_iter) {
                providers_map_iter->second.providers_.clear();
            }
        }

        //!
        //! @brief Previous add calls are canceled.
        //! Added providers are not stored.
        //!
        //! @see add(std::string provider_id, std::shared_ptr<Provider>& provider);
        //!
        inline void cancelAddictions()
        {
            request_for_addition_.clear();
        }

        //!
        //! @brief Get providers of a certain type.
        //!
        //! @param type Provider type.
        //! @return Pointer to the list of providers of that \a type,
        //! or nullptr if \a type is not registered.
        //!
        inline const std::map<std::string,
            std::shared_ptr<Provider>>
        getProviders(const std::string& type) const
        {
            auto iter = known_type_providers_.find(type);
            if (iter != known_type_providers_.end())
                return iter->second.providers_;

            return std::map<std::string,
                std::shared_ptr<Provider>>();
        }

    protected:
        //!
        //! @brief Check provider type and version.
        //!
        //! @param provider
        //! @return True if the provider is acceptable.
        //!
        bool checkProvider(std::shared_ptr<Provider>& provider);

        //!
        //! @brief Ckeck if a provider type is registered.
        //!
        //! @param type Provider type id.
        //! @return True if the type is registered
        //!
        bool checkType(const std::string& type) const;

        //!
        //! @brief Get lowest compatible version of a type of providers.
        //!
        //! @param type Provider type.
        //! @return The lowest compatible version of the provider type.
        //!
        Version getLowestVersion(const std::string& type) const;

        //!
        //! @brief Get version of a type of providers.
        //!
        //! @param type Provider type.
        //! @return The version of the provider type.
        //!
        Version getVersion(const std::string& type) const;

        //!
        //! @brief Check Provider compatibility.
        //!
        //! The same provider may be compiled with different versions
        //! on host side and on plugins side. This function checks if
        //! a plugin provider is compatible with the current version of
        //! the same provider type on the host side.
        //!
        //! @param provider Provider.
        //! @return True if provider is compatible with host.
        //!
        bool isCompatible(std::shared_ptr<Provider>& provider) const;
    };

} // namespace core
} // namespace pginf

#endif // !_PGINF_PLUGIN_HOST_H_