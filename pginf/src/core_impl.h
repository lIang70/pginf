#pragma once
#ifndef _PGINF_CORE_IMPL_H_
#define _PGINF_CORE_IMPL_H_

#include "read_write_lock.h"

#include <pginf/pipe.h>

PGINF_NAMESPACE_BEGIN()

class MsgHandle;

/****************************************
 * @brief 
 * 
 ****************************************/
class Core_Impl {
    using _Topic            = int;
    using _Pointer          = void*;
    using _Event            = std::shared_ptr<EventMeta>;
    using _Shared_Thread    = std::shared_ptr<std::thread>;
    using _Shared_Threads   = std::vector<_Shared_Thread>;
    using _Address          = std::array<unsigned char, 3 * sizeof(_Pointer)>;
    using _Subscribers      = std::map<_Topic, std::map<_Address, MsgHandle*>>;
    using _Topic_Lock       = std::map<_Topic, ReadWriteLock>;

private:
    static std::once_flag   s_oc_flag_;
    static Core_Impl *      s_core_impl_; // The singleton.

    // Publish-Subscribe
    _Topic_Lock     topic_locks_;
    _Subscribers    topic_subscribers_;

protected:
    /****************************************
     * @brief The static function that sends data.
     * 
     * @param handle Sent data operations.
     * @param event Data to be sent.
     ****************************************/
    static void SendData(_Topic topic, MsgHandle * handle, _Event & event, _Shared_Threads & join_threads, Pipe_Type activate_way);

    /****************************************
     * @brief Construct a new Core_Impl object
     * Core_Impl cannot be publicly instantiated.
     ****************************************/
    Core_Impl();

    /****************************************
     * @brief Destroy the Core_Impl object
     * Core_Impl cannot be publicly destroy.
     ****************************************/
    ~Core_Impl();

public:
    /****************************************
     * @brief Get a singleton pointer.
     * 
     * @return Singleton pointer.
     ****************************************/
    static Core_Impl* Get();

    /****************************************
     * @brief Initialize the core
     * 
     * @param core_impl 
     ****************************************/
    static void PluginCoreInit(Core_Impl* core_impl);

    /****************************************
     * @brief To send data.
     * 
     * @param topic The topic of data.
     * @param event Data.
     ****************************************/
    void Active(_Topic topic, _Event & event, Pipe_Type activate_way = DEFAULTCONNECT);

    /****************************************
     * @brief Subscription data.
     * 
     * @param topic Topic to subscribe.
     * @param address Address of The function
     * @param handle The function that receives data
     * @return If true, the subscription is successful
     ****************************************/
    bool Subscribe(_Topic topic, _Address & address, MsgHandle * handle);

    /****************************************
     * @brief Unsubscribe data.
     * 
     * @param topic Topic to unsubscribe.
     * @param address Address of The function
     ****************************************/
    void Unsubscribe(_Topic topic, _Address & address);

};

PGINF_NAMESPACE_END()

#endif // !_PGINF_CORE_IMPL_H_
