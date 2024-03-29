#ifndef _PGINF_PLUGIN_INTERFACE_HPP_
#define _PGINF_PLUGIN_INTERFACE_HPP_

#include <pginf/base/util.h>
#include <pginf/plugin/manage.h>
#include <pginf/plugin/provider.h>

#include <memory>

// Macro: Create Provider.
#define PGINF_PROVIDER_CREATE(object)                          \
    class object##_Provider : public pginf::plugin::Provider { \
        friend class pginf::plugin::Manage;                    \
        friend class object;                                   \
        using Type = std::string;                              \
                                                               \
    public:                                                    \
        static const Version PGINF_INTER_VERSION;              \
        static const Version PGINF_INTER_LOWEST_VERSION;       \
        static const Type PGINF_PROVIDER_TYPE;                 \
                                                               \
    public:                                                    \
        ~object##_Provider() override = default;               \
        inline const Type getProviderType() const override     \
        {                                                      \
            return PGINF_PROVIDER_TYPE;                        \
        }                                                      \
        inline const Version getInterVersion() const override  \
        {                                                      \
            return PGINF_INTER_VERSION;                        \
        }                                                      \
        virtual object* create() const = 0;                    \
    };

// Marcro: Init Provider type, version and lowest_version
#define PGINF_PROVIDER_INIT(object_type, type, version, lower_version)                                         \
    const pginf::plugin::Provider::Version object_type##_Provider::PGINF_INTER_VERSION = version;              \
    const pginf::plugin::Provider::Version object_type##_Provider::PGINF_INTER_LOWEST_VERSION = lower_version; \
    const object_type##_Provider::Type object_type##_Provider::PGINF_PROVIDER_TYPE = POLITE_STR(type);

// Marcro: Help plugins generating their Provider implementations
#define PGINF_PROVIDER_INHERIT(specialized_object, base_object)           \
    class specialized_object##_Provider : public base_object##_Provider { \
    public:                                                               \
        base_object* create() const override                              \
        {                                                                 \
            return new specialized_object;                                \
        }                                                                 \
    };

// Macro: Create connect of plug-in
#define PGINF_CONNECTOR_CREATE(specialized_object)                                        \
    DLL_DECL bool importPlugin(pginf::plugin::Manage& manage, std::string plugin_id)      \
    {                                                                                     \
        return manage.importProvider(plugin_id,                                           \
            std::shared_ptr<pginf::plugin::Provider>(new specialized_object##_Provider(), \
                pginf::plugin::detail::NotFree()));                                       \
    };

// Macro: Define identifier of plug-in
#define PGINF_DEFINE_PLUGINID(specialized_object_id)           \
    DLL_DECL void getId(std::string& plugin_identifier)        \
    {                                                          \
        plugin_identifier = POLITE_STR(specialized_object_id); \
    }

namespace pginf {

//!
//! @brief STI of pginf.
//!
class Interface {

public:
    virtual ~Interface() = default;

    //!
    //! @brief Init the plug-in.
    //!
    virtual void load() = 0;

    //!
    //! @brief Init the plug-in.
    //!
    virtual void unload() = 0;

    //!
    //! @brief Get the description of interface.
    //!
    //! @return Description of interface.
    //!
    virtual std::string getDescription() const = 0;

    //!
    //! @brief Get the name of interface.
    //!
    //! @return Name of interface.
    //!
    virtual std::string getName() const = 0;
};

PGINF_PROVIDER_CREATE(Interface)

} // namespace pginf

#endif // !_PGINF_PLUGIN_INTERFACE_HPP_
