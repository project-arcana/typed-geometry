#pragma once

#include <typed-geometry/detail/operators/ops_vec.hh>
#include <typed-geometry/detail/scalar_traits.hh>
#include <typed-geometry/functions/basic/scalar_math.hh>
#include <typed-geometry/types/objects/segment.hh>
#include <typed-geometry/types/vec.hh>

namespace tg
{
// Base case for vec
template <int D, class ScalarT>
[[nodiscard]] constexpr fractional_result<ScalarT> length_sqr(vec<D, ScalarT> const& v)
{
    auto const fv = vec<D, fractional_result<ScalarT>>(v);
    return dot(fv, fv);
}
template <int D, class ScalarT>
[[deprecated("length of a position is ill-defined. did you mean length(vec)?")]] [[nodiscard]] constexpr ScalarT length_sqr(pos<D, ScalarT> const& v)
{
    return dot(vec(v), vec(v));
}

// Default implementation of length as sqrt(length_sqr)
template <class T>
[[nodiscard]] constexpr auto length(T const& v) -> decltype(sqrt(length_sqr(v)))
{
    return sqrt(length_sqr(v));
}

// Object implementations
template <int D, class ScalarT>
[[nodiscard]] constexpr fractional_result<ScalarT> length(vec<D, ScalarT> const& v)
{
    return sqrt(length_sqr(vec<D, fractional_result<ScalarT>>(v)));
}

template <int D, class ScalarT>
[[nodiscard]] constexpr fractional_result<ScalarT> length_sqr(segment<D, ScalarT> const& s)
{
    return length_sqr(s.pos0 - s.pos1);
}

template <int D, class ScalarT>
[[nodiscard]] constexpr fractional_result<ScalarT> length(segment<D, ScalarT> const& s)
{
    return sqrt(length_sqr(segment<D, fractional_result<ScalarT>>(s)));
}
} // namespace tg
