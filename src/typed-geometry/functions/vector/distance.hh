#pragma once

#include <typed-geometry/functions/vector/length.hh>

namespace tg
{
// Base case for distance_sqr of point/point
template <int D, class ScalarA, class ScalarB>
[[nodiscard]] constexpr auto distance_sqr(pos<D, ScalarA> const& a, pos<D, ScalarB> const& b) -> decltype(length_sqr(a - b))
{
    return length_sqr(a - b);
}
template <int D, class ScalarA, class ScalarB>
[[deprecated("distance between vectors is ill-defined. did you mean distance_sqr(pos, pos)?")]] [[nodiscard]] constexpr auto distance_sqr(
    vec<D, ScalarA> const& a, vec<D, ScalarB> const& b) -> decltype(length_sqr(a - b))
{
    return length_sqr(a - b);
}
}
