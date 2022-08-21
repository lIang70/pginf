#pragma once
#ifndef _PGINF_PFLAG_H_
#define _PGINF_PFLAG_H_

#include <pginf/pginf_global.h>
#include <initializer_list>

PGINF_NAMESPACE_BEGIN()

template <typename Enum>
class PGINF_API PFlags {
    static_assert(bool(sizeof(Enum) <= sizeof(int)),
                      "pginf::PFlags uses an int as storage, so an enum with underlying "
                      "long long will overflow.");
    static_assert(bool(std::is_enum<Enum>::value), "pginf::PFlags is only usable on enumeration types.");

#if defined(_MSC_VER)
    using _Int          =   int;
#else
    using _Int          =   typename std::conditional<
                                std::is_unsigned<typename std::underlying_type<Enum>::type>::value,
                                unsigned int,
                                signed int
                            >::type;
#endif // defined(_MSC_VER)

private:
    _Int i;

private:
    static inline _Int initializer_list_helper( typename std::initializer_list<Enum>::const_iterator it,
                                                typename std::initializer_list<Enum>::const_iterator end)
    noexcept {
        return (it == end ? _Int(0) : (_Int(*it) | initializer_list_helper(it + 1, end)));
    }

public:
    inline PFlags() noexcept : i(0) {}
    inline PFlags(Enum flags) noexcept : i(_Int(flags)) {}
    inline PFlags(std::initializer_list<Enum> flags) noexcept
        : i(initializer_list_helper(flags.begin(), flags.end())) {}
    ~PFlags() = default;

    constexpr inline PFlags& operator&=(int mask)           noexcept { i &= mask;        return *this; }
    constexpr inline PFlags& operator&=(unsigned int mask)  noexcept { i &= mask;        return *this; }
    constexpr inline PFlags& operator&=(Enum mask)          noexcept { i &= _Int(mask);  return *this; }
    constexpr inline PFlags& operator|=(PFlags other)       noexcept { i |= other.i;     return *this; }
    constexpr inline PFlags& operator|=(Enum other)         noexcept { i |= _Int(other); return *this; }
    constexpr inline PFlags& operator^=(PFlags other)       noexcept { i ^= other.i;     return *this; }
    constexpr inline PFlags& operator^=(Enum other)         noexcept { i ^= _Int(other); return *this; }

    constexpr inline PFlags  operator|(PFlags other)        const noexcept { return PFlags((i | other.i)); }
    constexpr inline PFlags  operator|(Enum other)          const noexcept { return PFlags((i | _Int(other))); }
    constexpr inline PFlags  operator^(PFlags other)        const noexcept { return PFlags((i ^ other.i)); }
    constexpr inline PFlags  operator^(Enum other)          const noexcept { return PFlags((i ^ _Int(other))); }
    constexpr inline PFlags  operator&(int mask)            const noexcept { return PFlags((i & mask)); }
    constexpr inline PFlags  operator&(unsigned int mask)   const noexcept { return PFlags((i & mask)); }
    constexpr inline PFlags  operator&(Enum other)          const noexcept { return PFlags((i & _Int(other))); }
    constexpr inline PFlags  operator~()                    const noexcept { return PFlags((~i)); }

    constexpr inline bool    operator!()                    const noexcept { return !i; }

    constexpr inline _Int    ToInt()                        const noexcept { return i; }

    constexpr inline bool    TestFlag(Enum flag)            const noexcept { return (i & _Int(flag)) == _Int(flag) && (_Int(flag) != 0 || i == _Int(flag) ); }
    constexpr inline PFlags& SetFlag(Enum flag, bool on = true)   noexcept { return on ? (*this |= flag) : (*this &= ~_Int(flag)); }
};

#define P_DECLARE_FLAGS(Flags, Enum) \
typedef PFlags<Enum> Flags;

PGINF_NAMESPACE_END()

#endif // !_PGINF_PFLAG_H_