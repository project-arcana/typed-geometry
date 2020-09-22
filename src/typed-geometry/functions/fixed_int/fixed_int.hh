#pragma once

#ifdef _MSC_VER
#include <intrin.h>
#else
#include <x86intrin.h>
#endif

#include <typed-geometry/functions/basic/minmax.hh>
#include <typed-geometry/types/scalars/fixed_int.hh>

// todo: division and modulo are essentially the same code, put into detail maybe
// todo: gcc does not seem to produce optimal code gen

namespace tg
{
// comparison

template <int w0, int w1>
constexpr bool operator<(fixed_int<w0> const& lhs, fixed_int<w1> const& rhs) noexcept;
template <int w0, int w1>
constexpr bool operator>(fixed_int<w0> const& lhs, fixed_int<w1> const& rhs) noexcept;
template <int w0, int w1>
constexpr bool operator<=(fixed_int<w0> const& lhs, fixed_int<w1> const& rhs) noexcept;
template <int w0, int w1>
constexpr bool operator>=(fixed_int<w0> const& lhs, fixed_int<w1> const& rhs) noexcept;

template <int w>
constexpr bool operator<(i64 lhs, fixed_int<w> const& rhs) noexcept;
template <int w>
constexpr bool operator>(i64 lhs, fixed_int<w> const& rhs) noexcept;
template <int w>
constexpr bool operator<=(i64 lhs, fixed_int<w> const& rhs) noexcept;
template <int w>
constexpr bool operator>=(i64 lhs, fixed_int<w> const& rhs) noexcept;

template <int w>
constexpr bool operator<(fixed_int<w> const& lhs, i64 rhs) noexcept;
template <int w>
constexpr bool operator>(fixed_int<w> const& lhs, i64 rhs) noexcept;
template <int w>
constexpr bool operator<=(fixed_int<w> const& lhs, i64 rhs) noexcept;
template <int w>
constexpr bool operator>=(fixed_int<w> const& lhs, i64 rhs) noexcept;

// standard arithmetics

template <int w>
constexpr fixed_int<w> operator-(fixed_int<w> const& lhs) noexcept;
template <int w0, int w1>
constexpr fixed_int<max(w0, w1)> operator+(fixed_int<w0> const& lhs, fixed_int<w1> const& rhs) noexcept;
template <int w0, int w1>
constexpr fixed_int<max(w0, w1)> operator-(fixed_int<w0> const& lhs, fixed_int<w1> const& rhs) noexcept;
template <int w0, int w1>
constexpr fixed_int<max(w0, w1)> operator*(fixed_int<w0> const& lhs, fixed_int<w1> const& rhs) noexcept;
template <int w0, int w1>
constexpr fixed_int<max(w0, w1)> operator/(fixed_int<w0> const& lhs, fixed_int<w1> const& rhs) noexcept;
template <int w0, int w1>
constexpr fixed_int<max(w0, w1)> operator%(fixed_int<w0> const& lhs, fixed_int<w1> const& rhs) noexcept;

template <int w>
constexpr fixed_int<w> operator+(i64 lhs, fixed_int<w> const& rhs) noexcept;
template <int w>
constexpr fixed_int<w> operator-(i64 lhs, fixed_int<w> const& rhs) noexcept;
template <int w>
constexpr fixed_int<w> operator*(i64 lhs, fixed_int<w> const& rhs) noexcept;
template <int w>
constexpr fixed_int<w> operator/(i64 lhs, fixed_int<w> const& rhs) noexcept;
template <int w>
constexpr fixed_int<w> operator%(i64 lhs, fixed_int<w> const& rhs) noexcept;

template <int w>
constexpr fixed_int<w> operator+(fixed_int<w> const& lhs, i64 rhs) noexcept;
template <int w>
constexpr fixed_int<w> operator-(fixed_int<w> const& lhs, i64 rhs) noexcept;
template <int w>
constexpr fixed_int<w> operator*(fixed_int<w> const& lhs, i64 rhs) noexcept;
template <int w>
constexpr fixed_int<w> operator/(fixed_int<w> const& lhs, i64 rhs) noexcept;
template <int w>
constexpr fixed_int<w> operator%(fixed_int<w> const& lhs, i64 rhs) noexcept;

// assignment arithmetics

template <int w0, int w1>
constexpr fixed_int<w0>& operator+=(fixed_int<w0>& lhs, fixed_int<w1> const& rhs) noexcept;
template <int w0, int w1>
constexpr fixed_int<w0>& operator-=(fixed_int<w0>& lhs, fixed_int<w1> const& rhs) noexcept;
template <int w0, int w1>
constexpr fixed_int<w0>& operator*=(fixed_int<w0>& lhs, fixed_int<w1> const& rhs) noexcept;
template <int w0, int w1>
constexpr fixed_int<w0>& operator/=(fixed_int<w0>& lhs, fixed_int<w1> const& rhs) noexcept;
template <int w0, int w1>
constexpr fixed_int<w0>& operator%=(fixed_int<w0>& lhs, fixed_int<w1> const& rhs) noexcept;

template <int w>
constexpr fixed_int<w>& operator+=(fixed_int<w>& lhs, i64 rhs) noexcept;
template <int w>
constexpr fixed_int<w>& operator-=(fixed_int<w>& lhs, i64 rhs) noexcept;
template <int w>
constexpr fixed_int<w>& operator*=(fixed_int<w>& lhs, i64 rhs) noexcept;
template <int w>
constexpr fixed_int<w>& operator/=(fixed_int<w>& lhs, i64 rhs) noexcept;
template <int w>
constexpr fixed_int<w>& operator%=(fixed_int<w>& lhs, i64 rhs) noexcept;

// increment / decrement

template <int w>
constexpr fixed_int<w>& operator++(fixed_int<w>& lhs) noexcept;
template <int w>
constexpr fixed_int<w> operator++(fixed_int<w>& lhs, int) noexcept;
template <int w>
constexpr fixed_int<w>& operator--(fixed_int<w>& lhs) noexcept;
template <int w>
constexpr fixed_int<w> operator--(fixed_int<w>& lhs, int) noexcept;

// bitwise logic

template <int w0, int w1>
constexpr fixed_int<max(w0, w1)> operator|(fixed_int<w0> const& lhs, fixed_int<w1> const& rhs) noexcept;
template <int w0, int w1>
constexpr fixed_int<max(w0, w1)> operator&(fixed_int<w0> const& lhs, fixed_int<w1> const& rhs) noexcept;
template <int w0, int w1>
constexpr fixed_int<max(w0, w1)> operator^(fixed_int<w0> const& lhs, fixed_int<w1> const& rhs) noexcept;
template <int w>
constexpr fixed_int<w> operator~(fixed_int<w> const& lhs) noexcept;

template <int w>
constexpr fixed_int<w> operator|(i64 lhs, fixed_int<w> const& rhs) noexcept;
template <int w>
constexpr fixed_int<w> operator&(i64 lhs, fixed_int<w> const& rhs) noexcept;
template <int w>
constexpr fixed_int<w> operator^(i64 lhs, fixed_int<w> const& rhs) noexcept;

template <int w>
constexpr fixed_int<w> operator|(fixed_int<w> const& lhs, i64 rhs) noexcept;
template <int w>
constexpr fixed_int<w> operator&(fixed_int<w> const& lhs, i64 rhs) noexcept;
template <int w>
constexpr fixed_int<w> operator^(fixed_int<w> const& lhs, i64 rhs) noexcept;

// assignment bit ops

template <int w0, int w1>
constexpr fixed_int<w0>& operator|=(fixed_int<w0>& lhs, fixed_int<w1> const& rhs) noexcept;
template <int w0, int w1>
constexpr fixed_int<w0>& operator&=(fixed_int<w0>& lhs, fixed_int<w1> const& rhs) noexcept;
template <int w0, int w1>
constexpr fixed_int<w0>& operator^=(fixed_int<w0>& lhs, fixed_int<w1> const& rhs) noexcept;

template <int w>
constexpr fixed_int<w>& operator|=(fixed_int<w>& lhs, i64 rhs) noexcept;
template <int w>
constexpr fixed_int<w>& operator&=(fixed_int<w>& lhs, i64 rhs) noexcept;
template <int w>
constexpr fixed_int<w>& operator^=(fixed_int<w>& lhs, i64 rhs) noexcept;

template <int w>
constexpr fixed_int<w>& operator|=(i64& lhs, fixed_int<w> const& rhs) noexcept;
template <int w>
constexpr fixed_int<w>& operator&=(i64& lhs, fixed_int<w> const& rhs) noexcept;
template <int w>
constexpr fixed_int<w>& operator^=(i64& lhs, fixed_int<w> const& rhs) noexcept;

// shift

template <int w>
constexpr fixed_int<w> operator>>(fixed_int<w> const& lhs, int shift) noexcept;
template <int w>
constexpr fixed_int<w> operator<<(fixed_int<w> const& lhs, int shift) noexcept;

// assignment shift

template <int w>
constexpr fixed_int<w>& operator>>=(fixed_int<w>& lhs, int shift) noexcept;
template <int w>
constexpr fixed_int<w>& operator<<=(fixed_int<w>& lhs, int shift) noexcept;

// muliplication specializations

namespace detail
{
template <int w_res, class T0, class T1>
fixed_int<w_res> imul(T0 lhs, T1 rhs);
}

// utility

template <int w>
[[nodiscard]] constexpr u64 leading_zeros_count(fixed_int<w> const& v);

template <int w>
[[nodiscard]] constexpr u64 leading_ones_count(fixed_int<w> const& v);

template <int w>
[[nodiscard]] constexpr bool is_zero(fixed_int<w> const& v);

template <int w>
[[nodiscard]] constexpr int sign(fixed_int<w> const& v);


//#############################################################################
//#                             implemenation                                 #
//#############################################################################

template <int words>
template <int rhs_words, class, class>
constexpr fixed_int<words>::fixed_int(fixed_int<rhs_words> rhs)
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
constexpr fixed_int<words>::fixed_int(fixed_int<rhs_words> rhs)
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

