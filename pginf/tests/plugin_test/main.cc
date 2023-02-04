#include <gtest/gtest.h>

#include <pginf/base/detail/system_info.h>
#include <pginf/base/logging.h>
#include <pginf/base/util.h>
#include <pginf/plugin/interface.h>
#include <pginf/plugin/manage.h>
#include <pginf/plugin/provider.h>

class PluginManage : public pginf::plugin::Manage {
public:
    PluginManage() = default;
};

class PluginTest : public ::testing::Test {
    using Provider = pginf::plugin::Provider;

    std::shared_ptr<PluginManage> plugin_manage_ { nullptr };

protected:
    virtual void SetUp() override { plugin_manage_.reset(new PluginManage); }
    virtual void TearDown() override { }

public:
    void loadFromFolder()
    {
        auto root_path = pginf::util::systemdir();
        auto plugin_path = root_path + "/plugin/";
        std::list<std::string> plugins;
        pginf::file::listFiles(plugins, plugin_path, PGINF_EXTENSION, true);
        for (auto& dll : plugins) {
            if (plugin_manage_->loadPlugin(dll)) {
                LOG_INFO() << "Load plug-in from " << dll;
            }
        }
    }

    inline std::map<std::string, std::shared_ptr<Provider>> getProvides()
    {
        return PGINF_GET_PROVI(plugin_manage_, pginf::Interface);
    }

    inline void unloadAll() { plugin_manage_->unloadAll(); }

    inline bool unload(std::string& id) { return plugin_manage_->unload(id); }
};

TEST_F(PluginTest, LoadPluginFromFolder)
{
    loadFromFolder();

    auto providers = getProvides();
    for (auto& p : providers) {
        LOG_INFO() << "Provider[" << p.first
                   << "]\n  type: " << p.second->getProviderType()
                   << "\n  version: " << p.second->getInterVersion();
        if (p.second->getProviderType() == "STI") {
            auto provider = static_cast<pginf::Interface_Provider*>(p.second.get());
            auto object = provider->create();
            LOG_INFO() << "Plugin[" << object->getName()
                       << "]\n  Description: " << object->getDescription();
            delete object;
        }
    }
    providers.clear();

    std::string id = "Decode.20230205";
    EXPECT_EQ(unload(id), true);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    pginf::TimeZone tz(8 * 3600, "UTC");
    pginf::Logger::setTimeZone(tz);
    pginf::Logger::setLogLevel(pginf::log::LogLevel::TRACE);

    return RUN_ALL_TESTS();
}