#pragma once
#ifndef _READ_WRITE_LOCK_H_
#define _READ_WRITE_LOCK_H_

#include "global.h"

/****************************************
 * @brief Implementation of read and write lock.
 * 
 ****************************************/
class ReadWriteLock {
    using _Counter      = volatile int;
    using _Condition    = std::condition_variable;
    using _Unique_Mutex = std::unique_lock<std::mutex>;

private:
    _Counter            reader_counter_         = 0;
    _Counter            reader_wait_counter_    = 0;
    _Counter            writer_counter_         = 0;
    _Counter            writer_wait_counter_    = 0;
    std::mutex          main_mutex_{};
    _Condition          condition_read_{};
    _Condition          condition_write_{};

public:
    ReadWriteLock() = default;
    ~ReadWriteLock() = default;

    // Delete the copy/move constructor/assignment function
    ReadWriteLock(const ReadWriteLock &)                = delete;
    ReadWriteLock(const ReadWriteLock &&)               = delete;
    ReadWriteLock & operator=(const ReadWriteLock &)    = delete;
    ReadWriteLock & operator=(const ReadWriteLock &&)   = delete;

    /****************************************
     * @brief Lock the read lock.
     * 
     ****************************************/
    void LockForRead() {
        _Unique_Mutex unique_lock(main_mutex_);
        reader_wait_counter_++;
        condition_read_.wait(unique_lock, [this] { return writer_wait_counter_ == 0 && writer_counter_ == 0 ; });
        PGINF_ASSERT(reader_wait_counter_ > 0);
        PGINF_ASSERT(writer_wait_counter_ == 0);
        PGINF_ASSERT(writer_counter_ == 0);
        reader_counter_++;
        reader_wait_counter_--;
    }

    /****************************************
     * @brief Lock the write lock.
     * 
     ****************************************/
    void LockForWrite() {
        _Unique_Mutex unique_lock(main_mutex_);
        writer_wait_counter_++;
        condition_write_.wait(unique_lock, [this] { 
            bool write = false; 
            return reader_counter_ == 0 && writer_counter_ == 0; 
        });
        PGINF_ASSERT(writer_wait_counter_ > 0);
        PGINF_ASSERT(writer_counter_ == 0);
        PGINF_ASSERT(reader_counter_ == 0);
        writer_counter_++;
        writer_wait_counter_--;
    }

    /****************************************
     * @brief Unlock.
     * 
     ****************************************/
    void Unlock() {
        _Unique_Mutex unique_lock(main_mutex_);
        if (writer_counter_ != 0) {
            PGINF_ASSERT(writer_counter_ == 1);
            PGINF_ASSERT(reader_counter_ == 0);
            writer_counter_ = 0;
        } else {
            PGINF_ASSERT(reader_counter_ > 0);
            reader_counter_--;
            if (reader_counter_ > 0)
                return;
        }

        if (writer_wait_counter_ != 0) {
            condition_write_.notify_one();
            return ;
        } else if (reader_wait_counter_ != 0) {
            condition_read_.notify_all();
            return ;
        }
    }
};

PGINF_NAMESPACE_BEGIN()

template <typename _Read_Write_Lock>
class UniqueLock {
    _Read_Write_Lock & read_write_lock_;
    
public:
    explicit UniqueLock(_Read_Write_Lock & read_write_lock, bool lock_for_read = true)
        : read_write_lock_(read_write_lock) {
        Lock(lock_for_read);
    }

    ~UniqueLock() {
        Unlock();
    }

    void Lock(bool lock_for_read = true) {
        if (lock_for_read) read_write_lock_.LockForRead();
        else read_write_lock_.LockForWrite();
    }

    void Unlock() {
        read_write_lock_.Unlock();
    }

    // Delete the copy/move constructor/assignment function
    UniqueLock(const UniqueLock &) = delete;
    UniqueLock(const UniqueLock &&) = delete;
    UniqueLock & operator=(const UniqueLock &) = delete;
    UniqueLock & operator=(const UniqueLock &&) = delete;
};

PGINF_NAMESPACE_END()

#endif // !_READ_WRITE_LOCK_H_