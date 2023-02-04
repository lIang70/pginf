#ifndef _PGINF_PLUGIN_PROVIDER_H_
#define _PGINF_PLUGIN_PROVIDER_H_

#include <pginf/base/util.h>

#include <memory>
#include <string>

namespace pginf {
namespace plugin {

    class Host;

    class PGINF_API Provider : public std::enable_shared_from_this<Provider> {
    public:
        using Version = unsigned int;

        //!
        //! @brief Construct a new provider object
        //!
        Provider() = default;

        //!
        //! @brief Destructor for provider object
        //!
        virtual ~Provider() = default;

        //!
        //! @brief Get provider version.
        //!
        //! @return Version number.
        //!
        virtual const Version getInterVersion() const = 0;

        //!
        //! @brief Check compatibility with Host.
        //!
        //! The same provider may be compiled with different versions
        //! on Host side and on plugins side. This function checks if
        //! a plugin provider is compatible with the current version of
        //! the same provider type on the Host side.
        //!
        //! @param Host Host, proxy of Host application.
        //! @return True if it's compatible with \a Host.
        //!
        bool isCompatible(const Host* host) const;

        //!
        //! @brief Get object type.
        //!  Each provider defined on the Host application is identified by
        //!  a unique type. Those types are automatically managed internally by
        //!  pginf.
        //!
        //! @return Provider type id.
        //!
        virtual const std::string getProviderType() const = 0;
    };

} // namespace plugin
} // namespace pginf

#endif // !_PGINF_PLUGIN_PROVIDER_H_