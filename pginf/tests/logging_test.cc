#include "base/log/file.h"
#include <pginf/base/detail/system_info.h>
#include <pginf/base/logging.h>
#include <pginf/base/thread_pool.h>

#include <gtest/gtest.h>

#include <dirent.h>
#include <memory>
#include <regex>
#include <stdio.h>

class LoggingTest : public ::testing::Test {
    static int64_t total_size_;
    static FILE* pure_file_;
    static std::unique_ptr<pginf::log::File> log_file_;
    char buffer_[64 * 1024];

public:
    void logInThread()
    {
        for (int32_t i = 0; i < 2; i++) {
            LOG_INFO() << "Test logging in thread.";
        }
        usleep(1000);
    }

    void bench(const char* file_name, bool long_log = false)
    {
        pure_file_ = fopen(file_name, "w");
        ::setbuffer(pure_file_, buffer_, sizeof(buffer_));

        pginf::Logger::setOutput(&LoggingTest::output);
        pginf::Timestamp start(pginf::Timestamp::now());
        total_size_ = 0;

        int n = 1000 * 1000;
        std::string empty = " ";
        std::string longStr(3000, 'X');
        longStr += " ";
        for (int i = 0; i < n; ++i) {
            LOG_INFO() << "Hello 0123456789"
                       << " abcdefghijklmnopqrstuvwxyz"
                       << (long_log ? longStr : empty)
                       << i;
        }
        pginf::Timestamp end(pginf::Timestamp::now());
        double seconds = pginf::Timestamp::difference(end, start);
        printf("%12s: %f seconds, %.2f Mbytes, %10.2f msg/s, %.2f MiB/s\n",
            file_name, seconds, total_size_ / double(1024 * 1024), n / seconds, total_size_ / seconds / (1024 * 1024));

        ::fclose(pure_file_);
        pure_file_ = nullptr;
        EXPECT_GE(::remove(file_name), 0);
        pginf::Logger::setOutput(nullptr);
    }

    void benchForLogFile(const char* file_name, bool long_log = false)
    {
        log_file_.reset(new pginf::log::File(pginf::util::systemdir() + file_name, 200 * 1000));
        pginf::Logger::setOutput(&LoggingTest::output);
        pginf::Logger::setFlush(&LoggingTest::flush);

        pginf::Timestamp start(pginf::Timestamp::now());
        total_size_ = 0;

        int n = 1000 * 1000;
        std::string empty = " ";
        std::string longStr(3000, 'X');
        longStr += " ";
        for (int i = 0; i < n; ++i) {
            LOG_INFO() << "Hello 0123456789"
                       << " abcdefghijklmnopqrstuvwxyz"
                       << (long_log ? longStr : empty)
                       << i;
        }
        pginf::Timestamp end(pginf::Timestamp::now());
        double seconds = pginf::Timestamp::difference(end, start);
        printf("%12s: %f seconds, %.2f Mbytes, %10.2f msg/s, %.2f MiB/s\n",
            (pginf::util::systemdir() + file_name).c_str(),
            seconds, total_size_ / double(1024 * 1024), n / seconds, total_size_ / seconds / (1024 * 1024));

        log_file_.reset();

        auto dir = ::opendir(pginf::util::systemdir().c_str());
        EXPECT_NE(dir, nullptr);
        std::regex reg("(.*)(" + std::string(file_name) + ")(.*)log");
        while (auto ptr = ::readdir(dir)) {
            if (!ptr)
                break;
            if (std::regex_match(ptr->d_name, reg)) {
                EXPECT_GE(::remove((pginf::util::systemdir() + ptr->d_name).c_str()), 0);
            }
        }
        pginf::Logger::setOutput(nullptr);
        pginf::Logger::setFlush(nullptr);
    }

private:
    static void output(const char* msg, int32_t len)
    {
        total_size_ += len;
        if (pure_file_) {
            fwrite(msg, 1, len, pure_file_);
        } else if (log_file_) {
            log_file_->append(msg, len);
        }
    }

    static void flush()
    {
        if (log_file_) {
            log_file_->flush();
        }
    }
};

int64_t LoggingTest::total_size_ { 0 };
FILE* LoggingTest::pure_file_ { nullptr };
std::unique_ptr<pginf::log::File> LoggingTest::log_file_ { nullptr };

TEST_F(LoggingTest, logging_test_in_thread_pool)
{
    pginf::ThreadPool pool("logging test");
    pool.start(5);
    pool.run(std::bind(&LoggingTest::logInThread, this));
    pool.run(std::bind(&LoggingTest::logInThread, this));
    pool.run(std::bind(&LoggingTest::logInThread, this));
    pool.run(std::bind(&LoggingTest::logInThread, this));
    pool.run(std::bind(&LoggingTest::logInThread, this));

    LOG_TRACE() << "trace";
    LOG_DEBUG() << "debug";
    LOG_INFO() << "Hello";
    LOG_WARNING() << "World";
    LOG_ERROR() << "Error";
    LOG_INFO() << sizeof(pginf::Logger);
    LOG_INFO() << sizeof(pginf::log::Stream);
    LOG_INFO() << sizeof(pginf::log::detail::Fmt);
    LOG_INFO() << sizeof(pginf::log::Stream::Buffer);

    pool.stop();
}

TEST_F(LoggingTest, logging_test_time_zone)
{
    pginf::TimeZone beijing(8 * 3600, "CST");
    pginf::Logger::setTimeZone(beijing);
    LOG_TRACE() << "trace CST";
    LOG_DEBUG() << "debug CST";
    LOG_INFO() << "Hello CST";
    LOG_WARNING() << "World CST";
    LOG_ERROR() << "Error CST";
}

TEST_F(LoggingTest, logging_bench_short_log_for_log_file)
{
    benchForLogFile("log_file_bench");
}

TEST_F(LoggingTest, logging_bench_long_log_for_log_file)
{
    benchForLogFile("log_file_bench", true);
}

TEST_F(LoggingTest, logging_bench_short_log)
{
    bench((pginf::util::systemdir() + "tmp.log").c_str());
}

TEST_F(LoggingTest, logging_bench_long_log)
{
    bench((pginf::util::systemdir() + "tmp.log").c_str(), true);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}