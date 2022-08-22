#include <pginf/plugin_manager.h>

#include "manager_impl.h"
#include "dir_tool.h"
#include "internal_global.h"

// Import tinyxml
#include <tinyxml.h>

PGINF_NAMESPACE_BEGIN()

FILE * g_log_file = stdout;  // Pointer to the log

PluginManager::PluginManager() 
    : impl_(new Manager_Impl) {
}

void 
PluginManager::RegisterType(const std::string& type, _Version cur_version, _Version lowest_version) const {
    impl_->RegisterType(type, cur_version, lowest_version);
}

const PluginManager::_Provider_Map 
PluginManager::GetProviders(const std::string& type) const {
    return impl_->GetProviders(type);
}

std::string
PluginManager::GetCurrentFolder() {
    return dir_tool::CurrentFolder();
}

PluginManager::~PluginManager() {
    delete impl_;
}

bool 
PluginManager::Load(const std::string& path_of_xml) {
    std::string path_fixed = dir_tool::TurnRightSlashes2LeftInPath(path_of_xml);
    TiXmlDocument plugin_xml_document(path_fixed);
    if (!plugin_xml_document.LoadFile()) {
        LOGGER("[Error] Failure to read the XML of plugin.");
        return false;
    }

    TiXmlElement* root_node = plugin_xml_document.RootElement();
    PluginXMLInfo plugin_xml_info;
    if (root_node->ValueStr() != "plugin") {
        LOGGER("[Error] Failure to parse the XML of plugin: \n   The root element isn't \'plugin\' .");
        return false;
    }

    plugin_xml_info.xml_path_       = path_fixed;
    plugin_xml_info.type_           = root_node->Attribute("type");
    plugin_xml_info.identifier_     = root_node->Attribute("id");
    plugin_xml_info.folder_         = dir_tool::GetFolderPathFromFile(path_fixed, "xml");

    TiXmlElement* sub_node = root_node->FirstChildElement();
    while (sub_node ) {
        if (sub_node->ValueStr() == "name") {
            plugin_xml_info.name_ = sub_node->GetText();
        } else if (sub_node->ValueStr() == "version") {
            plugin_xml_info.version_ = sub_node->GetText();
        } else if (sub_node->ValueStr() == "uninstall_need_reload") {
            if (sub_node->GetText() == "true")
                plugin_xml_info.plugin_flag_.SetFlag(UNINSTALL_NEED_RELOAD);
        } else if (sub_node->ValueStr() == "need_load") {
            if (sub_node->GetText() == "true")
                plugin_xml_info.plugin_flag_.SetFlag(PLUGIN_NEED_LOAD);
        }
        sub_node = sub_node->NextSiblingElement();
    }

    plugin_xml_info.absolute_path_  = plugin_xml_info.folder_ + "/" + plugin_xml_info.name_ + "/" + plugin_xml_info.name_ + "." + PGINF_EXTENSION;

    return impl_->Load(plugin_xml_info);
}

int  
PluginManager::LoadFromFolder(const std::string& folder, bool recursive) {
    std::list<std::string> files;
    dir_tool::ListFiles(files, dir_tool::TurnRightSlashes2LeftInPath(folder), "xml", recursive);
    // Try to load every library
    int res = 0;
    std::list<std::string>::const_iterator it;
    for (it = files.begin(); it != files.end(); ++it) {
        if (Load(*it)) 
            ++res;
    }
    return res;
}

bool 
PluginManager::Unload(const std::string& plugin_identifier) {
    return impl_->Unload(plugin_identifier);
}

void 
PluginManager::UnloadAll() {
    impl_->UnloadAll();
}

void 
PluginManager::GetLoadedPlugins(std::vector<std::string>& plugin_identifiers) const {
    impl_->GetLoadedPlugins(plugin_identifiers);
}

bool 
PluginManager::IsLoaded(const std::string& plugin_identifier) const {
    return impl_->IsLoaded(plugin_identifier);
}

bool 
PluginManager::SetLogFilePath(const std::string& logger_path) const {
#ifdef _MSC_VER
    errno_t errorno = fopen_s(&g_log_file, logger_path.c_str(), "a");
    if (!g_log_file) { 
        g_log_file = stdout;
        char error_str[80];
        strerror_s(error_str, 80, errorno);
        LOGGER("[Error] Failed to set the log path.\n   Detail: %s ", error_str);
        return false;
    } else {
        return true;
    }
#else
    g_log_file = fopen(logger_path.c_str(), "a");
    if (!g_log_file) { 
        g_log_file = stdout;
        LOGGER("[Error] Failed to set the log path.\n   Detail: %s ", strerror(_get_errno()));
        return false;
    } else {
        return true;
    }
#endif
}


PGINF_NAMESPACE_END()