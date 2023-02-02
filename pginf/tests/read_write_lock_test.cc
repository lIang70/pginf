#include "../src/read_write_lock.h"

#include <gtest/gtest.h>

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

FILE * pginf::g_log_file = stdout;

static int              s_expect = 0;
static int              s_real = 0;
static bool             s_is_exit = false;
static long long        s_sum = 0;
static ReadWriteLock    s_read_write_lock;

void Reader () {
    while (true) {
        if (s_is_exit) {
            break;
        }
        pginf::UniqueLock<ReadWriteLock> unique_lock(s_read_write_lock);
        EXPECT_EQ(s_expect, s_real); 
    }
}

void Writer () {
    auto thread_id = std::this_thread::get_id();
    int cnt = 10;
    do {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        pginf::UniqueLock<ReadWriteLock> unique_lock(s_read_write_lock, false);
        s_expect++;
        s_real++;
    } while (cnt--);
    s_is_exit = true;
};

TEST(ReadWriteLockTest, MainTest) {
    auto thread_count = std::thread::hardware_concurrency();
    std::list<std::shared_ptr<std::thread>> thread_list;
    for (unsigned int i = 0; i < thread_count; i++) {
        thread_list.emplace_back(std::make_shared<std::thread>(Reader));
    }
    std::make_shared<std::thread>(Writer)->join();
    for (auto thread : thread_list) {
        if (thread->joinable())
            thread->join();
    }
}

void Calc(int start, int end) {
    for (int i = start; i <= end; i++) {
        pginf::UniqueLock<ReadWriteLock> unique_lock(s_read_write_lock, false);
        s_sum += i;
    }
}

TEST(ReadWriteLockTest, TestWriteLock) {
    auto thread_count = std::thread::hardware_concurrency();
    long long sum = 0;
    int all_size = (2048 * 16) * thread_count;
    int interval = all_size / thread_count;
    for (int i = 1; i <= all_size; i++) sum += i;
    for (unsigned int i = 0; i < thread_count; i++) {
        std::make_shared<std::thread>(Calc, interval * i + 1, interval * (i + 1))->join();
    }
    EXPECT_EQ(sum, s_sum);
}

int main(int argc, char **argv) {

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}