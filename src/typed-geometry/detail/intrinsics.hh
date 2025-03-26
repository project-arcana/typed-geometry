#pragma once

#include <clean-core/macros.hh>

#ifdef _MSC_VER
#include <intrin.h>
#elif __has_include("x86intrin.h")
#include <x86intrin.h>
#include <cstring>
#endif


namespace tg::detail
{
CC_FORCE_INLINE char add_with_carry(char carry, unsigned long long a, unsigned long long b, unsigned long long* out)
{
#if defined(__x86_64__) || defined(_M_X64)
    return _addcarry_u64(carry, a, b, out);
#else
#pragma message("[typed-geometry] Using fallback for add_with_carry(...). This may be slow!")
    unsigned long long tmp = b + carry;
    a += tmp;
    *out = a;
    return (tmp < carry) + (a < tmp);
#endif
}

CC_FORCE_INLINE unsigned long long mul128(unsigned long long a, unsigned long long b, unsigned long long* high)
{
#if defined(_MSC_VER)
    return _umul128(a, b, high);
#elif __has_include("x86intrin.h")
    return _mulx_u64(a, b, high);
#elif defined(__SIZEOF_INT128__)
    __uint128_t res = __uint128_t(a) * __uint128_t(b);
    *high = res >> 64;
    return static_cast<unsigned long long>(res);
#else
#pragma message("[typed-geometry] Using fallback for mul128(...). This may be slow!")
    using u64 = unsigned long long;
    using u32 = unsigned int;
    static_assert(sizeof(u64) == 8, "u64 must be exactly 64 bits");
    static_assert(sizeof(u32) == 4, "u32 must be exactly 32 bits");

    u64 a_lo = u64(u32(a));
    u64 a_hi = a >> 32;
    u64 b_lo = u64(u32(b));
    u64 b_hi = b >> 32;

    u64 p0 = a_lo * b_lo;
    u64 p1 = a_lo * b_hi;
    u64 p2 = a_hi * b_lo;
    u64 p3 = a_hi * b_hi;

    u32 cy = u32(((p0 >> 32) + u32(p1) + u32(p2)) >> 32);

    *high = p3 + (p1 >> 32) + (p2 >> 32) + cy;
    return p0 + (p1 << 32) + (p2 << 32);
#endif
}
}
