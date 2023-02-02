#include <pginf/base/exception.h>

#include "base/thread/local.h"

namespace pginf {

Exception::Exception(std::string what)
    : what_(std::move(what))
    , stack_(current_thread::stackTrace(false))
{
}

}