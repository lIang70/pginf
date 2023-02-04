#include <pginf/plugin/interface.h>

class Decode : public pginf::Interface {
public:
    Decode()
    {
    }
    ~Decode() override
    {
    }

    void init() override
    {
    }

    std::string getDescription() const override
    {
        return "Decode plug-in: Video decoding module.";
    }

    std::string getName() const override
    {
        return "Decode";
    }
};

PGINF_PROVIDER_INHERIT(Decode, pginf::Interface)
PGINF_CONNECTOR_CREATE(Decode)
PGINF_DEFINE_PLUGINID(Decode.20230205)
