#include <pginf/base/detail/log_async.h>
#include <pginf/base/detail/system_info.h>
#include <pginf/base/logging.h>

#include <gtest/gtest.h>

#include <chrono>
#include <dirent.h>
#include <regex>
#include <string>
#include <thread>

class AsyncTest : public ::testing::Test {
    static pginf::log::Async* async_;
    static int64_t total_size_;
    pginf::Timestamp total_start_ { 0 };

public:
    void bench(bool long_log = false)
    {
        int cnt = 0;
        const int N = 10000;
        std::string empty = " ";
        std::string long_str(3000, 'X');
        long_str += " ";

        total_start_ = pginf::Timestamp::now();
        for (int t = 0; t < 30; ++t) {
            pginf::Timestamp start = pginf::Timestamp::now();
            for (int i = 0; i < N; ++i) {
                LOG_INFO() << "Hello 0123456789"
                           << " abcdefghijklmnopqrstuvwxyz "
                           << (long_log ? long_str : empty)
                           << cnt;
                ++cnt;
            }
            pginf::Timestamp end = pginf::Timestamp::now();
            // printf("%f p/s.\n", N / pginf::Timestamp::difference(end, start));
        }
    }

protected:
    void SetUp() override
    {
        total_size_ = 0;
        pginf::Logger::setOutput(&AsyncTest::output);
        std::string name = pginf::util::systemdir() + "async_test";
        async_ = new pginf::log::Async(name, 16 * 1024 * 1024);
        async_->start();
    }

    void TearDown() override
    {
        delete async_;
        pginf::Timestamp total_end = pginf::Timestamp::now();
        auto seconds = pginf::Timestamp::difference(total_end, total_start_);
        printf("%f seconds, %.2f Mbytes, %.2f MiB/s\n",
            seconds, total_size_ / double(1024 * 1024), total_size_ / seconds / (1024 * 1024));

        auto dir = ::opendir(pginf::util::systemdir().c_str());
        EXPECT_NE(dir, nullptr);
        std::regex reg("(.*)(async_test)(.*)log");
        while (auto ptr = ::readdir(dir)) {
            if (!ptr)
                break;
            if (std::regex_match(ptr->d_name, reg)) {
                EXPECT_GE(::remove((pginf::util::systemdir() + ptr->d_name).c_str()), 0);
            }
        }
    }

private:
    static void output(const char* msg, int32_t len)
    {
        total_size_ += len;
        async_->append(msg, len);
    }
};

int64_t AsyncTest::total_size_ { 0 };
pginf::log::Async* AsyncTest::async_ { nullptr };

TEST_F(AsyncTest, test_log_asnc_short)
{
    bench();
}

TEST_F(AsyncTest, test_log_asnc_long)
{
    bench(true);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}