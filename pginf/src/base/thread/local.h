#ifndef _PGINF_BASE_LOCAL_H_
#define _PGINF_BASE_LOCAL_H_

#include <pginf/base/thread.h>
#include "base/types.h"

namespace pginf {

struct ThreadLocal {
    Thread::Id tid_ { 0UL };
    std::string tid_string_ {};
    const char* thread_name_ { nullptr };
};

extern thread_local struct ThreadLocal t_data;

namespace current_thread {

    extern void cacheThreadId();

    extern inline Thread::Id tid()
    {
        if (__builtin_expect(t_data.tid_ == 0, 0)) {
            cacheThreadId();
        }
        return t_data.tid_;
    }

    inline const char* tidStr()
    {
        return t_data.tid_string_.c_str();
    }

    inline std::size_t tidStrSize()
    {
        return t_data.tid_string_.size();
    }

    inline const char* threadName()
    {
        return t_data.thread_name_;
    }


    extern bool isMainThread();
    extern std::string stackTrace(bool demangle);

}
}

#endif // !_PGINF_BASE_LOCAL_H_