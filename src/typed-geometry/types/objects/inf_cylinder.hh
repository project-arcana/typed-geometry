#pragma once

#include "../dir.hh"
#include "../pos.hh"
#include "line.hh"
#include "traits.hh"

namespace tg
{
/**
 * infinite tube
 *
 * An infinite tube is defined by a position and an axis
 */
template <int D, class ScalarT, class TraitsT = default_object_tag>
struct inf_cylinder;

// Common infinte tube types
using inf_cylinder3 = inf_cylinder<3, f32>;
using finttube3 = inf_cylinder<3, f32>;
using dinf_cylinder3 = inf_cylinder<3, f64>;
using iinf_cylinder3 = inf_cylinder<3, i32>;
using uinf_cylinder3 = inf_cylinder<3, u32>;

template <int D, class ScalarT>
using inf_cylinder_boundary = inf_cylinder<D, ScalarT, boundary_tag>;

// ======== IMPLEMENTATION ========
template <int D, class ScalarT, class TraitsT>
struct inf_cylinder
{
    using pos_t = pos<D, ScalarT>;
    using dir_t = dir<D, ScalarT>;
    using line_t = line<D, ScalarT>;

    line_t axis;
    ScalarT radius = ScalarT(0);

    constexpr inf_cylinder() = default;
    constexpr inf_cylinder(line_t const& axis, ScalarT radius) : axis(axis), radius(radius) {}

    [[nodiscard]] bool operator==(inf_cylinder const& rhs) const { return axis == rhs.axis && radius == rhs.radius; }
    [[nodiscard]] bool operator!=(inf_cylinder const& rhs) const { return !operator==(rhs); }
};

template <class I, int D, class ScalarT, class TraitsT>
constexpr void introspect(I&& i, inf_cylinder<D, ScalarT, TraitsT>& v)
{
    i(v.axis, "axis");
    i(v.radius, "radius");
}
} // namespace tg
