#ifndef _PGINF_PLUGIN_HOST_P_H_
#define _PGINF_PLUGIN_HOST_P_H_

#include <pginf/base/logging.h>
#include <pginf/plugin/host.h>
#include <pginf/plugin/provider.h>

#include <list>
#include <map>
#include <memory>
#include <string>

namespace pginf {

class HostPrivate {
public:
    struct _provide_info {
        Provider::Version cur_version_ {};
        Provider::Version lowest_version_ {};
        std::map<std::string,
            std::shared_ptr<Provider>>
            providers_ {};
    };

    std::map<std::string, _provide_info> known_type_providers_ {};
    std::map<std::string,
        std::list<std::pair<std::string,
            std::shared_ptr<Provider>>>>
        request_for_addition_ {};

    HostPrivate() = default;

    ~HostPrivate()
    {
        clearProviders();
        cancelAddictions();
    }

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
    //! @see add(std::string provider_identifier, std::shared_ptr<Provider>& provider);
    //!
    inline void cancelAddictions()
    {
        request_for_addition_.clear();
    }

    //!
    //! @brief Previous add calls are confirmed.
    //!  Added providers are finally stored.
    //!
    //! @return True if something was stored.
    //! @see add(std::string provider_identifier, std::shared_ptr<Provider>& provider);
    //!
    bool confirmAddictions()
    {
        if (request_for_addition_.empty()) {
            LOG_WARNING() << "No provider needs to be added";
            return false;
        }

        for (auto& ppair : request_for_addition_) {
            auto list = ppair.second;
            for (auto& provide : list) {
                known_type_providers_[ppair.first].providers_.insert(provide);
            }
        }

        // clear map
        cancelAddictions();
        return true;
    }

    //!
    //! @brief Get providers of a certain type.
    //!
    //! @param type Provider type.
    //! @return Pointer to the list of providers of that \a type,
    //! or nullptr if \a type is not registered.
    //!
    const std::map<std::string,
        std::shared_ptr<Provider>>
    getProviders(const std::string& type) const
    {
        auto iter = known_type_providers_.find(type);
        if (iter != known_type_providers_.end())
            return iter->second.providers_;

        return std::map<std::string,
            std::shared_ptr<Provider>>();
    }
};

} // namespace pginf

#endif // !_PGINF_PLUGIN_HOST_P_H_