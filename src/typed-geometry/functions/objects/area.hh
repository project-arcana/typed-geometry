#pragma once

#include <typed-geometry/types/objects/aabb.hh>
#include <typed-geometry/types/objects/box.hh>
#include <typed-geometry/types/objects/capsule.hh>
#include <typed-geometry/types/objects/cylinder.hh>
#include <typed-geometry/types/objects/ellipse.hh>
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

namespace tg
{
// ======== inner area of 2D objects ========

template <class ScalarT>
[[nodiscard]] constexpr ScalarT area_of(size<2, ScalarT> const& s)
{
    return s.width * s.height;
}

template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr ScalarT area_of(aabb<2, ScalarT, TraitsT> const& b)
{
    return area_of(size<2, ScalarT>(b.max - b.min));
}

template <class ScalarT, int D, class TraitsT>
[[nodiscard]] constexpr ScalarT area_of(box<2, ScalarT, D, TraitsT> const& b)
{
    return ScalarT(4) * sqrt(length_sqr(b.half_extents[0]) * length_sqr(b.half_extents[1]));
}

template <class ScalarT>
[[nodiscard]] constexpr fractional_result<ScalarT> signed_area_of(triangle<2, ScalarT> const& b)
{
    return cross(b.pos1 - b.pos0, b.pos2 - b.pos0) * fractional_result<ScalarT>(0.5);
}

template <class ScalarT>
[[nodiscard]] constexpr fractional_result<ScalarT> area_of(triangle<2, ScalarT> const& b)
{
    return abs(signed_area_of(b));
}

template <class ScalarT>
[[nodiscard]] constexpr fractional_result<ScalarT> area_of(triangle<3, ScalarT> const& b)
{
    return length(cross(b.pos1 - b.pos0, b.pos2 - b.pos0)) * fractional_result<ScalarT>(0.5);
}

template <class ScalarT, int D, class TraitsT>
[[nodiscard]] constexpr fractional_result<ScalarT> area_of(sphere<2, ScalarT, D, TraitsT> const& b)
{
    return tg::pi_scalar<fractional_result<ScalarT>> * pow2(b.radius);
}

template <class ScalarT>
[[nodiscard]] constexpr fractional_result<ScalarT> area_of(hemisphere<2, ScalarT> const& b)
{
    return tg::pi_scalar<fractional_result<ScalarT>> * pow2(b.radius) * fractional_result<ScalarT>(0.5);
}

template <class ScalarT, int D, class TraitsT>
[[nodiscard]] constexpr fractional_result<ScalarT> area_of(ellipse<2, ScalarT, D, TraitsT> const& e)
{
    return tg::pi_scalar<fractional_result<ScalarT>> * length(e.semi_axes[0]) * length(e.semi_axes[1]);
}

// ======== surface area of 3D objects ========

template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr fractional_result<ScalarT> area_of(sphere<3, ScalarT, 3, TraitsT> const& b)
{
    return ScalarT(4) * tg::pi_scalar<fractional_result<ScalarT>> * pow2(b.radius);
}

template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr fractional_result<ScalarT> area_of(hemisphere<3, ScalarT, TraitsT> const& b)
{
    if constexpr (std::is_same_v<TraitsT, boundary_no_caps_tag>)
        return ScalarT(2) * tg::pi_scalar<fractional_result<ScalarT>> * pow2(b.radius);
    else
        return ScalarT(3) * tg::pi_scalar<fractional_result<ScalarT>> * pow2(b.radius);
}

// Only an approximation, but with a relative error of at most 1.061%
template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr fractional_result<ScalarT> area_of(ellipse<3, ScalarT, 3, TraitsT> const& e)
{
    // see https://en.wikipedia.org/wiki/Ellipsoid#Surface_area
    using frac_t = fractional_result<ScalarT>;
    constexpr auto p = frac_t(1.6075);
    const auto a = length(e.semi_axes[0]);
    const auto b = length(e.semi_axes[1]);
    const auto c = length(e.semi_axes[2]);
    const auto base = (tg::pow(a * b, p) + tg::pow(a * c, p) + tg::pow(b * c, p)) / frac_t(3);
    return ScalarT(4) * tg::pi_scalar<frac_t> * tg::pow(base, frac_t(1) / p);
}

template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr fractional_result<ScalarT> area_of(cylinder<3, ScalarT, TraitsT> const& b)
{
    if constexpr (std::is_same_v<TraitsT, boundary_no_caps_tag>)
        return ScalarT(2) * tg::pi_scalar<fractional_result<ScalarT>> * b.radius * length(b.axis);
    else
        return ScalarT(2) * tg::pi_scalar<fractional_result<ScalarT>> * b.radius * (b.radius + length(b.axis)); // == 2 pi r^2 + 2 pi r h
}

template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr fractional_result<ScalarT> area_of(capsule<3, ScalarT, TraitsT> const& b)
{
    return ScalarT(2) * tg::pi_scalar<fractional_result<ScalarT>> * b.radius * (ScalarT(2) * b.radius + length(b.axis)); // == 4 pi r^2 + 2 pi r h
}

template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr ScalarT area_of(aabb<3, ScalarT, TraitsT> const& b)
{
    auto s = size<3, ScalarT>(b.max - b.min);
    return ScalarT(2) * (s.width * s.height + s.width * s.depth + s.height * s.depth);
}

template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr ScalarT area_of(box<3, ScalarT, 3, TraitsT> const& b)
{
    auto w = length(b.half_extents[0]);
    auto h = length(b.half_extents[1]);
    auto d = length(b.half_extents[2]);
    return ScalarT(8) * (w * h + w * d + h * d);
}

template <class BaseT, class TraitsT>
[[nodiscard]] constexpr fractional_result<typename BaseT::scalar_t> area_of(pyramid<BaseT, TraitsT> const& py)
{
    using ScalarT = fractional_result<typename BaseT::scalar_t>;
    if constexpr (std::is_same_v<TraitsT, boundary_no_caps_tag>)
    {
        if constexpr (std::is_same_v<BaseT, sphere<2, ScalarT, 3>>)
            return tg::pi_scalar<ScalarT> * py.base.radius * sqrt(pow2(py.base.radius) + pow2(py.height));
        else
        {
            auto areaSum = ScalarT(0);
            for (const auto& face : faces_of(py))
                areaSum += area_of(face);
            return areaSum;
        }
    }
    else
        return area_of(py.base) + area_of(boundary_no_caps_of(py));
}


template <class ObjectT>
[[deprecated("use area_of")]] [[nodiscard]] constexpr auto area(ObjectT const& o) -> decltype(area_of(o))
{
    return area_of(o);
}
template <class ObjectT>
[[deprecated("use signed_area_of")]] [[nodiscard]] constexpr auto signed_area(ObjectT const& o) -> decltype(signed_area_of(o))
{
    return signed_area_of(o);
}
} // namespace tg
