#ifndef _PGINF_PLUGIN_HOST_H_
#define _PGINF_PLUGIN_HOST_H_

#include <pginf/base/util.h>
#include <pginf/plugin/provider.h>

#include <memory>
#include <string>

namespace pginf {
namespace plugin {

    class HostPrivate;

    class PGINF_API Host : public std::enable_shared_from_this<Host> {
        friend class Provider;

        HostPrivate* p_ { nullptr };

    public:
        //!
        //! @brief Add provider.
        //!
        //! Provider type and version are checked. Only known and
        //! valid provider types are accepted.
        //!
        //! @param provider_identifier Provider's identifier.
        //! @param provider Provider to be added.
        //! @return True if the provider is accepted.
        //!
        bool add(std::string& provider_identifier, std::shared_ptr<Provider> provider);

        //!
        //! @brief Delete provider
        //!
        //! @param type  Provider's type
        //! @param provider_identifier Provider's identifier.
        //!
        void remove(std::string& type, std::string& provider_identifier);

        //!
        //! @brief Register a type of providers.
        //!
        //! @param type Provider type.
        //! @param version Current version of that provider type.
        //! @param lowest_version Lowest compatible version of that provider type.
        //!
        void registerType(const std::string& type,
            Provider::Version register_version, Provider::Version lowest_version);

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
        Provider::Version getLowestVersion(const std::string& type) const;

        //!
        //! @brief Get version of a type of providers.
        //!
        //! @param type Provider type.
        //! @return The version of the provider type.
        //!
        Provider::Version getVersion(const std::string& type) const;

    private:
        //!
        //! @brief Construct a new Host object.
        //! New Host instances are not publicly allowed.
        //!
        Host();

        //!
        //! @brief Destroy the Host object.
        //! Clears all hosted providers
        //!
        ~Host();
    };

} // namespace plugin
} // namespace pginf

#endif // !_PGINF_PLUGIN_HOST_H_