#ifndef _PGINF_PLUGIN_PROVIDER_H_
#define _PGINF_PLUGIN_PROVIDER_H_

#include <pginf/base/util.h>

#include <memory>
#include <string>

namespace pginf {
namespace plugin {

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
        //! @brief Get object type.
        //!  Each provider defined on the Manage application is identified by
        //!  a unique type. Those types are automatically managed internally by
        //!  pginf.
        //!
        //! @return Provider type id.
        //!
        virtual const std::string getProviderType() const = 0;
    };

    namespace detail {
        struct NotFree {
            void operator()(Provider* x) {}
        };
    } // namespace detail

} // namespace plugin
} // namespace pginf

#endif // !_PGINF_PLUGIN_PROVIDER_H_