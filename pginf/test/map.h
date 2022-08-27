#pragma once
#ifndef _PLUGIN_MAP_H_
#define _PLUGIN_MAP_H_

#include <pginf/plugin_interface.h>
#include <pginf/event_mate.h>

#include <memory>
#include <thread>

class Map
    : public pginf::Interface {
    using _Topic    = int;
    using _Event    = std::shared_ptr<pginf::EventMeta>;
    using _Work     = std::shared_ptr<std::thread>;

    bool    thread_exit_ = false;
    _Work   event_work_{};

    void Init();
    void RecvNetworkEvent(_Topic topic, _Event& event);
public:
    Map();
    ~Map() override;
    std::string get_description() const override;
    std::string get_name() const override;
};

PGINF_PROVIDER_INHERIT(Map, pginf::Interface)
PGINF_CONNECTOR_CREATE(Map)
PGINF_DEFINE_PLUGINID(map.20220815)
PGINF_CORE_INITIALIZE()

#endif // !_PLUGIN_MAP_H_