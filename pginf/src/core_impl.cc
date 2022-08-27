#include "core_impl.h"

PGINF_NAMESPACE_BEGIN()

std::once_flag  Core_Impl::s_oc_flag_;
Core_Impl *     Core_Impl::s_core_impl_ = nullptr;

void 
Core_Impl::SendData(_Topic topic, MsgHandle * handle, _Event & event, _Shared_Threads & join_threads, Pipe_Type activate_way) {
    Pipe_Type current_type = activate_way == DEFAULTCONNECT ? 
        (Pipe_Type)handle->get_pipe_type().ToInt() : activate_way;

    switch (current_type) {
    case Pipe_Type::DIRECTLYCONNECT:
        handle->Handle(topic, event);
        break;
    case Pipe_Type::DETACHCONNECT:
        std::make_shared<std::thread>([=] (_Event & event) { handle->Handle(topic, event); }, event)->detach();
        break;
    case Pipe_Type::JOINCONNECT:
        join_threads.push_back(std::make_shared<std::thread>([=] (_Event & event) { handle->Handle(topic, event); }, event));
        break;
    default:
        break;
    }
}

Core_Impl::Core_Impl() 
    : topic_locks_(_Topic_Lock()),
      topic_subscribers_(_Subscribers()) {
    
}

Core_Impl::~Core_Impl() {
    _Topic_Lock().swap(topic_locks_);
    _Subscribers().swap(topic_subscribers_);
}

Core_Impl *
Core_Impl::Get() {
    std::call_once(s_oc_flag_, [&] { s_core_impl_ = new Core_Impl(); });
    return s_core_impl_;
}

void 
Core_Impl::PluginCoreInit(Core_Impl* core_impl) {
    std::call_once(s_oc_flag_, [&] { s_core_impl_ = core_impl; });
}

void 
Core_Impl::Active(_Topic topic, _Event & event, Pipe_Type activate_way) {
    std::list<_Address> delete_subscribes{};
    _Shared_Threads     join_threads{};
    { // Send data
        UniqueLock<ReadWriteLock> topic_lock(topic_locks_[topic]);
        auto & subscribers = topic_subscribers_[topic];
        auto subscriber = subscribers.begin();
        for (; subscriber != subscribers.end(); ) {
            if (subscriber->second->IsNull()) {
                delete_subscribes.push_back(subscriber->first);
                subscriber++;
                continue;
            }
            SendData(topic, subscriber->second, event, join_threads, activate_way);
            subscriber++;
        }
    }

    // Delete subscribes
    if (!delete_subscribes.empty()) {    
        UniqueLock<ReadWriteLock> topic_lock(topic_locks_[topic], false);
        for (auto subscribe : delete_subscribes) {
            if (topic_subscribers_[topic].find(subscribe) != topic_subscribers_[topic].end()) {
                topic_subscribers_[topic].erase(subscribe);
            }
        }
    }
    
    // wait joinable thread
    for (_Shared_Thread join_thread : join_threads) {
        if (join_thread->joinable())
            join_thread->join();
    }
}

bool 
Core_Impl::Subscribe(_Topic topic, _Address & address, MsgHandle * handle) {
    UniqueLock<ReadWriteLock> topic_lock(topic_locks_[topic], false);
    if (topic_subscribers_[topic].find(address) == topic_subscribers_[topic].end()) {
        topic_subscribers_[topic].insert(std::make_pair(address, handle));
        return true;
    }
    return false;
}

void 
Core_Impl::Unsubscribe(_Topic topic, _Address & address) {
    UniqueLock<ReadWriteLock> topic_lock(topic_locks_[topic], false);
    if (topic_subscribers_[topic].find(address) != topic_subscribers_[topic].end()) {
        delete topic_subscribers_[topic][address];
        topic_subscribers_[topic].erase(address);
    }
}


PGINF_NAMESPACE_END()