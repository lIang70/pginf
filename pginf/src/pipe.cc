#include <pginf/pipe.h>
#include <pginf/event_mate.h>

#include "internal_global.h"
#include "core_impl.h"

PGINF_NAMESPACE_BEGIN()

bool 
Pipe::_Subscribe(_Topic topic, _Address& address, MsgHandle* handle) {;
    // Subscription Data
    return Core_Impl::Get()->Subscribe(topic, address, handle);
}

void 
Pipe::_Unsubscribe(_Topic topic, _Address& address) {;
    // Subscription Data
    Core_Impl::Get()->Unsubscribe(topic, address);
}

void 
Pipe::PulginInitImpl(Core_Impl * core_impl) {
    if (!core_impl)
        return;
    Core_Impl::PluginCoreInit(core_impl);
}

void 
Pipe::Send(_Topic topic, _Event * event, Pipe_Type pipe_type) {
    // To send data
    Core_Impl::Get()->Active(topic, std::shared_ptr<_Event>(event), pipe_type);
}

PGINF_NAMESPACE_END()