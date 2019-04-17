#pragma once

#include <typed-geometry/types/objects/triangle.hh>
#include <typed-geometry/types/size.hh>
#include <typed-geometry/detail/scalar_traits.hh>

namespace tg
{
// -- operator@ --

template <int D, class ScalarT>
TG_NODISCARD constexpr triangle<D, ScalarT> operator+(triangle<D, ScalarT> const& a, vec<D, ScalarT> const& b)
{
    triangle<D, ScalarT> r;
    r.v0 = a.v0 + b;
    r.v1 = a.v1 + b;
    r.v2 = a.v2 + b;
    return r;
}

template <int D, class ScalarT>
TG_NODISCARD constexpr triangle<D, ScalarT> operator-(triangle<D, ScalarT> const& a, vec<D, ScalarT> const& b)
{
    triangle<D, ScalarT> r;
    r.v0 = a.v0 - b;
    r.v1 = a.v1 - b;
    r.v2 = a.v2 - b;
    return r;
}

template <int D, class ScalarT>
TG_NODISCARD constexpr triangle<D, ScalarT> operator*(triangle<D, ScalarT> const& a, ScalarT b)
{
    triangle<D, ScalarT> r;
    r.v0 = a.v0 * b;
    r.v1 = a.v1 * b;
    r.v2 = a.v2 * b;
    return r;
}

template <int D, class ScalarT>
TG_NODISCARD constexpr triangle<D, ScalarT> operator*(triangle<D, ScalarT> const& a, size<D, ScalarT> const& b)
{
    triangle<D, ScalarT> r;
    r.v0 = a.v0 * b;
    r.v1 = a.v1 * b;
    r.v2 = a.v2 * b;
    return r;
}

template <int D, class ScalarT, class = enable_if<is_floating_point<ScalarT>>>
TG_NODISCARD constexpr triangle<D, ScalarT> operator/(triangle<D, ScalarT> const& a, ScalarT b)
{
    triangle<D, ScalarT> r;
    auto inv_b = ScalarT(1) / b;
    r.v0 = a.v0 * inv_b;
    r.v1 = a.v1 * inv_b;
    r.v2 = a.v2 * inv_b;
    return r;
}

template <int D, class IntegerT, class ScalarT, class = enable_if<is_floating_point<ScalarT> && is_integer<IntegerT>>>
TG_NODISCARD constexpr triangle<D, ScalarT> operator/(triangle<D, IntegerT> const& a, ScalarT b)
{
    triangle<D, ScalarT> r;
    auto inv_b = ScalarT(1) / b;
    r.v0 = a.v0 * inv_b;
    r.v1 = a.v1 * inv_b;
    r.v2 = a.v2 * inv_b;
    return r;
}

template <int D, class ScalarT, class = enable_if<is_integer<ScalarT>>, class = detail::unused>
TG_NODISCARD constexpr triangle<D, ScalarT> operator/(triangle<D, ScalarT> const& a, ScalarT b)
{
    triangle<D, ScalarT> r;
    r.v0 = a.v0 / b;
    r.v1 = a.v1 / b;
    r.v2 = a.v2 / b;
    return r;
}

template <int D, class ScalarT>
TG_NODISCARD constexpr triangle<D, ScalarT> operator/(triangle<D, ScalarT> const& a, size<D, ScalarT> const& b)
{
    triangle<D, ScalarT> r;
    auto inv_b = ScalarT(1) / b;
    r.v0 = a.v0 * inv_b;
    r.v1 = a.v1 * inv_b;
    r.v2 = a.v2 * inv_b;
    return r;
}
} // namespace tg
