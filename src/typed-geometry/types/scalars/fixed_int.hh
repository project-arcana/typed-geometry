#pragma once

#include <typed-geometry/detail/utility.hh>
#include <typed-geometry/types/scalars/default.hh>

namespace tg
{
// fwd
template <int words>
struct fixed_uint;
template <int words>
struct fixed_int;
using i128 = fixed_int<2>;
using i192 = fixed_int<3>;
using i256 = fixed_int<4>;

template <int words>
struct fixed_int
{
    static_assert(words <= 4, "only up to 256 bit supported");

    u64 d[words] = {};

    // ctors
    constexpr fixed_int() = default;
    constexpr fixed_int(fixed_int const& rhs) = default;
    constexpr fixed_int(fixed_int&& rhs) noexcept = default;
    constexpr fixed_int& operator=(fixed_int const& rhs) = default;
    constexpr fixed_int& operator=(fixed_int&& rhs) noexcept = default;

    constexpr fixed_int(i64 rhs)
    {
        d[0] = u64(rhs);
        if constexpr (words > 1)
            d[1] = rhs < 0 ? u64(-1) : 0;
        if constexpr (words > 2)
            d[2] = rhs < 0 ? u64(-1) : 0;
        if constexpr (words > 3)
            d[3] = rhs < 0 ? u64(-1) : 0;
    }

    template <int rhs_words>
    explicit constexpr fixed_int(fixed_uint<rhs_words> const& rhs);

    // explicit down cast
    template <int rhs_words, class = enable_if<(rhs_words > words)>, class = void>
    explicit constexpr fixed_int(fixed_int<rhs_words> rhs);

    // implicit promotion
    template <int rhs_words, class = enable_if<(rhs_words < words)>>
    constexpr fixed_int(fixed_int<rhs_words> rhs);

    // explicit conversion
    explicit constexpr operator i64() const { return i64(d[0]); }
    explicit constexpr operator float() const;
    explicit constexpr operator double() const;
};

static_assert(sizeof(i128) == 128 / 8, "i128 is not 128 bit!");
static_assert(sizeof(i192) == 192 / 8, "i192 is not 192 bit!");
static_assert(sizeof(i256) == 256 / 8, "i256 is not 256 bit!");

template <int w0, int w1>
constexpr bool operator==(fixed_int<w0> const& lhs, fixed_int<w1> const& rhs) noexcept
{
    constexpr int w = w0 > w1 ? w0 : w1;
    fixed_int<w> l = lhs;
    fixed_int<w> r = rhs;

    bool eq = true;

    eq &= lhs.d[0] == rhs.d[0];
    if constexpr (w > 1)
        eq &= l.d[1] == r.d[1];
    if constexpr (w > 2)
        eq &= l.d[2] == r.d[2];
    if constexpr (w > 3)
        eq &= l.d[3] == r.d[3];

    return eq;
}

template <int w>
constexpr bool operator==(i64 lhs, fixed_int<w> const& rhs) noexcept
{
    bool eq = true;
    eq &= u64(lhs) == rhs.d[0];

    if (lhs < 0)
    {
        if constexpr (w > 1)
            eq &= rhs.d[1] == u64(-1);
        if constexpr (w > 2)
            eq &= rhs.d[2] == u64(-1);
        if constexpr (w > 3)
            eq &= rhs.d[3] == u64(-1);
    }
    else
    {
        if constexpr (w > 1)
            eq &= rhs.d[1] == 0;
        if constexpr (w > 2)
            eq &= rhs.d[2] == 0;
        if constexpr (w > 3)
            eq &= rhs.d[3] == 0;
    }

    return eq;
}

template <int w>
constexpr bool operator==(fixed_int<w> const& lhs, i64 rhs) noexcept
{
    return rhs == lhs;
}

template <int w0, int w1>
constexpr bool operator!=(fixed_int<w0> const& lhs, fixed_int<w1> const& rhs) noexcept
{
    constexpr int w = w0 > w1 ? w0 : w1;
    fixed_int<w> l = lhs;
    fixed_int<w> r = rhs;
    bool neq = false;
    neq |= l.d[0] != r.d[0];
    if constexpr (w > 1)
        neq |= l.d[1] != r.d[1];
    if constexpr (w > 2)
        neq |= l.d[2] != r.d[2];
    if constexpr (w > 3)
        neq |= l.d[3] != r.d[3];
    return neq;
}

template <int w>
constexpr bool operator!=(i64 lhs, fixed_int<w> const& rhs) noexcept
{
    return fixed_int<w>(lhs) != rhs;
}

template <int w>
constexpr bool operator!=(fixed_int<w> const& lhs, i64 rhs) noexcept
{
    return fixed_int<w>(rhs) != lhs;
}

template <class I, int w>
constexpr void introspect(I&& i, fixed_int<w>& v)
{
    i(v.d[0], "w0");
    if constexpr (w >= 2)
        i(v.d[1], "w1");
    if constexpr (w >= 3)
        i(v.d[2], "w2");
    if constexpr (w >= 4)
        i(v.d[3], "w3");
    static_assert(w <= 4, "not supported");
}
}
