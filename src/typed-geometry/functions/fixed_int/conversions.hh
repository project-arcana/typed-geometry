#pragma once

#include <typed-geometry/detail/scalar_traits.hh>
#include <typed-geometry/types/scalars/fixed_int.hh>
#include <typed-geometry/types/scalars/fixed_uint.hh>

namespace tg
{
template <int words>
template <int rhs_words>
constexpr fixed_int<words>::fixed_int(fixed_uint<rhs_words> const& rhs)
{
    d[0] = rhs.d[0];
    if constexpr (rhs_words > 1 && words > 1)
        d[1] = rhs.d[1];
    if constexpr (rhs_words > 2 && words > 2)
        d[2] = rhs.d[2];
    if constexpr (rhs_words > 3 && words > 3)
        d[3] = rhs.d[3];
}

template <int words>
template <int rhs_words>
constexpr fixed_uint<words>::fixed_uint(fixed_int<rhs_words> const& rhs)
{
    d[0] = rhs.d[0];
    if constexpr (rhs_words > 1 && words > 1)
        d[1] = rhs.d[1];
    if constexpr (rhs_words > 2 && words > 2)
        d[2] = rhs.d[2];
    if constexpr (rhs_words > 3 && words > 3)
        d[3] = rhs.d[3];
}

template <int w>
struct make_unsigned_t<fixed_int<w>>
{
    using type = fixed_uint<w>;
};
template <int w>
struct make_unsigned_t<fixed_int<w> const>
{
    using type = fixed_uint<w> const;
};
template <int w>
struct make_unsigned_t<fixed_uint<w>>
{
    using type = fixed_uint<w>;
};
template <int w>
struct make_unsigned_t<fixed_uint<w> const>
{
    using type = fixed_uint<w> const;
};
template <int w>
struct make_signed_t<fixed_uint<w>>
{
    using type = fixed_int<w>;
};
template <int w>
struct make_signed_t<fixed_uint<w> const>
{
    using type = fixed_int<w> const;
};
template <int w>
struct make_signed_t<fixed_int<w>>
{
    using type = fixed_int<w>;
};
template <int w>
struct make_signed_t<fixed_int<w> const>
{
    using type = fixed_int<w> const;
};
}
