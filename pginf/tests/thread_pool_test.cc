#include "base/thread/local.h"
#include "base/thread/thread_pool.h"
#include <pginf/base/logging.h>

#include <gtest/gtest.h>

#include <atomic>
#include <functional>
#include <thread>
#include <unistd.h>

void print()
{
    printf("tid=%lu\n", pginf::current_thread::tid());
}

void printString(const std::string& str)
{
    LOG_INFO() << str;
    usleep(100 * 1000);
}

void test(int maxSize)
{
    LOG_WARNING() << "Test ThreadPool with max queue size = " << maxSize;
    pginf::ThreadPool pool("MainThreadPool");
    pool.setMaxQueueSize(maxSize);
    pool.start(5);

    LOG_WARNING() << "Adding";
    pool.run(print);
    pool.run(print);
    for (int i = 0; i < 100; ++i) {
        char buf[32];
        snprintf(buf, sizeof(buf), "task %d", i);
        pool.run(std::bind(printString, std::string(buf)));
    }
    LOG_WARNING() << "Done";

    std::atomic_flag flag {};
    flag.test_and_set();
    pool.run([&flag]() {
        flag.clear();
    });
    while (flag.test_and_set())
        ;
    pool.stop();
}

void longTask(int num)
{
    LOG_INFO() << "longTask " << num;
    std::this_thread::sleep_for(std::chrono::milliseconds(20000));
}

void test2()
{
    LOG_WARNING() << "Test ThreadPool by stoping early.";
    pginf::ThreadPool pool("ThreadPool");
    pool.setMaxQueueSize(5);
    pool.start(3);

    pginf::Thread thread1(
        [&pool]() {
            for (int i = 0; i < 20; ++i) {
                pool.run(std::bind(longTask, i));
            }
        },
        "thread1");
    thread1.start();

    std::this_thread::sleep_for(std::chrono::milliseconds(20000));
    LOG_WARNING() << "stop pool";
    pool.stop(); // early stop

    thread1.join();
    // run() after stop()
    pool.run(print);
    LOG_WARNING() << "test2 Done";
}

TEST(thread_pool_test, test_1)
{
    test(0);
    test(1);
    test(5);
    test(10);
    test(50);
}

TEST(thread_pool_test, test_2)
{
    test2();
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
