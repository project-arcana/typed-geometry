#pragma once

#include "../dir.hh"
#include "../pos.hh"
#include "traits.hh"

#include "segment.hh"

namespace tg
{
/**
 * capped circular right cylinder
 *
 * A cylinder is defined by a segment and a radius
 */
template <int D, class ScalarT, class TraitsT = default_object_tag>
struct cylinder;

// Common cylinder types
using cylinder3 = cylinder<3, f32>;
using fcylinder3 = cylinder<3, f32>;
using dcylinder3 = cylinder<3, f64>;
using icylinder3 = cylinder<3, i32>;
using ucylinder3 = cylinder<3, u32>;

template <int D, class ScalarT>
using cylinder_boundary = cylinder<D, ScalarT, boundary_tag>;
template <int D, class ScalarT>
using cylinder_boundary_no_caps = cylinder<D, ScalarT, boundary_no_caps_tag>;

// ======== IMPLEMENTATION ========
template <class ScalarT, class TraitsT>
struct cylinder<3, ScalarT, TraitsT>
{
    using scalar_t = ScalarT;
    using pos_t = pos<3, ScalarT>;
    using dir_t = dir<3, ScalarT>;
    using seg_t = segment<3, ScalarT>;

    seg_t axis;
    scalar_t radius = ScalarT(0);

    constexpr cylinder() = default;
    constexpr cylinder(seg_t const& axis, scalar_t radius) : axis(axis), radius(radius) {}
    constexpr cylinder(pos_t const& p0, pos_t const& p1, scalar_t radius) : axis(p0, p1), radius(radius) {}

    template <class OtherT, class OtherTraitsT>
    explicit constexpr cylinder(cylinder<3, OtherT, OtherTraitsT> const& v) : axis(v.axis), radius(v.radius)
    {
    }

    [[nodiscard]] bool operator==(cylinder const& rhs) const { return axis == rhs.axis && radius == rhs.radius; }
    [[nodiscard]] bool operator!=(cylinder const& rhs) const { return !operator==(rhs); }
};

template <class I, int D, class ScalarT, class TraitsT>
constexpr void introspect(I&& i, cylinder<D, ScalarT, TraitsT>& v)
{
    i(v.axis, "axis");
    i(v.radius, "radius");
}

template <int D, class ScalarT, class TraitsT>
struct object_traits<cylinder<D, ScalarT, TraitsT>> : detail::finite_object_traits<D, ScalarT, D, TraitsT>
{
};
} // namespace tg
