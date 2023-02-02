#ifndef _PGINF_BASE_COUNT_DOWN_LATCH_H_
#define _PGINF_BASE_COUNT_DOWN_LATCH_H_

#include "base/types.h"

namespace pginf {
namespace util {

    class CountDownLatch {
        mutable std::mutex mutex_ {};
        uint32_t count_ GUARDED_BY(mutex_) { 0 };
        std::condition_variable GUARDED_BY(mutex_) cv_ {};

    public:
        explicit CountDownLatch(uint32_t count)
            : count_(count)
        {
        }

        void countDown()
        {
            std::lock_guard<std::mutex> lock(mutex_);
            --count_;
            if (!count_)
                cv_.notify_all();
        }

        void await(int32_t milliseconds = 0)
        {
            std::unique_lock<std::mutex> lock(mutex_);
            while (count_ > 0) {
                if (milliseconds > 0) {
                    cv_.wait_for(lock, std::chrono::milliseconds(milliseconds));
                } else {
                    cv_.wait(lock);
                }
            }
        }

        uint32_t count()
        {
            std::lock_guard<std::mutex> lock(mutex_);
            return count_;
        }
    };

} // namespace util
} // namespace hare

#endif // !_PGINF_BASE_COUNT_DOWN_LATCH_H_