#pragma once

#include <typed-geometry/detail/utility.hh>
#include <typed-geometry/types/scalars/default.hh>
#include <typed-geometry/types/scalars/fixed_int.hh>

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
    explicit constexpr fixed_uint(fixed_int<rhs_words> const& rhs)
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
    explicit constexpr fixed_uint(fixed_uint<rhs_words> rhs);

    // implicit promotion
    template <int rhs_words, class = enable_if<(rhs_words < words)>>
    constexpr fixed_uint(fixed_uint<rhs_words> rhs);


    // explicit conversion
    explicit constexpr operator u64() const { return d[0]; }
    explicit constexpr operator float();
    explicit constexpr operator double();
};

static_assert(sizeof(u128) == 128 / 8, "u128 is not 128 bit!");
static_assert(sizeof(u192) == 192 / 8, "u192 is not 192 bit!");
static_assert(sizeof(u256) == 256 / 8, "u256 is not 256 bit!");
}
