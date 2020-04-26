#pragma once

#include <typed-geometry/detail/scalar_traits.hh>
#include <typed-geometry/types/objects/triangle.hh>
#include <typed-geometry/types/size.hh>

namespace tg
{
// -- operator@ --

template <int D, class ScalarT>
[[nodiscard]] constexpr triangle<D, ScalarT> operator+(triangle<D, ScalarT> const& a, vec<D, ScalarT> const& b)
{
    triangle<D, ScalarT> r;
    r.pos0 = a.pos0 + b;
    r.pos1 = a.pos1 + b;
    r.pos2 = a.pos2 + b;
    return r;
}

template <int D, class ScalarT>
[[nodiscard]] constexpr triangle<D, ScalarT> operator-(triangle<D, ScalarT> const& a, vec<D, ScalarT> const& b)
{
    triangle<D, ScalarT> r;
    r.pos0 = a.pos0 - b;
    r.pos1 = a.pos1 - b;
    r.pos2 = a.pos2 - b;
    return r;
}

template <int D, class ScalarT>
[[nodiscard]] constexpr triangle<D, ScalarT> operator*(triangle<D, ScalarT> const& a, ScalarT b)
{
    triangle<D, ScalarT> r;
    r.pos0 = a.pos0 * b;
    r.pos1 = a.pos1 * b;
    r.pos2 = a.pos2 * b;
    return r;
}

template <int D, class ScalarT>
[[nodiscard]] constexpr triangle<D, ScalarT> operator*(triangle<D, ScalarT> const& a, size<D, ScalarT> const& b)
{
    triangle<D, ScalarT> r;
    r.pos0 = a.pos0 * b;
    r.pos1 = a.pos1 * b;
    r.pos2 = a.pos2 * b;
    return r;
}

template <int D, class ScalarT, class = enable_if<is_floating_point<ScalarT>>>
[[nodiscard]] constexpr triangle<D, ScalarT> operator/(triangle<D, ScalarT> const& a, ScalarT b)
{
    triangle<D, ScalarT> r;
    auto inv_b = ScalarT(1) / b;
    r.pos0 = a.pos0 * inv_b;
    r.pos1 = a.pos1 * inv_b;
    r.pos2 = a.pos2 * inv_b;
    return r;
}

template <int D, class IntegerT, class ScalarT, class = enable_if<is_floating_point<ScalarT> && is_integer<IntegerT>>>
[[nodiscard]] constexpr triangle<D, ScalarT> operator/(triangle<D, IntegerT> const& a, ScalarT b)
{
    triangle<D, ScalarT> r;
    auto inv_b = ScalarT(1) / b;
    r.pos0 = a.pos0 * inv_b;
    r.pos1 = a.pos1 * inv_b;
    r.pos2 = a.pos2 * inv_b;
    return r;
}

template <int D, class ScalarT, class = enable_if<is_integer<ScalarT>>, class = detail::unused>
[[nodiscard]] constexpr triangle<D, ScalarT> operator/(triangle<D, ScalarT> const& a, ScalarT b)
{
    triangle<D, ScalarT> r;
    r.pos0 = a.pos0 / b;
    r.pos1 = a.pos1 / b;
    r.pos2 = a.pos2 / b;
    return r;
}

template <int D, class ScalarT>
[[nodiscard]] constexpr triangle<D, ScalarT> operator/(triangle<D, ScalarT> const& a, size<D, ScalarT> const& b)
{
    triangle<D, ScalarT> r;
    auto inv_b = ScalarT(1) / b;
    r.pos0 = a.pos0 * inv_b;
    r.pos1 = a.pos1 * inv_b;
    r.pos2 = a.pos2 * inv_b;
    return r;
}
} // namespace tg
