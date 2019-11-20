#pragma once

#include <typed-geometry/detail/utility.hh>
#include <typed-geometry/types/scalars/default.hh>
#include <typed-geometry/types/scalars/fixed_uint.hh>

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
    explicit constexpr fixed_int(fixed_uint<rhs_words> const& rhs)
    {
        d[0] = rhs.d[0];
        if constexpr (rhs_words > 1 && words > 1)
            d[1] = rhs.d[1];
        if constexpr (rhs_words > 2 && words > 2)
            d[2] = rhs.d[2];
        if constexpr (rhs_words > 3 && words > 3)
            d[3] = rhs.d[3];
    }

    // explicit down cast
    template <int rhs_words, class = enable_if<(rhs_words > words)>, class = void>
    explicit constexpr fixed_int(fixed_int<rhs_words> rhs);

    // implicit promotion
    template <int rhs_words, class = enable_if<(rhs_words < words)>>
    constexpr fixed_int(fixed_int<rhs_words> rhs);

    // explicit conversion
    explicit constexpr operator i64() { return i64(d[0]); }
    explicit constexpr operator float();
    explicit constexpr operator double();
};

static_assert(sizeof(i128) == 128 / 8, "i128 is not 128 bit!");
static_assert(sizeof(i192) == 192 / 8, "i192 is not 192 bit!");
static_assert(sizeof(i256) == 256 / 8, "i256 is not 256 bit!");
}
