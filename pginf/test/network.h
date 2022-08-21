#pragma once
#ifndef _PLUGIN_NETWORK_H_
#define _PLUGIN_NETWORK_H_

#include <pginf/plugin_interface.h>
#include <pginf/event_mate.h>

#include <memory>
#include <thread>

class Network
    : public pginf::Interface {
    using _Event    = std::shared_ptr<pginf::EventMeta>;
    using _Work     = std::shared_ptr<std::thread>;

    bool    thread_exit_ = false;
    _Work   event_work_{};

    void Init();
    void RecvMapEvent(_Event& event);
public:
    Network();
    ~Network() override;
    std::string get_description() const override;
    std::string get_name() const override;
};

PGINF_PROVIDER_INHERIT(Network, pginf::Interface)
PGINF_CONNECTOR_CREATE(Network)
PGINF_DEFINE_PLUGINID(network.20220821)
PGINF_CORE_INITIALIZE()

#endif // !_PLUGIN_NETWORK_H_