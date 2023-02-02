#include "base/util/file.h"
#include <pginf/base/exception.h>
#include <pginf/base/logging.h>

#include <stdio.h>

namespace pginf {
namespace util {

    AppendFile::AppendFile(const std::string& file_name)
        : fp_(::fopen(file_name.c_str(), "ae")) // 'e' for O_CLOEXEC
    {
        if (!fp_)
            throw Exception("Cannot open file[" + file_name + "].");
        ::setbuffer(fp_, buffer_, sizeof(buffer_)); // posix_fadvise POSIX_FADV_DONTNEED ?
    }

    AppendFile::~AppendFile()
    {
        ::fclose(fp_);
    }

    void AppendFile::append(const char* log_line, const size_t len)
    {
        size_t written = 0;

        while (written != len) {
            size_t remain = len - written;
            size_t n = write(log_line + written, remain);
            if (n != remain) {
                if (auto err = ::ferror(fp_)) {
                    fprintf(stderr, "AppendFile::append() failed %s\n", log::strErrorno(err));
                    break;
                }
            }
            written += n;
        }

        written_bytes_ += written;
    }

    void AppendFile::flush()
    {
        ::fflush(fp_);
    }

    size_t AppendFile::write(const char* logline, size_t len)
    {
        return ::fwrite_unlocked(logline, 1, len, fp_);
    }

} // namespace util
} // namespace hare
