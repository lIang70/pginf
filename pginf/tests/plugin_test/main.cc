#include <gtest/gtest.h>

#include <pginf/base/detail/system_info.h>
#include <pginf/plugin/interface.h>
#include <pginf/plugin/manage.h>
#include <pginf/plugin/provider.h>

class PluginManage : public pginf::plugin::Manage {
public:
    PluginManage() = default;
};

class PluginManagerTest : public ::testing::Test {
    using Provider = pginf::plugin::Provider;

    PluginManage* plugin_manage_ { nullptr };

protected:
    virtual void SetUp() override { plugin_manage_ = new PluginManage; }
    virtual void TearDown() override { delete plugin_manage_; }

public:
    void loadFromFolder()
    {
        auto root_path = pginf::util::systemdir();
    }

    inline std::map<std::string, std::shared_ptr<Provider>> getProvides()
    {
        return PGINF_GET_PROVI(plugin_manage_, pginf::Interface);
    }

    inline void unloadAll() { plugin_manage_->unloadAll(); }

    inline bool unload(std::string& id) { return plugin_manage_->unload(id); }
};

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}