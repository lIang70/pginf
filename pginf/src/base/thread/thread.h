#ifndef _PGINF_BASE_THREAD_H_
#define _PGINF_BASE_THREAD_H_

#include "base/non_copyable.h"
#include "base/types.h"

#include <thread>

namespace pginf {

class ThreadData;
class Thread : public NonCopyable {
public:
    friend class ThreadData;

    using Task = std::function<void()>;
    using Id = size_t;

private:
    using UniqueThread = std::unique_ptr<std::thread>;

    UniqueThread thread_ { nullptr };
    Task task_ {};
    std::string name_ {};
    Id thread_id_ {};
    ThreadData* data_ { nullptr };
    bool started_ { false };

public:
    static int32_t threadCreated();

    explicit Thread(Task task, const std::string& name = std::string());
    ~Thread();

    void start();
    bool join();

    inline Id tid() const { return thread_id_; }
    inline const std::string& name() const { return name_; }
    inline bool started() const { return started_; }
};

}

#endif // !_PGINF_BASE_THREAD_H_