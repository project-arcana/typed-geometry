#pragma once

#ifdef _MSC_VER
#include <intrin.h>
#else
#include <x86intrin.h>
#endif

#include <typed-geometry/functions/basic/minmax.hh>
#include <typed-geometry/types/scalars/fixed_uint.hh>

// todo: division and modulo are essentially the same code, put into detail maybe
// todo: gcc does not seem to produce optimal code gen

namespace tg
{
// comparison
template <int w0, int w1>
constexpr bool operator<(fixed_uint<w0> const& lhs, fixed_uint<w1> const& rhs) noexcept;
template <int w0, int w1>
constexpr bool operator>(fixed_uint<w0> const& lhs, fixed_uint<w1> const& rhs) noexcept;
template <int w0, int w1>
constexpr bool operator<=(fixed_uint<w0> const& lhs, fixed_uint<w1> const& rhs) noexcept;
template <int w0, int w1>
constexpr bool operator>=(fixed_uint<w0> const& lhs, fixed_uint<w1> const& rhs) noexcept;

template <int w>
constexpr bool operator<(u64 lhs, fixed_uint<w> const& rhs) noexcept;
template <int w>
constexpr bool operator>(u64 lhs, fixed_uint<w> const& rhs) noexcept;
template <int w>
constexpr bool operator<=(u64 lhs, fixed_uint<w> const& rhs) noexcept;
template <int w>
constexpr bool operator>=(u64 lhs, fixed_uint<w> const& rhs) noexcept;

template <int w>
constexpr bool operator<(fixed_uint<w> const& lhs, u64 rhs) noexcept;
template <int w>
constexpr bool operator>(fixed_uint<w> const& lhs, u64 rhs) noexcept;
template <int w>
constexpr bool operator<=(fixed_uint<w> const& lhs, u64 rhs) noexcept;
template <int w>
constexpr bool operator>=(fixed_uint<w> const& lhs, u64 rhs) noexcept;

// standard arithmetics

template <int w0, int w1>
constexpr fixed_uint<max(w0, w1)> operator+(fixed_uint<w0> const& lhs, fixed_uint<w1> const& rhs) noexcept;
template <int w0, int w1>
constexpr fixed_uint<max(w0, w1)> operator-(fixed_uint<w0> const& lhs, fixed_uint<w1> const& rhs) noexcept;
template <int w0, int w1>
constexpr fixed_uint<max(w0, w1)> operator*(fixed_uint<w0> const& lhs, fixed_uint<w1> const& rhs) noexcept;
template <int w0, int w1>
constexpr fixed_uint<max(w0, w1)> operator/(fixed_uint<w0> const& lhs, fixed_uint<w1> const& rhs) noexcept;
template <int w0, int w1>
constexpr fixed_uint<max(w0, w1)> operator%(fixed_uint<w0> const& lhs, fixed_uint<w1> const& rhs) noexcept;

template <int w>
constexpr fixed_uint<w> operator+(u64 lhs, fixed_uint<w> const& rhs) noexcept;
template <int w>
constexpr fixed_uint<w> operator-(u64 lhs, fixed_uint<w> const& rhs) noexcept;
template <int w>
constexpr fixed_uint<w> operator*(u64 lhs, fixed_uint<w> const& rhs) noexcept;
template <int w>
constexpr fixed_uint<w> operator/(u64 lhs, fixed_uint<w> const& rhs) noexcept;
template <int w>
constexpr fixed_uint<w> operator%(u64 lhs, fixed_uint<w> const& rhs) noexcept;

template <int w>
constexpr fixed_uint<w> operator+(fixed_uint<w> const& lhs, u64 rhs) noexcept;
template <int w>
constexpr fixed_uint<w> operator-(fixed_uint<w> const& lhs, u64 rhs) noexcept;
template <int w>
constexpr fixed_uint<w> operator*(fixed_uint<w> const& lhs, u64 rhs) noexcept;
template <int w>
constexpr fixed_uint<w> operator/(fixed_uint<w> const& lhs, u64 rhs) noexcept;
template <int w>
constexpr fixed_uint<w> operator%(fixed_uint<w> const& lhs, u64 rhs) noexcept;

// assignment arithmetics

template <int w0, int w1>
constexpr fixed_uint<w0>& operator+=(fixed_uint<w0>& lhs, fixed_uint<w1> const& rhs) noexcept;
template <int w0, int w1>
constexpr fixed_uint<w0>& operator-=(fixed_uint<w0>& lhs, fixed_uint<w1> const& rhs) noexcept;
template <int w0, int w1>
constexpr fixed_uint<w0>& operator*=(fixed_uint<w0>& lhs, fixed_uint<w1> const& rhs) noexcept;
template <int w0, int w1>
constexpr fixed_uint<w0>& operator/=(fixed_uint<w0>& lhs, fixed_uint<w1> const& rhs) noexcept;
template <int w0, int w1>
constexpr fixed_uint<w0>& operator%=(fixed_uint<w0>& lhs, fixed_uint<w1> const& rhs) noexcept;

template <int w>
constexpr fixed_uint<w>& operator+=(fixed_uint<w>& lhs, u64 rhs) noexcept;
template <int w>
constexpr fixed_uint<w>& operator-=(fixed_uint<w>& lhs, u64 rhs) noexcept;
template <int w>
constexpr fixed_uint<w>& operator*=(fixed_uint<w>& lhs, u64 rhs) noexcept;
template <int w>
constexpr fixed_uint<w>& operator/=(fixed_uint<w>& lhs, u64 rhs) noexcept;
template <int w>
constexpr fixed_uint<w>& operator%=(fixed_uint<w>& lhs, u64 rhs) noexcept;

// increment / decrement

template <int w>
constexpr fixed_uint<w>& operator++(fixed_uint<w>& lhs) noexcept;
template <int w>
constexpr fixed_uint<w> operator++(fixed_uint<w>& lhs, int) noexcept;
template <int w>
constexpr fixed_uint<w>& operator--(fixed_uint<w>& lhs) noexcept;
template <int w>
constexpr fixed_uint<w> operator--(fixed_uint<w>& lhs, int) noexcept;

// bitwise logic

template <int w0, int w1>
constexpr fixed_uint<max(w0, w1)> operator|(fixed_uint<w0> const& lhs, fixed_uint<w1> const& rhs) noexcept;
template <int w0, int w1>
constexpr fixed_uint<max(w0, w1)> operator&(fixed_uint<w0> const& lhs, fixed_uint<w1> const& rhs) noexcept;
template <int w0, int w1>
constexpr fixed_uint<max(w0, w1)> operator^(fixed_uint<w0> const& lhs, fixed_uint<w1> const& rhs) noexcept;
template <int w>
constexpr fixed_uint<w> operator~(fixed_uint<w> const& lhs) noexcept;

template <int w>
constexpr fixed_uint<w> operator|(u64 lhs, fixed_uint<w> const& rhs) noexcept;
template <int w>
constexpr fixed_uint<w> operator&(u64 lhs, fixed_uint<w> const& rhs) noexcept;
template <int w>
constexpr fixed_uint<w> operator^(u64 lhs, fixed_uint<w> const& rhs) noexcept;

template <int w>
constexpr fixed_uint<w> operator|(fixed_uint<w> const& lhs, u64 rhs) noexcept;
template <int w>
constexpr fixed_uint<w> operator&(fixed_uint<w> const& lhs, u64 rhs) noexcept;
template <int w>
constexpr fixed_uint<w> operator^(fixed_uint<w> const& lhs, u64 rhs) noexcept;

// assignment bit ops

template <int w0, int w1>
constexpr fixed_uint<w0>& operator|=(fixed_uint<w0>& lhs, fixed_uint<w1> const& rhs) noexcept;
template <int w0, int w1>
constexpr fixed_uint<w0>& operator&=(fixed_uint<w0>& lhs, fixed_uint<w1> const& rhs) noexcept;
template <int w0, int w1>
constexpr fixed_uint<w0>& operator^=(fixed_uint<w0>& lhs, fixed_uint<w1> const& rhs) noexcept;

template <int w>
constexpr fixed_uint<w>& operator|=(fixed_uint<w>& lhs, u64 rhs) noexcept;
template <int w>
constexpr fixed_uint<w>& operator&=(fixed_uint<w>& lhs, u64 rhs) noexcept;
template <int w>
constexpr fixed_uint<w>& operator^=(fixed_uint<w>& lhs, u64 rhs) noexcept;

template <int w>
constexpr fixed_uint<w>& operator|=(u64& lhs, fixed_uint<w> const& rhs) noexcept;
template <int w>
constexpr fixed_uint<w>& operator&=(u64& lhs, fixed_uint<w> const& rhs) noexcept;
template <int w>
constexpr fixed_uint<w>& operator^=(u64& lhs, fixed_uint<w> const& rhs) noexcept;

// shift

template <int w>
constexpr fixed_uint<w> operator>>(fixed_uint<w> const& lhs, int shift) noexcept;
template <int w>
constexpr fixed_uint<w> operator<<(fixed_uint<w> const& lhs, int shift) noexcept;

// assignment shift

template <int w>
constexpr fixed_uint<w>& operator>>=(fixed_uint<w>& lhs, int shift) noexcept;
template <int w>
constexpr fixed_uint<w>& operator<<=(fixed_uint<w>& lhs, int shift) noexcept;

// muliplication specializations

namespace detail
{
template <int w_res, class T0, class T1>
fixed_uint<w_res> mul(T0 lhs, T1 rhs);
}

// utility

template <int w>
[[nodiscard]] constexpr u64 leading_zeros_count(fixed_uint<w> const& v);

template <int w>
[[nodiscard]] constexpr u64 leading_ones_count(fixed_uint<w> const& v);

template <int w>
[[nodiscard]] constexpr u64 trailing_zeros_count(fixed_uint<w> const& v);

template <int w>
[[nodiscard]] constexpr u64 trailing_ones_count(fixed_uint<w> const& v);

template <int w>
[[nodiscard]] constexpr bool is_zero(fixed_int<w> const& v);

template <int w0, int w1>
[[nodiscard]] fixed_uint<max(w0, w1)> gcd(fixed_uint<w0> const& a, fixed_uint<w1> const& b);

template <int w>
[[nodiscard]] fixed_uint<w> gcd(u64 a, fixed_uint<w> const& b);

template <int w>
[[nodiscard]] fixed_uint<w> gcd(fixed_uint<w> const& a, u64 b);

//#############################################################################
//#                             implemenation                                 #
//#############################################################################

template <int words>
template <int rhs_words, class, class>
constexpr fixed_uint<words>::fixed_uint(fixed_uint<rhs_words> rhs)
{
    // explicit from larger type
    static_assert(words < rhs_words);
    d[0] = rhs.d[0];
    if constexpr (words > 1)
        d[1] = rhs.d[1];
    if constexpr (words > 2)
        d[2] = rhs.d[2];
    // not needed:
    //    if constexpr (words > 3)
    //        d[3] = rhs.d[3];
    static_assert(words <= 3, "should only cast down");
}

template <int words>
template <int rhs_words, class>
constexpr fixed_uint<words>::fixed_uint(fixed_uint<rhs_words> rhs)
{
    // implicit from smaller type
    static_assert(rhs_words < words);
    d[0] = rhs.d[0];
    if constexpr (rhs_words > 1)
        d[1] = rhs.d[1];
    if constexpr (rhs_words > 2)
        d[2] = rhs.d[2];

    // not needed:
    //    if constexpr (rhs_words > 3)
    //        d[3] = rhs.d[3];
    static_assert(rhs_words <= 3, "should only cast up");
}

template <int words>
constexpr fixed_uint<words>::operator float() const
{
    float res = float(d[0]);
    if constexpr (words > 1)
        res += float(d[1]) * 0x1p64f;
    if constexpr (words > 2) // if not 0 this is going to be inf anyways, but inf * 0 = nan, which is not what is desired here
        res += d[2] == 0 ? 0 : float(d[2]) * float(0x1p128);
    if constexpr (words > 3) // if not 0 this is going to be inf anyways, but inf * 0 = nan, which is not what is desired here
        res += d[3] == 0 ? 0 : float(d[3]) * float(0x1p128);
    return res;
}

template <int words>
constexpr fixed_uint<words>::operator double() const
{
    double res = double(d[0]);
    if constexpr (words > 1)
        res += double(d[1]) * 0x1p64;
    if constexpr (words > 2)
        res += double(d[2]) * 0x1p128;
    if constexpr (words > 3)
        res += double(d[3]) * 0x1p192;
    return res;
}

//
// comparison
//

template <int w0, int w1>
constexpr bool operator<(fixed_uint<w0> const& lhs, fixed_uint<w1> const& rhs) noexcept
{
    if constexpr (w0 > w1 + 0)
        if (lhs.d[w1 + 0] != 0)
            return false;
    if constexpr (w0 > w1 + 1)
        if (lhs.d[w1 + 1] != 0)
            return false;
    if constexpr (w0 > w1 + 2)
        if (lhs.d[w1 + 2] != 0)
            return false;

    if constexpr (w0 + 0 < w1)
        if (rhs.d[w0 + 0] != 0)
            return true;
    if constexpr (w0 + 1 < w1)
        if (rhs.d[w0 + 1] != 0)
            return true;
    if constexpr (w0 + 2 < w1)
        if (rhs.d[w0 + 2] != 0)
            return true;

    if constexpr (w0 > 3 && w1 > 3)
    {
        if (lhs.d[3] > rhs.d[3])
            return false;
        if (lhs.d[3] < rhs.d[3])
            return true;
    }
    if constexpr (w0 > 2 && w1 > 2)
    {
        if (lhs.d[2] > rhs.d[2])
            return false;
        if (lhs.d[2] < rhs.d[2])
            return true;
    }
    if constexpr (w0 > 1 && w1 > 1)
    {
        if (lhs.d[1] > rhs.d[1])
            return false;
        if (lhs.d[1] < rhs.d[1])
            return true;
    }
    if (lhs.d[0] < rhs.d[0])
        return true;

    return false; // equal
}

template <int w>
constexpr bool operator<(u64 lhs, fixed_uint<w> const& rhs) noexcept
{
    bool lt = lhs < rhs.d[0];
    if constexpr (w > 1)
        lt |= rhs.d[1] != 0;
    if constexpr (w > 2)
        lt |= rhs.d[2] != 0;
    if constexpr (w > 3)
        lt |= rhs.d[3] != 0;
    return lt;
}

template <int w>
constexpr bool operator<(fixed_uint<w> const& lhs, u64 rhs) noexcept
{
    bool lt = lhs.d[0] < rhs;
    if constexpr (w > 1)
        lt &= lhs.d[1] == 0;
    if constexpr (w > 2)
        lt &= lhs.d[2] == 0;
    if constexpr (w > 3)
        lt &= lhs.d[3] == 0;
    return lt;
}

template <int w0, int w1>
constexpr bool operator>(fixed_uint<w0> const& lhs, fixed_uint<w1> const& rhs) noexcept
{
    return rhs < lhs;
}

template <int w>
constexpr bool operator>(u64 lhs, fixed_uint<w> const& rhs) noexcept
{
    return rhs < lhs;
}

template <int w>
constexpr bool operator>(fixed_uint<w> const& lhs, u64 rhs) noexcept
{
    return rhs < lhs;
}

template <int w0, int w1>
constexpr bool operator<=(fixed_uint<w0> const& lhs, fixed_uint<w1> const& rhs) noexcept
{
    return !(rhs < lhs);
}

template <int w>
constexpr bool operator<=(u64 lhs, fixed_uint<w> const& rhs) noexcept
{
    return !(rhs < lhs);
}

template <int w>
constexpr bool operator<=(fixed_uint<w> const& lhs, u64 rhs) noexcept
{
    return !(rhs < lhs);
}

template <int w0, int w1>
constexpr bool operator>=(fixed_uint<w0> const& lhs, fixed_uint<w1> const& rhs) noexcept
{
    return rhs <= lhs;
}

template <int w>
constexpr bool operator>=(u64 lhs, fixed_uint<w> const& rhs) noexcept
{
    return rhs <= lhs;
}

template <int w>
constexpr bool operator>=(fixed_uint<w> const& lhs, u64 rhs) noexcept
{
    return rhs <= lhs;
}

//
//  standard arithmetics
//

template <int w0, int w1>
constexpr fixed_uint<max(w0, w1)> operator+(fixed_uint<w0> const& lhs, fixed_uint<w1> const& rhs) noexcept
{
    constexpr int w_out = max(w0, w1);
    fixed_uint<w_out> res;
    fixed_uint<w_out> l = lhs;
    fixed_uint<w_out> r = rhs;
    decltype(_addcarry_u64(0, 0, 0, nullptr)) c = 0;

    c = _addcarry_u64(c, l.d[0], r.d[0], &res.d[0]);
    if constexpr (w_out > 1)
        c = _addcarry_u64(c, l.d[1], r.d[1], &res.d[1]);
    if constexpr (w_out > 2)
        c = _addcarry_u64(c, l.d[2], r.d[2], &res.d[2]);
    if constexpr (w_out > 3)
        c = _addcarry_u64(c, l.d[3], r.d[3], &res.d[3]);

    return res;
}

template <int w>
constexpr fixed_uint<w> operator+(u64 lhs, fixed_uint<w> const& rhs) noexcept
{
    return fixed_uint<w>(lhs) + rhs;
}

template <int w>
constexpr fixed_uint<w> operator+(fixed_uint<w> const& lhs, u64 rhs) noexcept
{
    return lhs + fixed_uint<w>(rhs);
}

template <int w0, int w1>
constexpr fixed_uint<max(w0, w1)> operator-(fixed_uint<w0> const& lhs, fixed_uint<w1> const& rhs) noexcept
{
    constexpr int w_out = max(w0, w1);
    fixed_uint<w_out> res;
    fixed_uint<w_out> l = lhs;
    fixed_uint<w_out> r = rhs;
    decltype(_subborrow_u64(0, 0, 0, nullptr)) c = 0;

    c = _subborrow_u64(c, l.d[0], r.d[0], &res.d[0]);
    if constexpr (w_out > 1)
        c = _subborrow_u64(c, l.d[1], r.d[1], &res.d[1]);
    if constexpr (w_out > 2)
        c = _subborrow_u64(c, l.d[2], r.d[2], &res.d[2]);
    if constexpr (w_out > 3)
        c = _subborrow_u64(c, l.d[3], r.d[3], &res.d[3]);

    return res;
}

template <int w>
constexpr fixed_uint<w> operator-(u64 lhs, fixed_uint<w> const& rhs) noexcept
{
    return fixed_uint<w>(lhs) - rhs;
}

template <int w>
constexpr fixed_uint<w> operator-(fixed_uint<w> const& lhs, u64 rhs) noexcept
{
    return lhs - fixed_uint<w>(rhs);
}

template <int w0, int w1>
constexpr fixed_uint<max(w0, w1)> operator*(fixed_uint<w0> const& lhs, fixed_uint<w1> const& rhs) noexcept
{
    constexpr int w_out = max(w0, w1);
    fixed_uint<w_out> l = lhs;
    fixed_uint<w_out> r = rhs;
    return detail::mul<w_out>(l, r);
}

template <int w>
constexpr fixed_uint<w> operator*(u64 lhs, fixed_uint<w> const& rhs) noexcept
{
    return detail::mul<w>(lhs, rhs);
}

template <int w>
constexpr fixed_uint<w> operator*(fixed_uint<w> const& lhs, u64 rhs) noexcept
{
    return detail::mul<w>(lhs, rhs);
}

template <int w0, int w1>
constexpr fixed_uint<max(w0, w1)> operator/(fixed_uint<w0> const& lhs, fixed_uint<w1> const& rhs) noexcept
{
    TG_ASSERT(rhs != 0 && "division by zero!");

    constexpr int w_out = max(w0, w1);
    fixed_uint<w_out> l = lhs;
    fixed_uint<w_out> r = rhs;

    /// see https://en.wikipedia.org/wiki/Division_algorithm#Integer_division_(unsigned)_with_remainder
    fixed_uint<w_out> quotient = 0;
    fixed_uint<w_out> remainder = 0;
    for (auto i = (w_out * 64) - 1; i >= 0; --i)
    {
        remainder <<= 1;
        auto const word = i / 64;
        auto const idx = i % 64;
        remainder.d[0] |= (l.d[word] & (u64(1) << idx)) >> idx;
        if (remainder >= r)
        {
            remainder -= r;
            quotient.d[word] |= u64(1) << idx;
        }
    }
    return quotient;
}

template <int w>
constexpr fixed_uint<w> operator/(u64 lhs, fixed_uint<w> const& rhs) noexcept
{
    return fixed_uint<1>(lhs) / rhs;
}

template <int w>
constexpr fixed_uint<w> operator/(fixed_uint<w> const& lhs, u64 rhs) noexcept
{
    return lhs / fixed_uint<1>(rhs);
}

template <int w0, int w1>
constexpr fixed_uint<max(w0, w1)> operator%(fixed_uint<w0> const& lhs, fixed_uint<w1> const& rhs) noexcept
{
    TG_ASSERT(rhs != 0 && "division by zero!");
    constexpr int w_out = max(w0, w1);
    fixed_uint<w_out> l = lhs;
    fixed_uint<w_out> r = rhs;

    /// see https://en.wikipedia.org/wiki/Division_algorithm#Integer_division_(unsigned)_with_remainder
    fixed_uint<w_out> quotient = 0;
    fixed_uint<w_out> remainder = 0;
    for (auto i = (w_out * 64) - 1; i >= 0; --i)
    {
        remainder <<= 1;
        auto const word = i / 64;
        auto const idx = i % 64;
        remainder.d[0] |= (l.d[word] & (u64(1) << idx)) >> idx;
        if (remainder >= r)
        {
            remainder -= r;
            quotient.d[word] |= u64(1) << idx;
        }
    }
    return remainder;
}

template <int w>
constexpr fixed_uint<w> operator%(u64 lhs, fixed_uint<w> const& rhs) noexcept
{
    return fixed_uint<1>(lhs) % rhs;
}

template <int w>
constexpr fixed_uint<w> operator%(fixed_uint<w> const& lhs, u64 rhs) noexcept
{
    return lhs % fixed_uint<1>(rhs);
}

// assignment arithmetics

template <int w0, int w1>
constexpr fixed_uint<w0>& operator+=(fixed_uint<w0>& lhs, fixed_uint<w1> const& rhs) noexcept
{
    lhs = lhs + rhs;
    return lhs;
}

template <int w>
constexpr fixed_uint<w>& operator+=(fixed_uint<w>& lhs, u64 rhs) noexcept
{
    lhs = lhs + rhs;
    return lhs;
}

template <int w0, int w1>
constexpr fixed_uint<w0>& operator-=(fixed_uint<w0>& lhs, fixed_uint<w1> const& rhs) noexcept
{
    lhs = lhs - rhs;
    return lhs;
}

template <int w>
constexpr fixed_uint<w>& operator-=(fixed_uint<w>& lhs, u64 rhs) noexcept
{
    lhs = lhs - rhs;
    return lhs;
}

template <int w0, int w1>
constexpr fixed_uint<w0>& operator*=(fixed_uint<w0>& lhs, fixed_uint<w1> const& rhs) noexcept
{
    lhs = lhs * rhs;
    return lhs;
}

template <int w>
constexpr fixed_uint<w>& operator*=(fixed_uint<w>& lhs, u64 rhs) noexcept
{
    lhs = lhs * rhs;
    return lhs;
}

template <int w0, int w1>
constexpr fixed_uint<w0>& operator/=(fixed_uint<w0>& lhs, fixed_uint<w1> const& rhs) noexcept
{
    lhs = lhs / rhs;
    return lhs;
}

template <int w>
constexpr fixed_uint<w>& operator/=(fixed_uint<w>& lhs, u64 rhs) noexcept
{
    lhs = lhs / rhs;
    return lhs;
}

template <int w0, int w1>
constexpr fixed_uint<w0>& operator%=(fixed_uint<w0>& lhs, fixed_uint<w1> const& rhs) noexcept
{
    lhs = lhs % rhs;
    return lhs;
}

template <int w>
constexpr fixed_uint<w>& operator%=(fixed_uint<w>& lhs, u64 rhs) noexcept
{
    lhs = lhs % rhs;
    return lhs;
}

// increment / decrement

template <int w>
constexpr fixed_uint<w>& operator++(fixed_uint<w>& lhs) noexcept
{
    [[maybe_unused]] u64 c0 = 0;
    [[maybe_unused]] u64 c1 = 0;
    [[maybe_unused]] u64 c2 = 0;

    ++lhs.d[0];
    if constexpr (w > 1)
    {
        c0 = lhs.d[0] == 0;
        lhs.d[1] += c0;
    }
    if constexpr (w > 2)
    {
        c1 = (lhs.d[1] == 0) & c0;
        lhs.d[2] += c1;
    }
    if constexpr (w > 3)
    {
        c2 = (lhs.d[2] == 0) & c1;
        lhs.d[3] += c2;
    }
    return lhs;
}

template <int w>
constexpr fixed_uint<w> operator++(fixed_uint<w>& lhs, int) noexcept
{
    fixed_uint<w> cpy = lhs;
    ++lhs;
    return cpy;
}

template <int w>
constexpr fixed_uint<w>& operator--(fixed_uint<w>& lhs) noexcept
{
    [[maybe_unused]] u64 c0 = 0;
    [[maybe_unused]] u64 c1 = 0;
    [[maybe_unused]] u64 c2 = 0;

    --lhs.d[0];
    if constexpr (w > 1)
    {
        c0 = lhs.d[0] == u64(-1);
        lhs.d[1] -= c0;
    }
    if constexpr (w > 2)
    {
        c1 = (lhs.d[1] == u64(-1)) & c0;
        lhs.d[2] -= c1;
    }
    if constexpr (w > 3)
    {
        c2 = (lhs.d[2] == u64(-1)) & c1;
        lhs.d[3] -= c2;
    }
    return lhs;
}

template <int w>
constexpr fixed_uint<w> operator--(fixed_uint<w>& lhs, int) noexcept
{
    fixed_uint<w> cpy = lhs;
    --lhs;
    return cpy;
}

// bitwise logic

template <int w0, int w1>
constexpr fixed_uint<max(w0, w1)> operator|(fixed_uint<w0> const& lhs, fixed_uint<w1> const& rhs) noexcept
{
    fixed_uint<max(w0, w1)> res;
    fixed_uint<max(w0, w1)> l = lhs;
    fixed_uint<max(w0, w1)> r = rhs;

    res.d[0] = l.d[0] | r.d[0];
    if constexpr (max(w0, w1) > 1)
        res.d[1] = l.d[1] | r.d[1];
    if constexpr (max(w0, w1) > 2)
        res.d[2] = l.d[2] | r.d[2];
    if constexpr (max(w0, w1) > 3)
        res.d[3] = l.d[3] | r.d[3];

    return res;
}

template <int w>
constexpr fixed_uint<w> operator|(u64 lhs, fixed_uint<w> const& rhs) noexcept
{
    fixed_uint<w> res = rhs;
    res.d[0] |= lhs;
    return res;
}

template <int w>
constexpr fixed_uint<w> operator|(fixed_uint<w> const& lhs, u64 rhs) noexcept
{
    fixed_uint<w> res = lhs;
    res.d[0] |= rhs;
    return res;
}

template <int w0, int w1>
constexpr fixed_uint<max(w0, w1)> operator&(fixed_uint<w0> const& lhs, fixed_uint<w1> const& rhs) noexcept
{
    fixed_uint<max(w0, w1)> res;
    fixed_uint<max(w0, w1)> l = lhs;
    fixed_uint<max(w0, w1)> r = rhs;

    res.d[0] = l.d[0] & r.d[0];
    if constexpr (max(w0, w1) > 1)
        res.d[1] = l.d[1] & r.d[1];
    if constexpr (max(w0, w1) > 2)
        res.d[2] = l.d[2] & r.d[2];
    if constexpr (max(w0, w1) > 3)
        res.d[3] = l.d[3] & r.d[3];

    return res;
}

template <int w>
constexpr fixed_uint<w> operator&(u64 lhs, fixed_uint<w> const& rhs) noexcept
{
    fixed_uint<w> res = rhs;
    res.d[0] &= lhs;
    // this works as if lhs is promoted, meaning the uppper bits are 0.
    if constexpr (w > 1)
        res.d[1] = 0;
    if constexpr (w > 2)
        res.d[2] = 0;
    if constexpr (w > 3)
        res.d[3] = 0;
    return res;
}

template <int w>
constexpr fixed_uint<w> operator&(fixed_uint<w> const& lhs, u64 rhs) noexcept
{
    fixed_uint<w> res = lhs;
    res.d[0] &= rhs;
    // this works as if rhs is promoted, meaning the uppper bits are 0.
    if constexpr (w > 1)
        res.d[1] = 0;
    if constexpr (w > 2)
        res.d[2] = 0;
    if constexpr (w > 3)
        res.d[3] = 0;
    return res;
}

template <int w0, int w1>
constexpr fixed_uint<max(w0, w1)> operator^(fixed_uint<w0> const& lhs, fixed_uint<w1> const& rhs) noexcept
{
    fixed_uint<max(w0, w1)> res;
    fixed_uint<max(w0, w1)> l = lhs;
    fixed_uint<max(w0, w1)> r = rhs;

    res.d[0] = l.d[0] ^ r.d[0];
    if constexpr (max(w0, w1) > 1)
        res.d[1] = l.d[1] ^ r.d[1];
    if constexpr (max(w0, w1) > 2)
        res.d[2] = l.d[2] ^ r.d[2];
    if constexpr (max(w0, w1) > 3)
        res.d[3] = l.d[3] ^ r.d[3];

    return res;
}

template <int w>
constexpr fixed_uint<w> operator^(u64 lhs, fixed_uint<w> const& rhs) noexcept
{
    fixed_uint<w> res = rhs;
    res.d[0] ^= lhs;
    return res;
}

template <int w>
constexpr fixed_uint<w> operator^(fixed_uint<w> const& lhs, u64 rhs) noexcept
{
    fixed_uint<w> res = lhs;
    res.d[0] ^= rhs;
    return res;
}

template <int w>
constexpr fixed_uint<w> operator~(fixed_uint<w> const& lhs) noexcept
{
    fixed_uint<w> res;
    res.d[0] = ~lhs.d[0];
    if constexpr (w > 1)
        res.d[1] = ~lhs.d[1];
    if constexpr (w > 2)
        res.d[2] = ~lhs.d[2];
    if constexpr (w > 3)
        res.d[3] = ~lhs.d[3];
    return res;
}

// assignment bit ops

template <int w0, int w1>
constexpr fixed_uint<w0>& operator|=(fixed_uint<w0>& lhs, fixed_uint<w1> const& rhs) noexcept
{
    fixed_uint<w0> r = fixed_uint<w0>(rhs);

    lhs.d[0] |= r.d[0];
    if constexpr (w0 > 1)
        lhs.d[1] |= r.d[1];
    if constexpr (w0 > 2)
        lhs.d[2] |= r.d[2];
    if constexpr (w0 > 3)
        lhs.d[3] |= r.d[3];

    return lhs;
}

template <int w>
constexpr fixed_uint<w>& operator|=(fixed_uint<w>& lhs, u64 rhs) noexcept
{
    lhs.d[0] |= rhs;
    return lhs;
}

template <int w>
constexpr u64& operator|=(u64& lhs, fixed_uint<w> const& rhs) noexcept
{
    lhs |= rhs.d[0];
    return lhs;
}

template <int w0, int w1>
constexpr fixed_uint<w0>& operator&=(fixed_uint<w0>& lhs, fixed_uint<w1> const& rhs) noexcept
{
    fixed_uint<w0> r = fixed_uint<w0>(rhs);

    lhs.d[0] &= r.d[0];
    if constexpr (w0 > 1)
        lhs.d[1] &= r.d[1];
    if constexpr (w0 > 2)
        lhs.d[2] &= r.d[2];
    if constexpr (w0 > 3)
        lhs.d[3] &= r.d[3];

    return lhs;
}

template <int w>
constexpr fixed_uint<w>& operator&=(fixed_uint<w>& lhs, u64 rhs) noexcept
{
    lhs.d[0] |= rhs;
    // this works as if rhs is promoted, meaning the uppper bits are 0.
    if constexpr (w > 1)
        lhs.d[1] = 0;
    if constexpr (w > 2)
        lhs.d[2] = 0;
    if constexpr (w > 3)
        lhs.d[3] = 0;
    return lhs;
}

template <int w>
constexpr u64& operator&=(u64& lhs, fixed_uint<w> const& rhs) noexcept
{
    lhs |= rhs.d[0];
    return lhs;
}

template <int w0, int w1>
constexpr fixed_uint<w0>& operator^=(fixed_uint<w0>& lhs, fixed_uint<w1> const& rhs) noexcept
{
    fixed_uint<w0> r = fixed_uint<w0>(rhs);

    lhs.d[0] ^= r.d[0];
    if constexpr (w0 > 1)
        lhs.d[1] ^= r.d[1];
    if constexpr (w0 > 2)
        lhs.d[2] ^= r.d[2];
    if constexpr (w0 > 3)
        lhs.d[3] ^= r.d[3];

    return lhs;
}

template <int w>
constexpr fixed_uint<w>& operator^=(fixed_uint<w>& lhs, u64 rhs) noexcept
{
    lhs.d[0] ^= rhs;
    return lhs;
}

template <int w>
constexpr u64& operator^=(u64& lhs, fixed_uint<w> const& rhs) noexcept
{
    lhs ^= rhs.d[0];
    return lhs;
}

// shift

template <int w>
constexpr fixed_uint<w> operator>>(fixed_uint<w> const& lhs, int shift) noexcept
{
    TG_ASSERT(0 <= shift && shift < (w * 64)); // similar to default types this would be UB

    if (shift == 0)
        return lhs;

    const int mod_shift = shift % 64;     // avoid ub
    const int skip = shift / 64;          // count skipped words
    const int inv_shift = 64 - mod_shift; // inverted shift

    fixed_uint<w> res;

    if (mod_shift == 0)
    {
        if (skip == 1)
        {
            if constexpr (w > 1)
                res.d[0] = lhs.d[1];
            if constexpr (w > 2)
                res.d[1] = lhs.d[2];
            if constexpr (w > 3)
                res.d[2] = lhs.d[3];
        }
        if (skip == 2)
        {
            if constexpr (w > 2)
                res.d[0] = lhs.d[2];
            if constexpr (w > 3)
                res.d[1] = lhs.d[3];
        }
        if (skip == 3)
        {
            if constexpr (w > 3)
                res.d[0] = lhs.d[3];
        }
        return res;
    }


#if 1
    if (skip == 0)
    {
        res.d[0] = lhs.d[0] >> mod_shift;
        if constexpr (w > 1)
        {
            res.d[0] |= lhs.d[1] << inv_shift;
            res.d[1] = lhs.d[1] >> mod_shift;
        }
        if constexpr (w > 2)
        {
            res.d[1] |= lhs.d[2] << inv_shift;
            res.d[2] = lhs.d[2] >> mod_shift;
        }
        if constexpr (w > 3)
        {
            res.d[2] |= lhs.d[3] << inv_shift;
            res.d[3] = lhs.d[3] >> mod_shift;
        }
    }
    else if (skip == 1)
    {
        if constexpr (w > 1)
        {
            res.d[0] = lhs.d[1] >> mod_shift;
        }
        if constexpr (w > 2)
        {
            res.d[0] |= lhs.d[2] << inv_shift;
            res.d[1] = lhs.d[2] >> mod_shift;
        }
        if constexpr (w > 3)
        {
            res.d[1] |= lhs.d[3] << inv_shift;
            res.d[2] = lhs.d[3] >> mod_shift;
        }
    }
    else if (skip == 2)
    {
        if constexpr (w > 2)
        {
            res.d[0] = lhs.d[2] >> mod_shift;
        }
        if constexpr (w > 3)
        {
            res.d[0] |= lhs.d[3] << inv_shift;
            res.d[1] = lhs.d[3] >> mod_shift;
        }
    }
    else if (skip == 3)
    {
        if constexpr (w > 3)
        {
            res.d[0] = lhs.d[3] >> mod_shift;
        }
    }
#else
    if constexpr (w == 2)
    {
        if (skip == 0)
        {
            res.d[0] = lhs.d[0] >> mod_shift;
            res.d[0] |= lhs.d[1] << inv_shift;
            res.d[1] = lhs.d[1] >> mod_shift;
        }
        else // if (skip >= 1)
        {
            res.d[0] = lhs.d[1] >> mod_shift;
        }
    }
    if constexpr (w == 3)
    {
        if (skip == 0)
        {
            res.d[0] = lhs.d[0] >> mod_shift;
            res.d[0] |= lhs.d[1] << inv_shift;
            res.d[1] = lhs.d[1] >> mod_shift;
            res.d[1] |= lhs.d[2] << inv_shift;
            res.d[2] = lhs.d[2] >> mod_shift;
        }
        if (skip == 1)
        {
            res.d[0] = lhs.d[1] >> mod_shift;
            res.d[0] |= lhs.d[2] << inv_shift;
            res.d[1] = lhs.d[2] >> mod_shift;
        }
        else // if (skip == 2)
        {
            res.d[0] = lhs.d[2] >> mod_shift;
        }
    }
    if constexpr (w == 4)
    {
        if (skip == 0)
        {
            res.d[0] = lhs.d[0] >> mod_shift;
            res.d[0] |= lhs.d[1] << inv_shift;
            res.d[1] = lhs.d[1] >> mod_shift;
            res.d[1] |= lhs.d[2] << inv_shift;
            res.d[2] = lhs.d[2] >> mod_shift;
            res.d[2] |= lhs.d[3] << inv_shift;
            res.d[3] = lhs.d[3] >> mod_shift;
        }
        if (skip == 1)
        {
            res.d[0] = lhs.d[1] >> mod_shift;
            res.d[0] |= lhs.d[2] << inv_shift;
            res.d[1] = lhs.d[2] >> mod_shift;
            res.d[1] |= lhs.d[3] << inv_shift;
            res.d[2] = lhs.d[3] >> mod_shift;
        }
        else if (skip == 2)
        {
            res.d[0] = lhs.d[2] >> mod_shift;
            res.d[0] |= lhs.d[3] << inv_shift;
            res.d[1] = lhs.d[3] >> mod_shift;
        }
        else // if (skip == 3)
        {
            res.d[0] = lhs.d[3] >> mod_shift;
        }
    }
#endif
    return res;
}

template <int w>
constexpr fixed_uint<w> operator<<(fixed_uint<w> const& lhs, int shift) noexcept
{
    TG_ASSERT(0 <= shift && shift < (w * 64)); // similar to default types this would be UB

    if (shift == 0)
        return lhs;

    const int mod_shift = shift % 64;     // avoid ub
    const int skip = shift / 64;          // count skipped words
    const int inv_shift = 64 - mod_shift; // inverted shift

    fixed_uint<w> res;

    if (mod_shift == 0)
    {
        if (skip == 1)
        {
            if constexpr (w > 1)
                res.d[1] = lhs.d[0];
            if constexpr (w > 2)
                res.d[2] = lhs.d[1];
            if constexpr (w > 3)
                res.d[3] = lhs.d[2];
        }
        if (skip == 2)
        {
            if constexpr (w > 2)
                res.d[2] = lhs.d[0];
            if constexpr (w > 3)
                res.d[3] = lhs.d[1];
        }
        if (skip == 3)
        {
            if constexpr (w > 3)
                res.d[3] = lhs.d[0];
        }
        return res;
    }

#if 1
    if (skip == 0)
    {
        res.d[0] = lhs.d[0] << mod_shift;
        if constexpr (w > 1)
        {
            res.d[1] = lhs.d[1] << mod_shift;
            res.d[1] |= lhs.d[0] >> inv_shift;
        }
        if constexpr (w > 2)
        {
            res.d[2] = lhs.d[2] << mod_shift;
            res.d[2] |= lhs.d[1] >> inv_shift;
        }
        if constexpr (w > 3)
        {
            res.d[3] = lhs.d[3] << mod_shift;
            res.d[3] |= lhs.d[2] >> inv_shift;
        }
    }
    else if (skip == 1)
    {
        if constexpr (w > 1)
        {
            res.d[1] = lhs.d[0] << mod_shift;
        }
        if constexpr (w > 2)
        {
            res.d[2] = lhs.d[1] << mod_shift;
            res.d[2] |= lhs.d[0] >> inv_shift;
        }
        if constexpr (w > 3)
        {
            res.d[3] = lhs.d[2] << mod_shift;
            res.d[3] |= lhs.d[1] >> inv_shift;
        }
    }
    else if (skip == 2)
    {
        if constexpr (w > 2)
        {
            res.d[2] = lhs.d[0] << mod_shift;
        }
        if constexpr (w > 3)
        {
            res.d[3] = lhs.d[1] << mod_shift;
            res.d[3] |= lhs.d[0] >> inv_shift;
        }
    }
    else if (skip == 3)
    {
        if constexpr (w > 3)
        {
            res.d[3] = lhs.d[0] << mod_shift;
        }
    }
#else

    if constexpr (w == 2)
    {
        if (skip == 0)
        {
            res.d[1] = lhs.d[1] << mod_shift;
            res.d[1] |= lhs.d[0] >> inv_shift;
            res.d[0] = lhs.d[0] << mod_shift;
        }
        else // if (skip >= 1)
        {
            res.d[1] = lhs.d[0] << mod_shift;
        }
    }
    if constexpr (w == 3)
    {
        if (skip == 0)
        {
            res.d[2] = lhs.d[2] << mod_shift;
            res.d[2] |= lhs.d[1] >> inv_shift;
            res.d[1] = lhs.d[1] << mod_shift;
            res.d[1] |= lhs.d[0] >> inv_shift;
            res.d[0] = lhs.d[0] << mod_shift;
        }
        if (skip == 1)
        {
            res.d[2] = lhs.d[1] << mod_shift;
            res.d[2] |= lhs.d[0] >> inv_shift;
            res.d[1] = lhs.d[0] << mod_shift;
        }
        else // if (skip == 2)
        {
            res.d[2] = lhs.d[0] << mod_shift;
        }
    }
    if constexpr (w == 4)
    {
        if (skip == 0)
        {
            res.d[3] = lhs.d[3] << mod_shift;
            res.d[3] |= lhs.d[2] >> inv_shift;
            res.d[2] = lhs.d[2] << mod_shift;
            res.d[2] |= lhs.d[1] >> inv_shift;
            res.d[1] = lhs.d[1] << mod_shift;
            res.d[1] |= lhs.d[0] >> inv_shift;
            res.d[0] = lhs.d[0] << mod_shift;
        }
        if (skip == 1)
        {
            res.d[3] = lhs.d[2] << mod_shift;
            res.d[3] |= lhs.d[1] >> inv_shift;
            res.d[2] = lhs.d[1] << mod_shift;
            res.d[2] |= lhs.d[0] >> inv_shift;
            res.d[1] = lhs.d[0] << mod_shift;
        }
        else if (skip == 2)
        {
            res.d[3] = lhs.d[1] << mod_shift;
            res.d[3] |= lhs.d[0] >> inv_shift;
            res.d[2] = lhs.d[0] << mod_shift;
        }
        else // if (count == 3)
        {
            res.d[3] = lhs.d[0] << mod_shift;
        }
    }
#endif
    return res;
}

// assignment shift

template <int w>
constexpr fixed_uint<w>& operator>>=(fixed_uint<w>& lhs, int shift) noexcept
{
    lhs = lhs >> shift;
    return lhs;
}

template <int w>
constexpr fixed_uint<w>& operator<<=(fixed_uint<w>& lhs, int shift) noexcept
{
    lhs = lhs << shift;
    return lhs;
}

// utility

template <int w>
constexpr u64 leading_zeros_count(fixed_uint<w> const& v)
{
    u64 zeros = 0;
    if constexpr (w > 3)
    {
        zeros += _lzcnt_u64(v.d[3]);
        if (zeros < 64)
            return zeros;
    }
    if constexpr (w > 2)
    {
        zeros += _lzcnt_u64(v.d[2]);
        if (zeros < ((w - 2) * 64))
            return zeros;
    }
    if constexpr (w > 1)
    {
        zeros += _lzcnt_u64(v.d[1]);
        if (zeros < ((w - 1) * 64))
            return zeros;
    }
    return zeros + _lzcnt_u64(v.d[0]);
}

template <int w>
constexpr u64 leading_ones_count(fixed_uint<w> const& v)
{
    u64 ones = 0;
    if constexpr (w > 3)
    {
        ones += _lzcnt_u64(~v.d[3]);
        if (ones < 64)
            return ones;
    }
    if constexpr (w > 2)
    {
        ones += _lzcnt_u64(~v.d[2]);
        if (ones < ((w - 2) * 64))
            return ones;
    }
    if constexpr (w > 1)
    {
        ones += _lzcnt_u64(~v.d[1]);
        if (ones < ((w - 1) * 64))
            return ones;
    }
    return ones + _lzcnt_u64(~v.d[0]);
}

template <int w>
constexpr u64 trailing_zeros_count(fixed_uint<w> const& v)
{
    u64 zeros = 0;
    zeros += _tzcnt_u64(v.d[0]);
    if constexpr (w > 1)
    {
        if (zeros < 64)
            return zeros;
        zeros += _tzcnt_u64(v.d[1]);
    }
    if constexpr (w > 2)
    {
        if (zeros < (2 * 64))
            return zeros;
        zeros += _tzcnt_u64(v.d[2]);
    }
    if constexpr (w > 3)
    {
        if (zeros < (3 * 64))
            return zeros;
        zeros += _tzcnt_u64(v.d[3]);
    }
    return zeros;
}

template <int w>
constexpr u64 trailing_ones_count(fixed_uint<w> const& v)
{
    u64 ones = 0;
    ones += _tzcnt_u64(~v.d[0]);
    if constexpr (w > 1)
    {
        if (ones < 64)
            return ones;
        ones += _tzcnt_u64(~v.d[1]);
    }
    if constexpr (w > 2)
    {
        if (ones < (2 * 64))
            return ones;
        ones += _tzcnt_u64(~v.d[2]);
    }
    if constexpr (w > 3)
    {
        if (ones < (3 * 64))
            return ones;
        ones += _tzcnt_u64(~v.d[3]);
    }
    return ones;
}

template <int w>
constexpr bool is_zero(fixed_uint<w> const& v)
{
    if (v.d[0] != 0)
        return false;
    if constexpr (w > 1)
        if (v.d[1] != 0)
            return false;
    if constexpr (w > 2)
        if (v.d[2] != 0)
            return false;
    if constexpr (w > 3)
        if (v.d[3] != 0)
            return false;
    return true;
}

template <int w>
[[nodiscard]] fixed_uint<w> gcd(u64 a, fixed_uint<w> const& b)
{
    return gcd(fixed_uint<1>(a), b);
}

template <int w>
[[nodiscard]] fixed_uint<w> gcd(fixed_uint<w> const& a, u64 b)
{
    return gcd(a, fixed_uint<1>(b));
}

template <int w0, int w1>
[[nodiscard]] fixed_uint<max(w0, w1)> gcd(fixed_uint<w0> const& a, fixed_uint<w1> const& b)
{
    // todo: I think we can return fixed_uint<min(w0, w1)> (note the min instead of the max)

    // https://lemire.me/blog/2013/12/26/fastest-way-to-compute-the-greatest-common-divisor/
    fixed_uint<max(w0, w1)> u = a;
    fixed_uint<max(w0, w1)> v = b;

    int shift;
    if (is_zero(u))
        return v;
    if (is_zero(v))
        return u;
    shift = int(trailing_zeros_count(u | v));
    u >>= int(trailing_zeros_count(u));
    do
    {
        v >>= int(trailing_zeros_count(v));
        if (u > v)
        {
            fixed_uint<max(w0, w1)> t = v;
            v = u;
            u = t;
        }
        v = v - u;
    } while (!is_zero(v));
    return u << shift;
}

} // namespace tg
