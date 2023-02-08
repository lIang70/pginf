#include <pginf/plugin/interface.h>

class Net : public pginf::Interface {
public:
    Net()
    {
    }
    
    ~Net() override
    {
    }

    void load() override
    {
    }

    void unload() override
    {
    }

    std::string getDescription() const override
    {
        return "Net plug-in: Network module.";
    }

    std::string getName() const override
    {
        return "Net";
    }
};

PGINF_PROVIDER_INHERIT(Net, pginf::Interface)
PGINF_CONNECTOR_CREATE(Net)
PGINF_DEFINE_PLUGINID(Net.20230205)