    if (rhs < 0)
    {
        d[rhs_words] = u64(-1);
        if constexpr (words - rhs_words > 1)
            d[rhs_words + 1] = u64(-1);
        if constexpr (words - rhs_words > 2)
            d[rhs_words + 2] = u64(-1);
    }
}

template <int words>
constexpr fixed_int<words>::operator float() const
{
    auto cpy = *this;
    float sign = 1;
    if (cpy < 0)
    {
        sign = -1;
        cpy = -cpy;
    }

    float res = float(cpy.d[0]);
    if constexpr (words > 1)
        res += float(cpy.d[1]) * 0x1p64f;
    if constexpr (words > 2) // if not 0 this is going to be inf anyways, but inf * 0 = nan, which is not what is desired here
        res += cpy.d[2] == 0 ? 0 : float(cpy.d[2]) * float(0x1p128);
    if constexpr (words > 3) // if not 0 this is going to be inf anyways, but inf * 0 = nan, which is not what is desired here
        res += cpy.d[3] == 0 ? 0 : float(cpy.d[3]) * float(0x1p128);
    res *= sign;
    return res;
}

template <int words>
constexpr fixed_int<words>::operator double() const
{
    auto cpy = *this;
    double sign = 1;
    if (cpy < 0)
    {
        sign = -1;
        cpy = -cpy;
    }

    double res = double(cpy.d[0]);
    if constexpr (words > 1)
        res += double(cpy.d[1]) * 0x1p64;
    if constexpr (words > 2)
        res += double(cpy.d[2]) * 0x1p128;
    if constexpr (words > 3)
        res += double(cpy.d[3]) * 0x1p192;
    res *= sign;
    return res;
}

