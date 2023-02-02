#include "base/thread/local.h"
#include <pginf/base/exception.h>

#include <gtest/gtest.h>

#include <functional>
#include <string>
#include <vector>

class Foo {
public:
    void test(std::vector<std::string> names = {})
    {
        printf("Stack:\n%s\n", pginf::current_thread::stackTrace(true).c_str());

        [] {
            printf("Stack inside lambda:\n%s\n", pginf::current_thread::stackTrace(true).c_str());
        }();

        std::function<void()> func([] {
            printf("Stack inside std::function:\n%s\n", pginf::current_thread::stackTrace(true).c_str());
        });
        func();

        func = std::bind(&Foo::callback, this);
        func();

        throw pginf::Exception("oops");
    }

private:
    void callback()
    {
        printf("Stack inside std::bind:\n%s\n", pginf::current_thread::stackTrace(true).c_str());
    }
};

TEST(execption_test, test_1)
{
    try {
        Foo b;
        b.test();
    } catch (const pginf::Exception& e) {
        printf("reason: %s\n", e.what());
        printf("stack trace:\n%s\n", e.stackTrace());
    }
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}