#ifndef _PGINF_BASE_NON_COPYABLE_H_
#define _PGINF_BASE_NON_COPYABLE_H_

namespace pginf {

class NonCopyable {
public:
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable operator=(const NonCopyable&) = delete;

protected:
    NonCopyable() = default;
    ~NonCopyable() = default;
};

}

#endif // !_PGINF_BASE_NON_COPYABLE_H_