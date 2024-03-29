#ifndef _PGINF_BASE_TIME_ZONE_H_
#define _PGINF_BASE_TIME_ZONE_H_

#include <pginf/base/detail/datetime.h>
#include <pginf/base/util.h>

#include <inttypes.h>
#include <memory>

namespace pginf {

class PGINF_API TimeZone {
    using DateTime = struct time::DateTime;

    struct Data;
    std::shared_ptr<Data> data_ { nullptr };

public:
    TimeZone() = default; // an invalid timezone
    TimeZone(int east_of_utc, const char* tz_name); // a fixed timezone

    static TimeZone UTC();
    // gmtime(3)
    static DateTime toUtcTime(int64_t seconds_since_epoch);
    // timegm(3)
    static int64_t fromUtcTime(const DateTime&);

    inline explicit operator bool() const { return static_cast<bool>(data_); }

    DateTime toLocalTime(int64_t seconds_since_epoch, int* utc_offset = nullptr) const;
    int64_t fromLocalTime(const DateTime&, bool post_transition = false) const;

private:
    explicit TimeZone(std::unique_ptr<Data> data);
};

} // namespace pginf

#endif // !_PGINF_BASE_TIME_ZONE_H_