#pragma once

#include "../dir.hh"
#include "../pos.hh"
#include "traits.hh"

#include "segment.hh"

namespace tg
{
/**
 * sphere-capped right cylinder
 *
 * A capsule is defined by a segment and a radius
 */
template <int D, class ScalarT, class TraitsT = default_object_tag>
struct capsule;

// Common capsule types
using capsule3 = capsule<3, f32>;
using fcapsule3 = capsule<3, f32>;
using dcapsule3 = capsule<3, f64>;
using icapsule3 = capsule<3, i32>;
using ucapsule3 = capsule<3, u32>;

template <int D, class ScalarT>
using capsule_boundary = capsule<D, ScalarT, boundary_tag>;

// ======== IMPLEMENTATION ========
template <class ScalarT, class TraitsT>
struct capsule<3, ScalarT, TraitsT>
{
    using scalar_t = ScalarT;
    using pos_t = pos<3, ScalarT>;
    using dir_t = dir<3, ScalarT>;
    using seg_t = segment<3, ScalarT>;

    seg_t axis;
    scalar_t radius = ScalarT(0);

    constexpr capsule() = default;
    constexpr capsule(seg_t const& axis, scalar_t radius) : axis(axis), radius(radius) {}
    constexpr capsule(pos_t const& p0, pos_t const& p1, scalar_t radius) : axis(p0, p1), radius(radius) {}

    template <class OtherT, class OtherTraitsT>
    explicit constexpr capsule(capsule<3, OtherT, OtherTraitsT> const& v) : axis(v.axis), radius(v.radius)
    {
    }

    [[nodiscard]] bool operator==(capsule const& rhs) const { return axis == rhs.axis && radius == rhs.radius; }
    [[nodiscard]] bool operator!=(capsule const& rhs) const { return !operator==(rhs); }
};

template <class I, int D, class ScalarT, class TraitsT>
constexpr void introspect(I&& i, capsule<D, ScalarT, TraitsT>& v)
{
    i(v.axis, "axis");
    i(v.radius, "radius");
}

template <int D, class ScalarT, class TraitsT>
struct object_traits<capsule<D, ScalarT, TraitsT>> : detail::finite_object_traits<D, ScalarT, D, TraitsT>
{
};
} // namespace tg