//
// comparison
//

namespace detail
{
template <int w>
constexpr bool less_than_zero(fixed_int<w> const& v)
{
    return i64(v.d[w - 1]) < 0;
}
}

template <int w0, int w1>
constexpr bool operator<(fixed_int<w0> const& lhs, fixed_int<w1> const& rhs) noexcept
{
    return detail::less_than_zero(lhs - rhs);
}

template <int w>
constexpr bool operator<(i64 lhs, fixed_int<w> const& rhs) noexcept
{
    return fixed_int<w>(lhs) < rhs;
}

template <int w>
constexpr bool operator<(fixed_int<w> const& lhs, i64 rhs) noexcept
{
    return lhs < fixed_int<w>(rhs);
}

template <int w0, int w1>
constexpr bool operator>(fixed_int<w0> const& lhs, fixed_int<w1> const& rhs) noexcept
{
    return rhs < lhs;
}

template <int w>
constexpr bool operator>(i64 lhs, fixed_int<w> const& rhs) noexcept
{
    return rhs < lhs;
}

template <int w>
constexpr bool operator>(fixed_int<w> const& lhs, i64 rhs) noexcept
{
    return rhs < lhs;
}

template <int w0, int w1>
constexpr bool operator<=(fixed_int<w0> const& lhs, fixed_int<w1> const& rhs) noexcept
{
    return !(lhs > rhs);
}

