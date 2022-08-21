#include "manager_impl.h"

#include "library.h"
#include "dir_tool.h"
#include "core_impl.h"

PGINF_NAMESPACE_BEGIN()

Manager_Impl::Manager_Impl() 
    : libraries_(_Library_Map()),
      manager_host_(_Host()) {

}

Manager_Impl::~Manager_Impl() {
    UnloadAll();
    if (g_log_file != stdout)
        fclose(g_log_file);
}

bool 
Manager_Impl::Load(const PluginXMLInfo& plugin_xml_info) {
    Library* lib = Library::Load(plugin_xml_info.absolute_path_);
    if (!lib) 
        return false;

    // check identifier
    if (lib->identifier_ != plugin_xml_info.identifier_) {
        LOGGER("[Error] Failed to load plugin \"%s\": The identifier does not match the XML!", lib->identifier_.c_str());
        delete lib;
        return false;
    }

    // try to get registration function.
    _Fn_Register_Plugin* register_fn = reinterpret_cast<_Fn_Register_Plugin*>(lib->GetSymbol("pginf_import"));

    if (!register_fn) {
        LOGGER("[Error] Failed to initialize plugin \"%s\": pginf_import function not found!", lib->identifier_.c_str());
        delete lib;
        return false;
    }

    std::string identifier = lib->identifier_;
    // try to initialize plugin:
    if (!register_fn(manager_host_, identifier)) {
        // plugin decided to fail
        LOGGER("[Error] Self registry failed on plugin \"%s\".", identifier.c_str());
        manager_host_.impl_->CancelAddictions();
        delete lib;
        return false;
    }

    _Fn_Init_Core* init_core_fn = reinterpret_cast<_Fn_Init_Core*>(lib->GetSymbol("core_init"));

    if (!init_core_fn) {
        LOGGER("[Error] Failed to initialize plugin \"%s\": core_init function not found!", lib->identifier_.c_str());
        manager_host_.impl_->CancelAddictions();
        delete lib;
        return false;
    }

    init_core_fn(Core_Impl::Get());

    // Store the library if addictions are confirmed
    if (libraries_.find(lib->identifier_) == libraries_.end()) {
        if (manager_host_.impl_->ConfirmAddictions()) {
            libraries_[lib->identifier_].xml_info_ = plugin_xml_info;
            libraries_[lib->identifier_].library_ = lib;
#ifdef PGINF_DEBUG
            LOGGER(" -- [Manager_Impl::load] Plug-in [%s] has been loaded.", lib->identifier_.c_str());
#endif // PGINF_DEBUG
        } else {
            // otherwise nothing was registered
            LOGGER("[Error] Nothing registered by plugin \"%s\".", lib->identifier_.c_str());
            delete lib;
            return false;
        }
    } else {
        LOGGER("[Error] Plugin \"%s\" has been loaded.", lib->identifier_.c_str());
        delete lib;
        return false;
    }
    return true;
}

bool 
Manager_Impl::Unload(const std::string& plugin_identifier) {
    _Library_Map::iterator library_map_iter = libraries_.find(plugin_identifier);
    if(library_map_iter != libraries_.end()) {
        LOGGER("[Info] Plug-in [%s] is uninstalled.", plugin_identifier.c_str());
        manager_host_.Delete(library_map_iter->second.xml_info_.type_, plugin_identifier);
        delete library_map_iter->second.library_;
        libraries_.erase(library_map_iter);
        return true;
    }
    LOGGER("[Error] Failed to uninstall the plug-in [%s].", plugin_identifier.c_str());
    return false;
}

void 
Manager_Impl::UnloadAll() {
    manager_host_.impl_->ClearProviders();
    _Library_Map::iterator library_map_iter;
    for (library_map_iter = libraries_.begin(); 
         library_map_iter != libraries_.end(); 
         ++library_map_iter) {
        LOGGER("[Info] Plug-in [%s] is uninstalled.", library_map_iter->first.c_str());
        manager_host_.Delete(library_map_iter->second.xml_info_.type_, library_map_iter->first);
        delete library_map_iter->second.library_;
    }
    libraries_.clear();
}

void 
Manager_Impl::GetLoadedPlugins(std::vector<std::string>& plugin_identifiers) const {
    plugin_identifiers.reserve(plugin_identifiers.size() + libraries_.size());
    _Library_Map::const_iterator library_map_iter;
    for (library_map_iter = libraries_.begin(); 
         library_map_iter != libraries_.end(); 
         ++library_map_iter) {
        plugin_identifiers.emplace_back(library_map_iter->first);
    }
}

bool 
Manager_Impl::IsLoaded(const std::string& plugin_identifier) const {
    return libraries_.find(plugin_identifier) != libraries_.end();
}

PGINF_NAMESPACE_END()