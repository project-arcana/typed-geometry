#pragma once

#include <typed-geometry/types/scalars/default.hh>
#include "../dir.hh"
#include "../pos.hh"
#include "traits.hh"

namespace tg
{
/**
 * Half of the surface of a completely round ball
 * (which half is given by direction vector)
 */
template <int D, class ScalarT, class TraitsT = default_object_tag>
struct hemisphere;

// Common hemisphere types

using hemisphere1 = hemisphere<1, f32>;
using hemisphere2 = hemisphere<2, f32>;
using hemisphere3 = hemisphere<3, f32>;
using hemisphere4 = hemisphere<4, f32>;

using fhemisphere1 = hemisphere<1, f32>;
using fhemisphere2 = hemisphere<2, f32>;
using fhemisphere3 = hemisphere<3, f32>;
using fhemisphere4 = hemisphere<4, f32>;

using dhemisphere1 = hemisphere<1, f64>;
using dhemisphere2 = hemisphere<2, f64>;
using dhemisphere3 = hemisphere<3, f64>;
using dhemisphere4 = hemisphere<4, f64>;

using ihemisphere1 = hemisphere<1, i32>;
using ihemisphere2 = hemisphere<2, i32>;
using ihemisphere3 = hemisphere<3, i32>;
using ihemisphere4 = hemisphere<4, i32>;

using uhemisphere1 = hemisphere<1, u32>;
using uhemisphere2 = hemisphere<2, u32>;
using uhemisphere3 = hemisphere<3, u32>;
using uhemisphere4 = hemisphere<4, u32>;

template <int D, class ScalarT>
using hemisphere_boundary = hemisphere<D, ScalarT, boundary_tag>;
template <int D, class ScalarT>
using hemisphere_boundary_no_caps = hemisphere<D, ScalarT, boundary_no_caps_tag>;


// ======== IMPLEMENTATION ========

template <int D, class ScalarT, class TraitsT>
struct hemisphere
{
    using scalar_t = ScalarT;
    using pos_t = pos<D, ScalarT>;
    using dir_t = dir<D, ScalarT>;

    pos_t center;
    scalar_t radius;
    dir_t normal;

    constexpr hemisphere() = default;
    constexpr hemisphere(pos_t const& c, ScalarT r, dir_t const& normal) : center(c), radius(r), normal(normal) {}

    template <class OtherT, class OtherTraitsT>
    explicit constexpr hemisphere(hemisphere<D, OtherT, OtherTraitsT> const& v) : center(v.center), radius(v.radius), normal(v.normal)
    {
    }

    [[nodiscard]] bool operator==(hemisphere const& rhs) const { return center == rhs.center && radius == rhs.radius && normal == rhs.normal; }
    [[nodiscard]] bool operator!=(hemisphere const& rhs) const { return !operator==(rhs); }
};

template <class I, int D, class ScalarT, class TraitsT>
constexpr void introspect(I&& i, hemisphere<D, ScalarT, TraitsT>& v)
{
    i(v.center, "center");
    i(v.radius, "radius");
    i(v.normal, "normal");
}

template <int D, class ScalarT, class TraitsT>
struct object_traits<hemisphere<D, ScalarT, TraitsT>> : detail::finite_object_traits<D, ScalarT, D, TraitsT>
{
};
} // namespace tg
