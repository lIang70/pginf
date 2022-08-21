#pragma once
#ifndef _PGINF_PIPE_H_
#define _PGINF_PIPE_H_

#include <pginf/pginf_global.h>
#include <pginf/pflags.h>
#include <pginf/msg_handle.h>

PGINF_NAMESPACE_BEGIN()

class Core_Impl;

/****************************************
 * @brief The pipeline for subscribing data.
 * 
 ****************************************/
class PGINF_API Pipe {
    using _Topic    = int;
    using _Address  = std::array<unsigned char, sizeof(void*)>;
    using _Event    = EventMeta;

private:
    // Delete constructors and destructors
    Pipe()  = delete;
    ~Pipe() = delete;

    /****************************************
     * @brief Transformation format
     * 
     ****************************************/
    template<typename T>
    static _Address ConvertAnyToFormat(const T element) {
        union Convert_t {
            T base_;
            _Address base_t_;
        } result;
        result.base_ = element;
        return result.base_t_;
    }

    /****************************************
     * @brief Subscription data.
     * 
     * @param topic Topic to subscribe.
     * @param address Address of The function
     * @param handle The function that receives data
     * @return If true, the subscription is successful
     ****************************************/
    static bool _Subscribe(_Topic topic, _Address& address, MsgHandle* handle);

    /****************************************
     * @brief Unsubscribe data.
     * 
     * @param topic Topic to unsubscribe.
     * @param address Address of The function
     ****************************************/
    static void _Unsubscribe(_Topic topic, _Address& address);

public:
    /****************************************
     * @brief Initialize the core of pipe.
     * 
     * @param core_impl The pointer of pipe's core.
     ****************************************/
    static void PulginInitImpl(Core_Impl * core_impl);

    /****************************************
     * @brief Send data to the topic
     * 
     * @param topic The topic to which data needs to be sent.
     * @param event Data to be sent.
     ****************************************/
    static void Send(_Topic topic, _Event * event);

    /****************************************
     * @brief Subscribe to data for a topic.
     * 
     * @param topic Topic to subscribe.
     * @param interface_object Object to subscribe.
     * @param function The function that receives data.
     * @param pipe_type The pipe type.
     * @return If true, the subscription is successful.
     ****************************************/
    template<class _Interface_Object, typename _Func>
    static bool Subscribe(_Topic topic, _Interface_Object* interface_object, _Func function, PipeType pipe_type = _Pipe_Type::DIRECTLYCONNECT) {
        return _Subscribe(  topic, 
                            ConvertAnyToFormat<_Func>(function), 
                            new MsgHandleM<_Interface_Object, _Func>(interface_object, function, pipe_type) );
    }

    /****************************************
     * @brief Unsubscribe.
     * 
     * @param topic Topic to unsubscribe.
     * @param interface_object Object to unsubscribe.
     * @param function The function that receives data.
     ****************************************/
    template<class _Interface_Object, typename _Func>
    static void Unsubscribe(_Topic topic, _Interface_Object* interface_object, _Func function) {
        _Unsubscribe(   topic, 
                        ConvertAnyToFormat<_Func>(function) );
    }
};

PGINF_NAMESPACE_END()

#endif // !_PGINF_PIPE_H_