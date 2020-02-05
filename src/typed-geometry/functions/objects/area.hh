#pragma once

#include <typed-geometry/types/objects/aabb.hh>
#include <typed-geometry/types/objects/box.hh>
#include <typed-geometry/types/objects/capsule.hh>
#include <typed-geometry/types/objects/cone.hh>
#include <typed-geometry/types/objects/cylinder.hh>
#include <typed-geometry/types/objects/hemisphere.hh>
#include <typed-geometry/types/objects/pyramid.hh>
#include <typed-geometry/types/objects/sphere.hh>
#include <typed-geometry/types/objects/triangle.hh>

#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/size.hh>
#include <typed-geometry/types/vec.hh>

#include <typed-geometry/detail/operators/ops_vec.hh>
#include <typed-geometry/detail/scalar_traits.hh>
#include <typed-geometry/functions/basic/scalar_math.hh>
#include <typed-geometry/functions/vector/length.hh>

#include "perimeter.hh"

namespace tg
{
template <class ScalarT>
[[nodiscard]] constexpr ScalarT area(size<2, ScalarT> const& s)
{
    return s.width * s.height;
}

template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr ScalarT area(aabb<2, ScalarT, TraitsT> const& b)
{
    return area(size<2, ScalarT>(b.max - b.min));
}

template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr ScalarT area(aabb<3, ScalarT, TraitsT> const& b)
{
    auto s = size<3, ScalarT>(b.max - b.min);
    return 2 * (s.width * s.height + s.width * s.depth + s.height * s.depth);
}

template <class ScalarT, int D, class TraitsT>
[[nodiscard]] constexpr ScalarT area(box<2, ScalarT, D, TraitsT> const& b)
{
    return 4 * sqrt(length_sqr(b.half_extents[0]) * length_sqr(b.half_extents[1]));
}
template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr ScalarT area(box<3, ScalarT, 3, TraitsT> const& b)
{
    auto w = length(b.half_extents[0]);
    auto h = length(b.half_extents[1]);
    auto d = length(b.half_extents[2]);
    return 8 * (w * h + w * d + h * d);
}

template <class ScalarT>
[[nodiscard]] constexpr fractional_result<ScalarT> signed_area(triangle<2, ScalarT> const& b)
{
    return cross(b.pos1 - b.pos0, b.pos2 - b.pos0) * fractional_result<ScalarT>(0.5);
}

template <class ScalarT>
[[nodiscard]] constexpr fractional_result<ScalarT> area(triangle<2, ScalarT> const& b)
{
    return abs(signed_area(b));
}

template <class ScalarT>
[[nodiscard]] constexpr fractional_result<ScalarT> area(triangle<3, ScalarT> const& b)
{
    return length(cross(b.pos1 - b.pos0, b.pos2 - b.pos0)) * fractional_result<ScalarT>(0.5);
}

template <class ScalarT, int D, class TraitsT>
[[nodiscard]] constexpr fractional_result<ScalarT> area(sphere<2, ScalarT, D, TraitsT> const& b)
{
    return tg::pi_scalar<fractional_result<ScalarT>> * tg::pow2(b.radius);
}

template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr fractional_result<ScalarT> area(sphere<3, ScalarT, 3, TraitsT> const& b)
{
    return (4 * tg::pi_scalar<fractional_result<ScalarT>>)*tg::pow2(b.radius);
}

template <class ScalarT>
[[nodiscard]] constexpr fractional_result<ScalarT> area(cylinder<3, ScalarT> const& b)
{
    return tg::pi_scalar<fractional_result<ScalarT>> * tg::pow2(b.radius) * 2           // caps
           + 2 * tg::pi_scalar<fractional_result<ScalarT>> * b.radius * length(b.axis); // mantle
}

template <class ScalarT>
[[nodiscard]] constexpr ScalarT area(cone<3, ScalarT> const& b)
{
    // TODO: FIXME
    return area(b.base) + tg::pi_scalar<ScalarT> * b.base.radius * sqrt(pow2(b.base.radius) + pow2(b.height));
}
} // namespace tg
