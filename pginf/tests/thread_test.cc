#include "base/thread/local.h"
#include <pginf/base/thread.h>

#include <gtest/gtest.h>

#include <thread>
#include <unistd.h>

void mysleep(int seconds)
{
    timespec t = { seconds, 0 };
    nanosleep(&t, NULL);
}

void threadFunc()
{
    fprintf(stdout, "tid=%lu\n", pginf::current_thread::tid());
}

void threadFunc2(int x)
{
    fprintf(stdout, "tid=%lu, x=%d\n", pginf::current_thread::tid(), x);
}

void threadFunc3()
{
    fprintf(stdout, "tid=%lu\n", pginf::current_thread::tid());
    mysleep(1);
}

class Foo {
    double x_;

public:
    explicit Foo(double x)
        : x_(x)
    {
    }

    void memberFunc()
    {
        fprintf(stdout, "tid=%lu, Foo::x_=%f\n", pginf::current_thread::tid(), x_);
    }

    void memberFunc2(const std::string& text)
    {
        fprintf(stdout, "tid=%lu, Foo::x_=%f, text=%s\n", pginf::current_thread::tid(), x_, text.c_str());
    }
};

TEST(thread_tset, test_1)
{
    fprintf(stdout, "pid=%d, tid=%lu\n", ::getpid(), pginf::current_thread::tid());

    pginf::Thread t1(threadFunc);
    t1.start();
    fprintf(stdout, "t1.tid=%lu\n", t1.tid());
    t1.join();

    pginf::Thread t2(std::bind(threadFunc2, 42), "thread for free function with argument");
    t2.start();
    fprintf(stdout, "t2.tid=%lu\n", t2.tid());
    t2.join();

    Foo foo(87.53);
    pginf::Thread t3(std::bind(&Foo::memberFunc, &foo), "thread for member function without argument");
    t3.start();
    t3.join();

    pginf::Thread t4(std::bind(&Foo::memberFunc2, std::ref(foo), std::string("Hello Hare")));
    t4.start();
    t4.join();

    {
        pginf::Thread t5(threadFunc3);
        t5.start();
    }

    mysleep(2);
    {
        pginf::Thread t6(threadFunc3);
        t6.start();
        mysleep(2);
        // t6 destruct later than thread creation.
    }
    fprintf(stdout, "number of created threads %d\n", pginf::Thread::threadCreated());
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}