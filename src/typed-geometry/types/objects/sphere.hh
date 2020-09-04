#pragma once

#include "../pos.hh"
#include "traits.hh"

#include <typed-geometry/types/scalars/default.hh>

namespace tg
{
/**
 * A ObjectD-dimensional sphere embedded in a DomainD space
 */
template <int ObjectD, class ScalarT, int DomainD = ObjectD, class TraitsT = default_object_tag>
struct sphere;

// Common sphere types

using sphere1 = sphere<1, f32>;
using sphere2 = sphere<2, f32>;
using sphere3 = sphere<3, f32>;
using sphere4 = sphere<4, f32>;

using fsphere1 = sphere<1, f32>;
using fsphere2 = sphere<2, f32>;
using fsphere3 = sphere<3, f32>;
using fsphere4 = sphere<4, f32>;

using dsphere1 = sphere<1, f64>;
using dsphere2 = sphere<2, f64>;
using dsphere3 = sphere<3, f64>;
using dsphere4 = sphere<4, f64>;

using isphere1 = sphere<1, i32>;
using isphere2 = sphere<2, i32>;
using isphere3 = sphere<3, i32>;
using isphere4 = sphere<4, i32>;

using usphere1 = sphere<1, u32>;
using usphere2 = sphere<2, u32>;
using usphere3 = sphere<3, u32>;
using usphere4 = sphere<4, u32>;

// 2D disks embedded in 3D
using sphere2in3 = sphere<2, f32, 3>;
using fsphere2in3 = sphere<2, f32, 3>;
using dsphere2in3 = sphere<2, f64, 3>;
using isphere2in3 = sphere<2, i32, 3>;
using usphere2in3 = sphere<2, u32, 3>;

// boundary
template <int ObjectD, class ScalarT, int DomainD = ObjectD>
using sphere_boundary = sphere<ObjectD, ScalarT, DomainD, boundary_tag>;

// related objects
template <int D, class ScalarT, class TraitsT = default_object_tag>
using disk = sphere<2, ScalarT, D, TraitsT>;
template <int D, class ScalarT>
using disk_boundary = disk<D, ScalarT, boundary_tag>;

using disk2 = disk<2, f32>;
using fdisk2 = disk<2, f32>;
using ddisk2 = disk<2, f64>;
using idisk2 = disk<2, i32>;
using udisk2 = disk<2, u32>;

using disk3 = disk<3, f32>;
using fdisk3 = disk<3, f32>;
using ddisk3 = disk<3, f64>;
using idisk3 = disk<3, i32>;
using udisk3 = disk<3, u32>;

template <int D, class ScalarT>
using circle = sphere<2, ScalarT, D, boundary_tag>;

using circle2 = circle<2, f32>;
using fcircle2 = circle<2, f32>;
using dcircle2 = circle<2, f64>;
using icircle2 = circle<2, i32>;
using ucircle2 = circle<2, u32>;

using circle3 = circle<3, f32>;
using fcircle3 = circle<3, f32>;
using dcircle3 = circle<3, f64>;
using icircle3 = circle<3, i32>;
using ucircle3 = circle<3, u32>;


// ======== IMPLEMENTATION ========

template <int D, class ScalarT, class TraitsT>
struct sphere<D, ScalarT, D, TraitsT>
{
    using scalar_t = ScalarT;
    using pos_t = pos<D, ScalarT>;

    static const sphere unit; ///< unit sphere, center zero, radius 1

    pos_t center;
    scalar_t radius = ScalarT(0);

    constexpr sphere() = default;
    constexpr sphere(pos_t c, ScalarT r) : center(c), radius(r) {}

    template <class OtherT, class OtherTraitsT>
    explicit constexpr sphere(sphere<D, OtherT, D, OtherTraitsT> const& v) : center(v.center), radius(v.radius)
    {
    }

    [[nodiscard]] bool operator==(sphere const& rhs) const { return center == rhs.center && radius == rhs.radius; }
    [[nodiscard]] bool operator!=(sphere const& rhs) const { return !operator==(rhs); }
};

template <class ScalarT, class TraitsT>
struct sphere<2, ScalarT, 3, TraitsT>
{
    using scalar_t = ScalarT;
    using pos_t = pos<3, ScalarT>;
    using dir_t = dir<3, ScalarT>;

    pos_t center;
    scalar_t radius;
    dir_t normal;

    constexpr sphere() = default;
    constexpr sphere(pos_t c, ScalarT r, dir_t n) : center(c), radius(r), normal(n) {}

    template <class OtherT, class OtherTraitsT>
    explicit constexpr sphere(sphere<2, OtherT, 3, OtherTraitsT> const& v) : center(v.center), radius(v.radius), normal(v.normal)
    {
    }

    [[nodiscard]] bool operator==(sphere const& rhs) const { return center == rhs.center && radius == rhs.radius && normal == rhs.normal; }
    [[nodiscard]] bool operator!=(sphere const& rhs) const { return !operator==(rhs); }
};
// This essentially defines two points. It is used for dimension agnostic handling of objects
template <class ScalarT, class TraitsT>
struct sphere<1, ScalarT, 2, TraitsT>
{
    using scalar_t = ScalarT;
    using pos_t = pos<2, ScalarT>;
    using dir_t = dir<2, ScalarT>;

    pos_t center;
    scalar_t radius;
    dir_t normal;

    constexpr sphere() = default;
    constexpr sphere(pos_t c, ScalarT r, dir_t n) : center(c), radius(r), normal(n) {}

    template <class OtherT, class OtherTraitsT>
    explicit constexpr sphere(sphere<1, OtherT, 2, OtherTraitsT> const& v) : center(v.center), radius(v.radius), normal(v.normal)
    {
    }

    [[nodiscard]] bool operator==(sphere const& rhs) const { return center == rhs.center && radius == rhs.radius && normal == rhs.normal; }
    [[nodiscard]] bool operator!=(sphere const& rhs) const { return !operator==(rhs); }
};

template <class I, int ObjectD, class ScalarT, int DomainD, class TraitsT>
constexpr void introspect(I&& i, sphere<ObjectD, ScalarT, DomainD, TraitsT>& v)
{
    if constexpr (ObjectD == DomainD)
    {
        i(v.center, "center");
        i(v.radius, "radius");
    }
    else
    {
        i(v.center, "center");
        i(v.radius, "radius");
        i(v.normal, "normal");
    }
}

template <int ObjectD, class ScalarT, int DomainD, class TraitsT>
struct object_traits<sphere<ObjectD, ScalarT, DomainD, TraitsT>> : detail::finite_object_traits<ObjectD, ScalarT, DomainD, TraitsT>
{
};
} // namespace tg
