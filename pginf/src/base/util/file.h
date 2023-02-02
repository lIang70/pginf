#ifndef _PGINF_BASE_FILE_H_
#define _PGINF_BASE_FILE_H_

#include "base/non_copyable.h"

#include <stdint.h>
#include <string>

#include <sys/types.h> // for off_t

namespace pginf {
namespace util {

    class AppendFile : public NonCopyable {
    public:
        static const int BUFFER_SIZE = 64 * 1024;

    private:
        FILE* fp_ { nullptr };
        char buffer_[BUFFER_SIZE] {};
        int64_t written_bytes_ { 0 };

    public:
        explicit AppendFile(const std::string& file_name);
        ~AppendFile();

        void append(const char* logline, std::size_t len);

        void flush();

        inline int64_t writtenBytes() const { return written_bytes_; }

    private:
        std::size_t write(const char* log_line, std::size_t len);
    };

} // namespace util
} // namespace pginf

#endif // !_PGINF_BASE_FILE_H_