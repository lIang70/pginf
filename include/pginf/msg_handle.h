#pragma once
#ifndef _PGINF_MSG_HANDLE_H_
#define _PGINF_MSG_HANDLE_H_

#include <pginf/pginf_global.h>
#include <pginf/event_mate.h>
#include <pginf/pflags.h>

PGINF_NAMESPACE_BEGIN()

/****************************************
 * @brief The connection type of pipe.
 * 
 ****************************************/
typedef enum _Pipe_Type {
    DEFAULTCONNECT  = (0),
    DIRECTLYCONNECT = (1),
    DETACHCONNECT   = (2),
    JOINCONNECT     = (3),
} Pipe_Type;
P_DECLARE_FLAGS(PipeType, Pipe_Type)

/****************************************
 * @brief The base class of MsgHandle
 * 
 ****************************************/
class MsgHandle {    
    PipeType pipe_type_;

public:
    using _Topic = int;
    using _Event = std::shared_ptr<EventMeta>;

    MsgHandle(PipeType pipe_type = _Pipe_Type::DIRECTLYCONNECT) 
        : pipe_type_(pipe_type) {}
    virtual ~MsgHandle() = default;
    virtual void Handle(_Topic topic, _Event & e) = 0;
    virtual bool IsNull() = 0;
    inline  PipeType get_pipe_type() const { return pipe_type_; }
};

/****************************************
 * @brief The template of MsgHandle
 * 
 * @tparam _Class The class
 * @tparam _Func The function
 ****************************************/
template<class _Class, typename _Func>
class MsgHandleM 
    : public MsgHandle {
    static_assert(bool(std::is_same<_Func, void(_Class::*)(MsgHandle::_Topic, MsgHandle::_Event&)>::value), "Function's parameter should be (int, std::shared_ptr<EventMeta>&) .");
    
    using _Type  = PipeType;

private:
    _Class *  class_object_ = nullptr;
    _Func     function_;

public:
    /****************************************
     * @brief Construct a new Msg Handle object
     * 
     * @param class_object The class object
     * @param function The function
     * @param type The type of connection
     ****************************************/
    MsgHandleM(_Class* class_object, _Func function, _Type type = _Pipe_Type::DIRECTLYCONNECT) 
        : MsgHandle(type), 
          class_object_(class_object), function_(function) {}

    /****************************************
     * @brief Processing event Data
     * 
     * @param e Event data to process.
     ****************************************/
    inline virtual void Handle(_Topic topic, MsgHandle::_Event& e) override { (class_object_->*function_)(topic, e); }

    /****************************************
     * @brief Determines whether the object pointer is empty.
     * 
     ****************************************/
    inline virtual bool IsNull() override {
        // typeid is used to determine whether the pointer is freed.
        try {
            auto name = typeid(*class_object_).name();
            return false;
        } catch (std::bad_typeid) {
            return true;
        } catch (...) {
            return true;
        }
    }

};

PGINF_NAMESPACE_END()

#endif // !_PGINF_MSG_HANDLE_H_