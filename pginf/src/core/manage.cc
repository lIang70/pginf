#include "core/manage_p.h"
#include "core/util.h"

#include <pginf/base/logging.h>
#include <pginf/plugin/interface.h>

namespace pginf {

// Init STI infomation.
PGINF_PROVIDER_INIT(Interface, STI, 2, 1)

namespace plugin {

    Manage::~Manage()
    {
        p_->main_host_.cancelAddictions();
        unloadAll();
        delete p_;
    }

    bool Manage::loadPlugin(const std::string& path)
    {
        auto manage = shared_from_this();
        auto lib = core::loadPlugin(manage, path);
        if (!lib)
            return false;

        // Store the library if addictions are confirmed
        auto plugin_id = lib->id();
        if (p_->libraries_info_.find(plugin_id) != p_->libraries_info_.end()) {
            if (p_->main_host_.confirmAddictions()) {
                p_->libraries_info_[plugin_id].library_ = std::move(lib);
                p_->plugin_cnt_++;
                LOG_DEBUG() << "Plug-in[" << plugin_id << "] has been loaded.";
            } else {
                // Otherwise nothing was registered
                return false;
            }
        } else {
            p_->main_host_.cancelAddictions();
            LOG_ERROR() << "An error occurred while loading the plug-in[" << plugin_id << "].";
            return false;
        }
        return true;
    }

    bool Manage::unload(const std::string& plugin_id)
    {
        auto iter = p_->libraries_info_.find(plugin_id);
        if (iter != p_->libraries_info_.end()) {
            p_->main_host_.remove(p_->libraries_info_[plugin_id].type_, plugin_id);
            p_->libraries_info_.erase(plugin_id);
            p_->plugin_cnt_--;
            LOG_INFO() << "Plug-in[" << plugin_id << "] is uninstalled.";
            return true;
        }
        LOG_WARNING() << "Failed to uninstall the plug-in[" << plugin_id << "].";
        return false;
    }

    void Manage::unloadAll()
    {
        p_->main_host_.clearProviders();
        p_->libraries_info_.clear();
        p_->plugin_cnt_ = 0;
    }

    void Manage::getLoadedPlugins(std::vector<std::string>& plugin_ids) const
    {
        int32_t index = 0;
        plugin_ids.clear();
        plugin_ids.reserve(p_->plugin_cnt_);
        for (auto& p : p_->libraries_info_) {
            plugin_ids[index] = p.second.id_;
        }
    }

    bool Manage::isLoaded(const std::string& plugin_id) const
    {
        return p_->libraries_info_.find(plugin_id) != p_->libraries_info_.end();
    }

    bool Manage::importProvider(std::string& plugin_id, std::shared_ptr<Provider> provider)
    {
        if (p_->libraries_info_.find(plugin_id) != p_->libraries_info_.end()) {
            LOG_ERROR() << "Plug-in[" << plugin_id << "] already loaded.";
            return false;
        }
        auto ret = p_->main_host_.add(plugin_id, provider);
        if (!ret) {
            return ret;
        }
        ManagePrivate::library_info info;
        info.id_ = plugin_id;
        info.type_ = provider->getProviderType();
        p_->libraries_info_.insert(std::make_pair(plugin_id, std::move(info)));
        return ret;
    }

    Manage::Manage()
        : p_(new ManagePrivate)
    {
        PGINF_REG_PTYPE(this, Interface);
    }

    void Manage::registerType(const std::string& type, Provider::Version cur_version, Provider::Version lowest_version) const
    {
        p_->main_host_.registerType(type, cur_version, lowest_version);
    }

    Manage::Weak_Providers Manage::getProviders(const std::string& type) const
    {
        return p_->main_host_.getProviders(type);
    }

} // namespace plugin
} // namespace pginf