#include "base/util/file.h"
#include <pginf/base/exception.h>
#include <pginf/base/logging.h>
#include <pginf/base/util.h>

#include <stdio.h>

#ifdef P_OS_WIN32
#include <Windows.h>
#include <io.h>
#elif defined(P_OS_LINUX)
#include <dirent.h>
#endif

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

namespace file {

    void listFiles(std::list<std::string>& list,
        const std::string& folder_path, const std::string& extension, bool recursive)
    {
        // Empty string
        if (folder_path.empty())
            return;

#ifdef P_OS_WIN32
        // Open the file handle
        _finddata_t file_info;
        std::string current_path = folder_path + "/*.*";
        intptr_t handle = _findfirst(current_path.c_str(), &file_info);
        if (handle == -1) {
            LOG_ERROR() << "Cannot match the path[" << folder_path << "].";
            return;
        }

        // Search (recursive or not)
        do {
            // dir
            if (file_info.attrib == _A_SUBDIR) {
                if (strcmp(file_info.name, "..") != 0 && strcmp(file_info.name, ".") != 0 && recursive)
                    listFiles(list, folder_path + "/" + file_info.name, extension, recursive);
            } else {
                std::string file_name = file_info.name;
                auto last_dot = file_name.find_last_of('.');
                auto tmp = file_name.substr(last_dot + 1, extension.size());
                LOG_DEBUG() << "File: " << file_name
                            << ". File's ext:" << tmp;
                if (file_name.substr(last_dot + 1, extension.size()) == extension)
                    list.emplace_back(folder_path + "/" + file_info.name);
            }
        } while (!_findnext(handle, &file_info));

        // Close the file handle
        _findclose(handle);
#elif defined(P_OS_LINUX)
        // Open the file handle
        DIR* dir_info = ::opendir(folder_path.c_str());
        if (!dir_info) {
            LOG_ERROR() << "Cannot open folder[" << folder_path << "].";
            return;
        }

        while (auto dir_entry = readdir(dir_info)) {

            if (strcmp(dir_entry->d_name, "..") == 0 || strcmp(dir_entry->d_name, ".") == 0)
                continue;

            if (recursive && dir_entry->d_type == DT_DIR) {
                listFiles(list, folder_path + "/" + dir_entry->d_name, extension, recursive);
            } else {
                std::string file_name(dir_entry->d_name);
                auto last_dot = file_name.find_last_of('.');
                auto tmp = file_name.substr(last_dot + 1, extension.size());
                LOG_DEBUG() << "File: " << file_name
                            << ". File's ext:" << tmp;
                if (file_name.substr(last_dot + 1, extension.size()) == extension)
                    list.emplace_back(folder_path + "/" + dir_entry->d_name);
            }
            
        } // while

        closedir(dir_info);

#endif
    }

} // namespace file
} // namespace pginf
