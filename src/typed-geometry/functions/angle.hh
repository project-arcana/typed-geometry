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
TG_NODISCARD constexpr angle_t<fractional_result<ScalarT>> angle_between(vec<D, ScalarT> const& a, vec<D, ScalarT> const& b)
{
    constexpr auto lower = decltype(dot(normalize_safe(a), normalize_safe(b)))(-1);
    constexpr auto upper = decltype(dot(normalize_safe(a), normalize_safe(b)))(1);
    auto a_unit = normalize_safe(a);
    auto b_unit = normalize_safe(b);
    return acos(clamp(dot(a_unit, b_unit), lower, upper));
}

// returns the (smaller) angle between two directions, i.e. the result is in 0..pi (0°..180°)
template <int D, class ScalarT>
TG_NODISCARD constexpr angle_t<fractional_result<ScalarT>> angle_between(dir<D, ScalarT> const& a, dir<D, ScalarT> const& b)
{
    constexpr auto lower = decltype(dot(normal(a), normal(b)))(-1);
    constexpr auto upper = decltype(dot(normal(a), normal(b)))(1);
    return acos(clamp(dot(a, b), lower, upper));
}

// returns the angle between any two objects with unambiguous normals. The result is in 0..pi (0°..180°)
template <class A, class B>
TG_NODISCARD constexpr auto angle_between(A const& a, B const& b) -> decltype(acos(dot(normal(a), normal(b))))
{
    // TODO(ks): call to angle_between(dir, dir)?
    constexpr auto lower = decltype(dot(normal(a), normal(b)))(-1);
    constexpr auto upper = decltype(dot(normal(a), normal(b)))(1);
    return acos(clamp(dot(normal(a), normal(b))), lower, upper);
}

// Returns the angle of a rotation of a towards b about the orthogonal_axis
// The orthogonal axis is important to determine the direction of orientation (axb vs -axb)
template <class ScalarT>
TG_NODISCARD constexpr angle_t<fractional_result<ScalarT>> angle_towards(vec<3, ScalarT> const& a, vec<3, ScalarT> const& b, vec<3, ScalarT> const& orthogonal_axis)
{
    TG_CONTRACT(are_orthogonal(a, orthogonal_axis));
    TG_CONTRACT(are_orthogonal(b, orthogonal_axis));

    auto a_unit = normalize_safe(a);
    auto b_unit = normalize_safe(b);
    auto axis_unit = normalize_safe(orthogonal_axis);
    auto angle = tg::atan2(dot(cross(a_unit, b_unit), axis_unit), dot(a_unit, b_unit));
    return angle;
}
}