template <int w>
constexpr bool operator<=(i64 lhs, fixed_int<w> const& rhs) noexcept
{
    return fixed_int<w>(lhs) < rhs;
}

template <int w>
constexpr bool operator<=(fixed_int<w> const& lhs, i64 rhs) noexcept
{
    return lhs < fixed_int<w>(rhs);
}

template <int w0, int w1>
constexpr bool operator>=(fixed_int<w0> const& lhs, fixed_int<w1> const& rhs) noexcept
{
    return rhs <= lhs;
}

template <int w>
constexpr bool operator>=(i64 lhs, fixed_int<w> const& rhs) noexcept
{
    return rhs <= lhs;
}

template <int w>
constexpr bool operator>=(fixed_int<w> const& lhs, i64 rhs) noexcept
{
    return rhs <= lhs;
}

//
//  standard arithmetics
//

template <int w>
constexpr fixed_int<w> operator-(fixed_int<w> const& lhs) noexcept
{
    fixed_int<w> res;
    [[maybe_unused]] u64 c0 = 0;
    [[maybe_unused]] u64 c1 = 0;
    [[maybe_unused]] u64 c2 = 0;
    res.d[0] = ~lhs.d[0] + 1;
    if constexpr (w > 1)
    {
        c0 = res.d[0] == 0;
        res.d[1] = ~lhs.d[1] + c0;
    }
    if constexpr (w > 2)
    {
        c1 = (res.d[1] == 0) & c0;
        res.d[2] = ~lhs.d[2] + c1;
    }
    if constexpr (w > 3)
    {
        c2 = (res.d[2] == 0) & c1;
        res.d[3] = ~lhs.d[3] + c2;
    }
    return res;
}

