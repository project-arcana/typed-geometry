#pragma once

#include <typed-geometry/common/assert.hh>
#include <typed-geometry/detail/scalar_traits.hh>
#include <typed-geometry/functions/normal.hh>
#include <typed-geometry/functions/normalize.hh>
#include <typed-geometry/tests/vec_tests.hh>
#include <typed-geometry/types/angle.hh>
#include <typed-geometry/types/objects/triangle.hh>
#include <typed-geometry/types/vec.hh>

namespace tg
{
// returns the (smaller) angle between two vectors, i.e. the result is in 0..pi (0°..180°)
template <int D, class ScalarT>
TG_NODISCARD constexpr angle<fractional_result<ScalarT>> angle_between(vec<D, ScalarT> const& a, vec<D, ScalarT> const& b)
{
    auto a_unit = normalize_safe(a);
    auto b_unit = normalize_safe(b);
    return acos(saturate(dot(a_unit, b_unit)));
}

// returns the angle between any two objects with unambiguous normals. The result is in 0..pi (0°..180°)
template <class A, class B>
TG_NODISCARD constexpr auto angle_between(A const& a, B const& b) -> decltype(acos(dot(normal(a), normal(b))))
{
    return acos(saturate(dot(normal(a), normal(b))));
}

// Returns the angle of a rotation of a towards b about the orthogonal_axis
// The orthogonal axis is important to determine the direction of orientation (axb vs -axb)
template <class ScalarT>
TG_NODISCARD constexpr angle<fractional_result<ScalarT>> angle_towards(vec<3, ScalarT> const& a, vec<3, ScalarT> const& b, vec<3, ScalarT> const& orthogonal_axis)
{
    TG_ASSERT(are_orthogonal(a, orthogonal_axis));
    TG_ASSERT(are_orthogonal(b, orthogonal_axis));

    auto a_unit = normalize_safe(a);
    auto b_unit = normalize_safe(b);
    auto axis_unit = normalize_safe(orthogonal_axis);
    auto angle = tg::atan2(dot(cross(a_unit, b_unit), axis_unit), dot(a_unit, b_unit));
    return angle;
}
}
