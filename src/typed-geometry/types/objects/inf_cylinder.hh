#pragma once

#include "../dir.hh"
#include "../pos.hh"
#include "line.hh"
#include "traits.hh"

namespace tg
{
/**
 * infinite cylinder
 *
 * An infinite cylinder is defined by an axis and a radius
 */
template <int D, class ScalarT, class TraitsT = default_object_tag>
struct inf_cylinder;

// Common infinite cylinder types
using inf_cylinder2 = inf_cylinder<2, f32>;
using finf_cylinder2 = inf_cylinder<2, f32>;
using dinf_cylinder2 = inf_cylinder<2, f64>;
using iinf_cylinder2 = inf_cylinder<2, i32>;
using uinf_cylinder2 = inf_cylinder<2, u32>;

using inf_cylinder3 = inf_cylinder<3, f32>;
using finf_cylinder3 = inf_cylinder<3, f32>;
using dinf_cylinder3 = inf_cylinder<3, f64>;
using iinf_cylinder3 = inf_cylinder<3, i32>;
using uinf_cylinder3 = inf_cylinder<3, u32>;

template <int D, class ScalarT>
using inf_cylinder_boundary = inf_cylinder<D, ScalarT, boundary_tag>;

// ======== IMPLEMENTATION ========
template <int D, class ScalarT, class TraitsT>
struct inf_cylinder
{
    using scalar_t = ScalarT;
    using pos_t = pos<D, ScalarT>;
    using dir_t = dir<D, ScalarT>;
    using line_t = line<D, ScalarT>;

    line_t axis;
    ScalarT radius = ScalarT(0);

    constexpr inf_cylinder() = default;
    constexpr inf_cylinder(line_t const& axis, ScalarT radius) : axis(axis), radius(radius) {}

    template <class OtherT, class OtherTraitsT>
    explicit constexpr inf_cylinder(inf_cylinder<D, OtherT, OtherTraitsT> const& v) : axis(v.axis), radius(v.radius)
    {
    }

    [[nodiscard]] bool operator==(inf_cylinder const& rhs) const { return axis == rhs.axis && radius == rhs.radius; }
    [[nodiscard]] bool operator!=(inf_cylinder const& rhs) const { return !operator==(rhs); }
};

template <class I, int D, class ScalarT, class TraitsT>
constexpr void introspect(I&& i, inf_cylinder<D, ScalarT, TraitsT>& v)
{
    i(v.axis, "axis");
    i(v.radius, "radius");
}

template <int D, class ScalarT, class TraitsT>
struct object_traits<inf_cylinder<D, ScalarT, TraitsT>> : detail::infinite_object_traits<D, ScalarT, D, TraitsT>
{
};
} // namespace tg
