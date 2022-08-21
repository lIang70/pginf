#include <pginf/plugin_manager.h>
#include <pginf/plugin_interface.h>
#include <gtest/gtest.h>

#include <vector>

#define LOGGER(format, ...) \
    do { \
        fprintf(stdout, format##"\n", __VA_ARGS__); \
        fflush(stdout); \
    } while (0)

class PluginManage 
    : public pginf::PluginManager {

public:
    PluginManage() = default;
};

class PluginManagerTest: 
    public ::testing::Test {

    using Providers_Map = std::map<std::string, pginf::Provider *>;
    
    PluginManage * plugin_manage = nullptr;
protected:
    virtual void SetUp() override { plugin_manage = new PluginManage; }
    virtual void TearDown() override { delete plugin_manage; }

public:
    void            AcceptProviderType() { plugin_manage->AcceptProviderType<pginf::Interface_Provider>(); }
    int             LoadFromFolder()     { return plugin_manage->LoadFromFolder(plugin_manage->GetCurrentFolder() + "/plugin/"); }

    Providers_Map   GetProvides()        { return plugin_manage->GetProviders<pginf::Interface_Provider>(); }
    void            Unload()             { plugin_manage->UnloadAll(); }

    bool            Unload(std::string & identifier) { return plugin_manage->Unload(identifier); }
};

TEST_F(PluginManagerTest, LoadPluginFromFolder) {
    AcceptProviderType();
    int plugin_cnt = LoadFromFolder();
    LOGGER("[Info] Load plugin in \"[plugin]\" number: %d .", plugin_cnt);
    EXPECT_NE(plugin_cnt, 0);
    Unload();
}

TEST_F(PluginManagerTest, GetProvider) {
    AcceptProviderType();
    int plugin_cnt = LoadFromFolder();
    LOGGER("[Info] Load plugin in \"[plugin]\" number: %d .", plugin_cnt);
    EXPECT_NE(plugin_cnt, 0);

    auto providers = GetProvides();
    EXPECT_NE(providers.size(), 0);

    for (auto provider_iter : providers) {
        pginf::Interface_Provider * provider = static_cast<pginf::Interface_Provider *>(provider_iter.second);
        EXPECT_NE(provider, nullptr);
        if (!provider)
            continue;
        pginf::Interface * Interface = static_cast<pginf::Interface *>(provider->Create());
        EXPECT_NE(Interface, nullptr);
        if (Interface) {
            EXPECT_NE(Interface->get_name().size(), 0);
            EXPECT_NE(Interface->get_description().size(), 0);
            LOGGER("[Info] Load plugin: [%s]\n   description: %s .", Interface->get_name().c_str(), Interface->get_description().c_str());
            delete Interface;
        }
    }

    Unload();
}

TEST_F(PluginManagerTest, Pipe) {
    AcceptProviderType();
    int plugin_cnt = LoadFromFolder();
    LOGGER("[Info] Load plugin in \"[plugin]\" number: %d .", plugin_cnt);
    EXPECT_NE(plugin_cnt, 0);

    std::vector<std::string> ids;
    std::vector<pginf::Interface *> interfaces;
    auto providers = GetProvides();
    EXPECT_NE(providers.size(), 0);

    for (auto provider_iter : providers) {
        pginf::Interface_Provider * provider = static_cast<pginf::Interface_Provider *>(provider_iter.second);
        ids.emplace_back(provider_iter.first);
        EXPECT_NE(provider, nullptr);
        if (!provider)
            continue;
        pginf::Interface * Interface = static_cast<pginf::Interface *>(provider->Create());
        EXPECT_NE(Interface, nullptr);
        if (Interface) {
            interfaces.push_back(Interface);
        }
    }

    do {
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    } while(0);

    for (auto & interface : interfaces) {
        EXPECT_NE(interface->get_name().size(), 0);
        EXPECT_NE(interface->get_description().size(), 0);
        LOGGER("[Info] Load plugin: [%s]\n   description: %s .", interface->get_name().c_str(), interface->get_description().c_str());
        delete interface;
    }

    for (auto & id : ids) {
        EXPECT_NE(Unload(id), false);
    }
}

TEST_F(PluginManagerTest, Unload) {
    AcceptProviderType();
    int plugin_cnt = LoadFromFolder();
    LOGGER("[Info] Load plugin in \"[plugin]\" number: %d .", plugin_cnt);
    EXPECT_NE(plugin_cnt, 0);

    std::vector<std::string> ids;
    auto providers = GetProvides();
    EXPECT_NE(providers.size(), 0);

    for (auto provider_iter : providers) {
        pginf::Interface_Provider * provider = static_cast<pginf::Interface_Provider *>(provider_iter.second);
        ids.emplace_back(provider_iter.first);
        EXPECT_NE(provider, nullptr);
        if (!provider)
            continue;
        pginf::Interface * Interface = static_cast<pginf::Interface *>(provider->Create());
        EXPECT_NE(Interface, nullptr);
        if (Interface) {
            EXPECT_NE(Interface->get_name().size(), 0);
            EXPECT_NE(Interface->get_description().size(), 0);
            LOGGER("[Info] Load plugin: [%s]\n   description: %s .", Interface->get_name().c_str(), Interface->get_description().c_str());
            delete Interface;
        }
    }

    for (auto & id : ids) {
        EXPECT_NE(Unload(id), false);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}