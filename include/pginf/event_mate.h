#pragma once
#ifndef _PGINF_EVENT_META_H_
#define _PGINF_EVENT_META_H_

#include <pginf/pginf_global.h>

PGINF_NAMESPACE_BEGIN()

/****************************************
 * @brief The base class of Event
 * 
 ****************************************/
class EventMeta {
    
public:
    EventMeta()          = default;
    virtual ~EventMeta() = default;
    virtual void Clear() {}        // To release memory
};

PGINF_NAMESPACE_END()

#endif // !_PGINF_EVENT_META_H_
