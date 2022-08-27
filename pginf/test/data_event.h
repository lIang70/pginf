#pragma once
#ifndef _DATA_EVENT_H_
#define _DATA_EVENT_H_

#include <pginf/event_mate.h>

typedef enum _Pipe_Event_Type {
    PIPE_NETWORK_EVENT  = 1,
    PIPE_MAP_EVENT      = 2
} PipeEventType;

class MapEvent
    : public pginf::EventMeta {
    
public:
    const char* greeting_ = "Hello PginF! This is a map event.";

    MapEvent() = default;
    ~MapEvent() override {
        
    };
    
    // To release memory
    void Clear() override {

    };
};

class NetworkEvent
    : public pginf::EventMeta {
    
public:
    const char* greeting_ = "Hello PginF! This is a network event.";

    NetworkEvent() = default;
    ~NetworkEvent() override {

    };
    
    // To release memory
    void Clear() override {

    };
};

#endif // !_DATA_EVENT_H_