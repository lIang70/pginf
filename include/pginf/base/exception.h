#ifndef _PGINF_BASE_EXCEPTION_H_
#define _PGINF_BASE_EXCEPTION_H_

#include <pginf/base/util.h>

#include <exception>
#include <string>

namespace pginf {

class PGINF_API Exception : std::exception {
    std::string what_ {};
    std::string stack_ {};

public:
    Exception(std::string what);
    ~Exception() = default;

    inline const char* what() const noexcept override
    {
        return what_.c_str();
    }

    inline const char* stackTrace() const noexcept
    {
        return stack_.c_str();
    }
};

}

#endif // !_PGINF_BASE_EXCEPTION_H_