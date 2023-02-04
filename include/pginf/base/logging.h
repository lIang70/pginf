#ifndef _PGINF_BASE_LOGGING_H_
#define _PGINF_BASE_LOGGING_H_

#include <pginf/base/detail/log_stream.h>
#include <pginf/base/time_zone.h>
#include <pginf/base/timestamp.h>
#include <pginf/base/util.h>

#include <functional>

namespace pginf {

namespace log {

    enum class LogLevel : int32_t {
        TRACE,
        DEBUG,
        INFO,
        WARNING,
        Error,
        FATAL,
        NUM_LOG_LEVELS
    };

    extern PGINF_API const char* strErrorno(int errorno);

}

class PGINF_API Logger {
public:
    using Output = std::function<void(const char*, int)>;
    using Flush = std::function<void()>;

    struct FilePath {
        const char* data_ { nullptr };
        int32_t size_ { 0 };

        template <int Length>
        FilePath(const char (&arr)[Length])
            : data_(arr)
            , size_(Length - 1)
        {
            const char* slash = strrchr(data_, '/'); // builtin function
            if (slash) {
                data_ = slash + 1;
                size_ -= static_cast<int32_t>(data_ - arr);
            }
        }

        explicit FilePath(const char* file_name)
            : data_(file_name)
        {
            const char* slash = strrchr(file_name, '/');
            if (slash) {
                data_ = slash + 1;
            }
            size_ = static_cast<int32_t>(strlen(data_));
        }
    };

private:
    struct Data {
        Timestamp time_ {};
        log::Stream stream_ {};
        log::LogLevel level_ {};
        int line_ {};
        FilePath base_name_;

        Data(log::LogLevel level, int old_errno, const FilePath& file, int line);

        void formatTime();
        void finish();
    };
    struct Data data_;

public:
    Logger(FilePath file, int line);
    Logger(FilePath file, int line, log::LogLevel level);
    Logger(FilePath file, int line, log::LogLevel level, const char* func);
    Logger(FilePath file, int line, bool to_abort);
    ~Logger();

    log::Stream& stream() { return data_.stream_; }

    static log::LogLevel logLevel();
    static void setLogLevel(log::LogLevel level);
    static void setOutput(Output output);
    static void setFlush(Flush flush);
    static void setTimeZone(const TimeZone& tz);
};

}

#define LOG_TRACE() \
    pginf::Logger(__FILE__, __LINE__, pginf::log::LogLevel::TRACE, __func__).stream()
#define LOG_DEBUG() \
    pginf::Logger(__FILE__, __LINE__, pginf::log::LogLevel::DEBUG, __func__).stream()
#define LOG_INFO() \
    pginf::Logger(__FILE__, __LINE__, pginf::log::LogLevel::INFO, __func__).stream()
#define LOG_WARNING() \
    pginf::Logger(__FILE__, __LINE__, pginf::log::LogLevel::WARNING, __func__).stream()
#define LOG_ERROR() \
    pginf::Logger(__FILE__, __LINE__, pginf::log::LogLevel::Error, __func__).stream()
#define LOG_FATAL() \
    pginf::Logger(__FILE__, __LINE__, pginf::log::LogLevel::FATAL, __func__).stream()
#define SYS_ERROR() \
    pginf::Logger(__FILE__, __LINE__, false).stream()
#define SYS_FATAL() \
    pginf::Logger(__FILE__, __LINE__, true).stream()

#ifdef PGINF_DEBUG
#define PGINF_ASSERT(val, what)                                 \
    do {                                                        \
        if (!(val))                                             \
            pginf::Logger(__FILE__, __LINE__, true).stream()    \
                << "Condition " << #val << " failed. " << what; \
    } while (0)
#else
#define PGINF_ASSERT(val, what) UNUSED(val), UNUSED(what)
#endif

#endif // !_PGINF_BASE_LOGGING_H_