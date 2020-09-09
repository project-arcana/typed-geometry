#pragma once

#include <typed-geometry/detail/utility.hh>
#include <typed-geometry/types/scalars/default.hh>

namespace tg
{
// fwd
template <int words>
struct fixed_int;
template <int words>
struct fixed_uint;
using u128 = fixed_uint<2>;
using u192 = fixed_uint<3>;
using u256 = fixed_uint<4>;

template <int words>
struct fixed_uint
{
    static_assert(words <= 4, "only up to 256 bit supported");

    u64 d[words] = {};

    // ctors
    constexpr fixed_uint() = default;
    constexpr fixed_uint(fixed_uint const& rhs) = default;
    constexpr fixed_uint(fixed_uint&& rhs) noexcept = default;
    constexpr fixed_uint& operator=(fixed_uint const& rhs) = default;
    constexpr fixed_uint& operator=(fixed_uint&& rhs) noexcept = default;

    constexpr fixed_uint(u64 rhs) { d[0] = rhs; }

    template <int rhs_words>
    explicit constexpr fixed_uint(fixed_int<rhs_words> const& rhs);

    // explicit down cast
    template <int rhs_words, class = enable_if<(rhs_words > words)>, class = void>
    explicit constexpr fixed_uint(fixed_uint<rhs_words> rhs);

    // implicit promotion
    template <int rhs_words, class = enable_if<(rhs_words < words)>>
    constexpr fixed_uint(fixed_uint<rhs_words> rhs);


    // explicit conversion
    explicit constexpr operator u64() const { return d[0]; }
    explicit constexpr operator float() const;
    explicit constexpr operator double() const;
};

static_assert(sizeof(u128) == 128 / 8, "u128 is not 128 bit!");
static_assert(sizeof(u192) == 192 / 8, "u192 is not 192 bit!");
static_assert(sizeof(u256) == 256 / 8, "u256 is not 256 bit!");

template <int w0, int w1>
constexpr bool operator==(fixed_uint<w0> const& lhs, fixed_uint<w1> const& rhs) noexcept
{
    bool eq = true;

    eq &= lhs.d[0] == rhs.d[0];
    if constexpr (w0 > 1 && w1 > 1)
        eq &= lhs.d[1] == rhs.d[1];
    if constexpr (w0 > 2 && w1 > 2)
        eq &= lhs.d[2] == rhs.d[2];
    if constexpr (w0 > 3 && w1 > 3)
        eq &= lhs.d[3] == rhs.d[3];

    if constexpr (w0 >= w1 + 1)
        eq &= lhs.d[w1 + 0] == 0;
    if constexpr (w0 >= w1 + 2)
        eq &= lhs.d[w1 + 1] == 0;
    if constexpr (w0 >= w1 + 3)
        eq &= lhs.d[w1 + 2] == 0;

    if constexpr (w0 + 1 <= w1)
        eq &= rhs.d[w0 + 0] == 0;
    if constexpr (w0 + 2 <= w1)
        eq &= rhs.d[w0 + 1] == 0;
    if constexpr (w0 + 3 <= w1)
        eq &= rhs.d[w0 + 2] == 0;

    return eq;
}

template <int w>
constexpr bool operator==(u64 lhs, fixed_uint<w> const& rhs) noexcept
{
    bool eq = true;
    eq &= lhs == rhs.d[0];
    if constexpr (w > 1)
        eq &= rhs.d[1] == 0;
    if constexpr (w > 2)
        eq &= rhs.d[2] == 0;
    if constexpr (w > 3)
        eq &= rhs.d[3] == 0;
    return eq;
}

template <int w>
constexpr bool operator==(fixed_uint<w> const& lhs, u64 rhs) noexcept
{
    return rhs == lhs;
}

template <int w0, int w1>
constexpr bool operator!=(fixed_uint<w0> const& lhs, fixed_uint<w1> const& rhs) noexcept
{
    bool neq = false;

    neq |= lhs.d[0] != rhs.d[0];
    if constexpr (w0 > 1 && w1 > 1)
        neq |= lhs.d[1] != rhs.d[1];
    if constexpr (w0 > 2 && w1 > 2)
        neq |= lhs.d[2] != rhs.d[2];
    if constexpr (w0 > 3 && w1 > 3)
        neq |= lhs.d[3] != rhs.d[3];

    if constexpr (w0 >= w1 + 1)
        neq |= lhs.d[w1 + 0] != 0;
    if constexpr (w0 >= w1 + 2)
        neq |= lhs.d[w1 + 1] != 0;
    if constexpr (w0 >= w1 + 3)
        neq |= lhs.d[w1 + 2] != 0;

    if constexpr (w0 + 1 <= w1)
        neq |= rhs.d[w0 + 0] != 0;
    if constexpr (w0 + 2 <= w1)
        neq |= rhs.d[w0 + 1] != 0;
    if constexpr (w0 + 3 <= w1)
        neq |= rhs.d[w0 + 2] != 0;

    return neq;
}

template <int w>
constexpr bool operator!=(u64 lhs, fixed_uint<w> const& rhs) noexcept
{
    bool neq = false;
    neq |= lhs != rhs.d[0];
    if constexpr (w > 1)
        neq |= rhs.d[1] != 0;
    if constexpr (w > 2)
        neq |= rhs.d[2] != 0;
    if constexpr (w > 3)
        neq |= rhs.d[3] != 0;
    return neq;
}

template <int w>
constexpr bool operator!=(fixed_uint<w> const& lhs, u64 rhs) noexcept
{
    return rhs != lhs;
}

template <class I, int w>
constexpr void introspect(I&& i, fixed_uint<w>& v)
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

} // namespace tg