template <int w0, int w1>
constexpr fixed_int<max(w0, w1)> operator+(fixed_int<w0> const& lhs, fixed_int<w1> const& rhs) noexcept
{
    constexpr int w_out = max(w0, w1);
    fixed_int<w_out> res;
    fixed_int<w_out> l = lhs;
    fixed_int<w_out> r = rhs;
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
constexpr fixed_int<w> operator+(i64 lhs, fixed_int<w> const& rhs) noexcept
{
    return fixed_int<w>(lhs) + rhs;
}

template <int w>
constexpr fixed_int<w> operator+(fixed_int<w> const& lhs, i64 rhs) noexcept
{
    return lhs + fixed_int<w>(rhs);
}

template <int w0, int w1>
constexpr fixed_int<max(w0, w1)> operator-(fixed_int<w0> const& lhs, fixed_int<w1> const& rhs) noexcept
{
    constexpr int w_out = max(w0, w1);
    fixed_int<w_out> res;
    fixed_int<w_out> l = lhs;
    fixed_int<w_out> r = rhs;
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
constexpr fixed_int<w> operator-(i64 lhs, fixed_int<w> const& rhs) noexcept
{
    return fixed_int<w>(lhs) - rhs;
}

template <int w>
constexpr fixed_int<w> operator-(fixed_int<w> const& lhs, i64 rhs) noexcept
{
    return lhs - fixed_int<w>(rhs);
}

template <int w0, int w1>
constexpr fixed_int<max(w0, w1)> operator*(fixed_int<w0> const& lhs, fixed_int<w1> const& rhs) noexcept
{
    constexpr int w_out = max(w0, w1);
    fixed_int<w_out> l = lhs;
    fixed_int<w_out> r = rhs;
    return detail::imul<w_out>(l, r);
}

template <int w>
constexpr fixed_int<w> operator*(i64 lhs, fixed_int<w> const& rhs) noexcept
{
    return detail::imul<w>(lhs, rhs);
}

template <int w>
constexpr fixed_int<w> operator*(fixed_int<w> const& lhs, i64 rhs) noexcept
{
    return detail::imul<w>(lhs, rhs);
}

template <int w0, int w1>
constexpr fixed_int<max(w0, w1)> operator/(fixed_int<w0> const& lhs, fixed_int<w1> const& rhs) noexcept
{
    TG_ASSERT(rhs != 0 && "division by zero!");

    bool is_neg_lhs = detail::less_than_zero(lhs);
    bool is_neg_rhs = detail::less_than_zero(rhs);
    bool is_neg_res = is_neg_lhs ^ is_neg_rhs;

    constexpr int w_out = max(w0, w1);
    fixed_int<w_out> l = is_neg_lhs ? -lhs : lhs;
    fixed_int<w_out> r = is_neg_rhs ? -rhs : rhs;

    /// see https://en.wikipedia.org/wiki/Division_algorithm#Integer_division_(unsigned)_with_remainder
    fixed_int<w_out> quotient = 0;
    fixed_int<w_out> remainder = 0;
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
    return is_neg_res ? -quotient : quotient;
}

template <int w>
constexpr fixed_int<w> operator/(i64 lhs, fixed_int<w> const& rhs) noexcept
{
    return fixed_int<1>(lhs) / rhs;
}

template <int w>
constexpr fixed_int<w> operator/(fixed_int<w> const& lhs, i64 rhs) noexcept
{
    return lhs / fixed_int<1>(rhs);
}

template <int w0, int w1>
constexpr fixed_int<max(w0, w1)> operator%(fixed_int<w0> const& lhs, fixed_int<w1> const& rhs) noexcept
{
    TG_ASSERT(rhs != 0 && "division by zero!");

    // this is implemented the same way cpp11 defines standard integer modulo
    // regarding the sign of the output:
    // the sign of the remainder is set so that (a/b)*a + a % b == a

    bool is_neg_lhs = detail::less_than_zero(lhs);
    bool is_neg_rhs = detail::less_than_zero(rhs);

    constexpr int w_out = max(w0, w1);
    fixed_int<w_out> l = is_neg_lhs ? -lhs : lhs;
    fixed_int<w_out> r = is_neg_rhs ? -rhs : rhs;

    /// see https://en.wikipedia.org/wiki/Division_algorithm#Integer_division_(unsigned)_with_remainder
    fixed_int<w_out> quotient = 0;
    fixed_int<w_out> remainder = 0;
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
    return is_neg_lhs ? -remainder : remainder;
}

template <int w>
constexpr fixed_int<w> operator%(i64 lhs, fixed_int<w> const& rhs) noexcept
{
    return fixed_int<1>(lhs) % rhs;
}

template <int w>
constexpr fixed_int<w> operator%(fixed_int<w> const& lhs, i64 rhs) noexcept
{
    return lhs % fixed_int<1>(rhs);
}

// assignment arithmetics

template <int w0, int w1>
constexpr fixed_int<w0>& operator+=(fixed_int<w0>& lhs, fixed_int<w1> const& rhs) noexcept
{
    lhs = lhs + rhs;
    return lhs;
}

template <int w>
constexpr fixed_int<w>& operator+=(fixed_int<w>& lhs, i64 rhs) noexcept
{
    lhs = lhs + rhs;
    return lhs;
}

template <int w0, int w1>
constexpr fixed_int<w0>& operator-=(fixed_int<w0>& lhs, fixed_int<w1> const& rhs) noexcept
{
    lhs = lhs - rhs;
    return lhs;
}

template <int w>
constexpr fixed_int<w>& operator-=(fixed_int<w>& lhs, i64 rhs) noexcept
{
    lhs = lhs - rhs;
    return lhs;
}

template <int w0, int w1>
constexpr fixed_int<w0>& operator*=(fixed_int<w0>& lhs, fixed_int<w1> const& rhs) noexcept
{
    lhs = lhs * rhs;
    return lhs;
}

template <int w>
constexpr fixed_int<w>& operator*=(fixed_int<w>& lhs, i64 rhs) noexcept
{
    lhs = lhs * rhs;
    return lhs;
}

template <int w0, int w1>
constexpr fixed_int<w0>& operator/=(fixed_int<w0>& lhs, fixed_int<w1> const& rhs) noexcept
{
    lhs = lhs / rhs;
    return lhs;
}

template <int w>
constexpr fixed_int<w>& operator/=(fixed_int<w>& lhs, i64 rhs) noexcept
{
    lhs = lhs / rhs;
    return lhs;
}

template <int w0, int w1>
constexpr fixed_int<w0>& operator%=(fixed_int<w0>& lhs, fixed_int<w1> const& rhs) noexcept
{
    lhs = lhs % rhs;
    return lhs;
}

template <int w>
constexpr fixed_int<w>& operator%=(fixed_int<w>& lhs, i64 rhs) noexcept
{
    lhs = lhs % rhs;
    return lhs;
}

// increment / decrement

template <int w>
constexpr fixed_int<w>& operator++(fixed_int<w>& lhs) noexcept
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
constexpr fixed_int<w> operator++(fixed_int<w>& lhs, int) noexcept
{
    fixed_int<w> cpy = lhs;
    ++lhs;
    return cpy;
}

template <int w>
constexpr fixed_int<w>& operator--(fixed_int<w>& lhs) noexcept
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
constexpr fixed_int<w> operator--(fixed_int<w>& lhs, int) noexcept
{
    fixed_int<w> cpy = lhs;
    --lhs;
    return cpy;
}

// bitwise logic

template <int w0, int w1>
constexpr fixed_int<max(w0, w1)> operator|(fixed_int<w0> const& lhs, fixed_int<w1> const& rhs) noexcept
{
    fixed_int<max(w0, w1)> res;
    fixed_int<max(w0, w1)> l = lhs;
    fixed_int<max(w0, w1)> r = rhs;

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
constexpr fixed_int<w> operator|(i64 lhs, fixed_int<w> const& rhs) noexcept
{
    fixed_int<w> res = rhs;
    res.d[0] |= u64(lhs);
    return res;
}

template <int w>
constexpr fixed_int<w> operator|(fixed_int<w> const& lhs, i64 rhs) noexcept
{
    fixed_int<w> res = lhs;
    res.d[0] |= u64(rhs);
    return res;
}

template <int w0, int w1>
constexpr fixed_int<max(w0, w1)> operator&(fixed_int<w0> const& lhs, fixed_int<w1> const& rhs) noexcept
{
    fixed_int<max(w0, w1)> res;
    fixed_int<max(w0, w1)> l = lhs;
    fixed_int<max(w0, w1)> r = rhs;

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
constexpr fixed_int<w> operator&(i64 lhs, fixed_int<w> const& rhs) noexcept
{
    fixed_int<w> res = rhs;
    res.d[0] &= u64(lhs);
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
constexpr fixed_int<w> operator&(fixed_int<w> const& lhs, i64 rhs) noexcept
{
    fixed_int<w> res = lhs;
    res.d[0] &= u64(rhs);
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
constexpr fixed_int<max(w0, w1)> operator^(fixed_int<w0> const& lhs, fixed_int<w1> const& rhs) noexcept
{
    fixed_int<max(w0, w1)> res;
    fixed_int<max(w0, w1)> l = lhs;
    fixed_int<max(w0, w1)> r = rhs;

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
constexpr fixed_int<w> operator^(i64 lhs, fixed_int<w> const& rhs) noexcept
{
    fixed_int<w> res = rhs;
    res.d[0] ^= u64(lhs);
    return res;
}

template <int w>
constexpr fixed_int<w> operator^(fixed_int<w> const& lhs, i64 rhs) noexcept
{
    fixed_int<w> res = lhs;
    res.d[0] ^= u64(rhs);
    return res;
}

template <int w>
constexpr fixed_int<w> operator~(fixed_int<w> const& lhs) noexcept
{
    fixed_int<w> res;
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
constexpr fixed_int<w0>& operator|=(fixed_int<w0>& lhs, fixed_int<w1> const& rhs) noexcept
{
    fixed_int<w0> r = fixed_int<w0>(rhs);

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
constexpr fixed_int<w>& operator|=(fixed_int<w>& lhs, i64 rhs) noexcept
{
    lhs.d[0] |= rhs;
    return lhs;
}

template <int w>
constexpr i64& operator|=(i64& lhs, fixed_int<w> const& rhs) noexcept
{
    lhs |= rhs.d[0];
    return lhs;
}

template <int w0, int w1>
constexpr fixed_int<w0>& operator&=(fixed_int<w0>& lhs, fixed_int<w1> const& rhs) noexcept
{
    fixed_int<w0> r = fixed_int<w0>(rhs);

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
constexpr fixed_int<w>& operator&=(fixed_int<w>& lhs, i64 rhs) noexcept
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
constexpr i64& operator&=(i64& lhs, fixed_int<w> const& rhs) noexcept
{
    lhs |= rhs.d[0];
    return lhs;
}

template <int w0, int w1>
constexpr fixed_int<w0>& operator^=(fixed_int<w0>& lhs, fixed_int<w1> const& rhs) noexcept
{
    fixed_int<w0> r = fixed_int<w0>(rhs);

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
constexpr fixed_int<w>& operator^=(fixed_int<w>& lhs, i64 rhs) noexcept
{
    lhs.d[0] ^= rhs;
    return lhs;
}

template <int w>
constexpr i64& operator^=(i64& lhs, fixed_int<w> const& rhs) noexcept
{
    lhs ^= rhs.d[0];
    return lhs;
}

// shift

template <int w>
constexpr fixed_int<w> operator>>(fixed_int<w> const& lhs, int shift) noexcept
{
    TG_ASSERT(0 <= shift && shift < (w * 64)); // similar to default types this would be UB

    if (shift == 0)
        return lhs;

    const int mod_shift = shift % 64;     // avoid ub
    const int skip = shift / 64;          // count skipped words
    const int inv_shift = 64 - mod_shift; // inverted shift

    fixed_int<w> res;

    if (mod_shift == 0)
    {
        if (skip == 1)
        {
            if (detail::less_than_zero(lhs))
                res.d[w - 1] = u64(-1);
            if constexpr (w > 1)
                res.d[0] = lhs.d[1];
            if constexpr (w > 2)
                res.d[1] = lhs.d[2];
            if constexpr (w > 3)
                res.d[2] = lhs.d[3];
        }
        if (skip == 2)
        {
            if (detail::less_than_zero(lhs))
                res.d[w - 1] = u64(-1);
            if constexpr (w > 2)
            {
                res.d[0] = lhs.d[2];
                if (detail::less_than_zero(lhs))
                    res.d[w - 2] = u64(-1);
            }
            if constexpr (w > 3)
                res.d[1] = lhs.d[3];
        }
        if (skip == 3)
        {
            if constexpr (w > 3)
            {
                res.d[0] = lhs.d[3];
                if (detail::less_than_zero(lhs))
                    res.d[w - 1] = u64(-1);
                if (detail::less_than_zero(lhs))
                    res.d[w - 2] = u64(-1);
                if (detail::less_than_zero(lhs))
                    res.d[w - 3] = u64(-1);
            }
        }
        return res;
    }
    if constexpr (w == 1)
    {
        res.d[0] = i64(lhs.d[0]) >> mod_shift;
    }
    if constexpr (w == 2)
    {
        if (skip == 0)
        {
            res.d[0] = lhs.d[0] >> mod_shift;
            res.d[0] |= lhs.d[1] << inv_shift;
            res.d[1] = u64(i64(lhs.d[1]) >> mod_shift);
        }
        else // if (skip >= 1)
        {
            res.d[0] = u64(i64(lhs.d[1]) >> mod_shift);
            res.d[1] = detail::less_than_zero(lhs) ? u64(-1) : 0;
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
            res.d[2] = u64(i64(lhs.d[2]) >> mod_shift);
        }
        else if (skip == 1)
        {
            res.d[0] = lhs.d[1] >> mod_shift;
            res.d[0] |= lhs.d[2] << inv_shift;
            res.d[1] = u64(i64(lhs.d[2]) >> mod_shift);
            res.d[2] = detail::less_than_zero(lhs) ? u64(-1) : 0;
        }
        else // if (skip == 2)
        {
            res.d[0] = u64(i64(lhs.d[2]) >> mod_shift);
            res.d[1] = detail::less_than_zero(lhs) ? u64(-1) : 0;
            res.d[2] = detail::less_than_zero(lhs) ? u64(-1) : 0;
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
            res.d[3] = u64(i64(lhs.d[3]) >> mod_shift);
        }
        else if (skip == 1)
        {
            res.d[0] = lhs.d[1] >> mod_shift;
            res.d[0] |= lhs.d[2] << inv_shift;
            res.d[1] = lhs.d[2] >> mod_shift;
            res.d[1] |= lhs.d[3] << inv_shift;
            res.d[2] = u64(i64(lhs.d[3]) >> mod_shift);
            res.d[3] = detail::less_than_zero(lhs) ? u64(-1) : 0;
        }
        else if (skip == 2)
        {
            res.d[0] = lhs.d[2] >> mod_shift;
            res.d[0] |= lhs.d[3] << inv_shift;
            res.d[1] = u64(i64(lhs.d[3]) >> mod_shift);
            res.d[2] = detail::less_than_zero(lhs) ? u64(-1) : 0;
            res.d[3] = detail::less_than_zero(lhs) ? u64(-1) : 0;
        }
        else // if (skip == 3)
        {
            res.d[0] = u64(i64(lhs.d[3]) >> mod_shift);
            res.d[1] = detail::less_than_zero(lhs) ? u64(-1) : 0;
            res.d[2] = detail::less_than_zero(lhs) ? u64(-1) : 0;
            res.d[3] = detail::less_than_zero(lhs) ? u64(-1) : 0;
        }
    }
    return res;
}

template <int w>
constexpr fixed_int<w> operator<<(fixed_int<w> const& lhs, int shift) noexcept
{
    TG_ASSERT(0 <= shift && shift < (w * 64)); // similar to default types this would be UB

    if (shift == 0)
        return lhs;

    const int mod_shift = shift % 64;     // avoid ub
    const int skip = shift / 64;          // count skipped words
    const int inv_shift = 64 - mod_shift; // inverted shift

    fixed_int<w> res;

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
    return res;
}

// assignment shift

template <int w>
constexpr fixed_int<w>& operator>>=(fixed_int<w>& lhs, int shift) noexcept
{
    lhs = lhs >> shift;
    return lhs;
}

template <int w>
constexpr fixed_int<w>& operator<<=(fixed_int<w>& lhs, int shift) noexcept
{
    lhs = lhs << shift;
    return lhs;
}

// utility

template <int w>
constexpr u64 leading_zeros_count(fixed_int<w> const& v)
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
constexpr u64 leading_ones_count(fixed_int<w> const& v)
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
constexpr bool is_zero(fixed_int<w> const& v)
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
constexpr int sign(fixed_int<w> const& v)
{
    int neg = i64(v.d[w - 1]) >> 63;
    return 1 - is_zero(v) + 2 * neg;
}

} // namespace tg
