#pragma once

#include <typed-geometry/types/scalars/default.hh>
#include "../mat.hh"
#include "../pos.hh"
#include "../vec.hh"
#include "traits.hh"

#include "aabb.hh"

// An oriented box
// stored as:
//   center position
//   orthogonal matrix that maps the -1..1 cube to the oriented box (half-extents)

namespace tg
{
template <int ObjectD, class ScalarT, int DomainD = ObjectD, class TraitsT = default_object_tag>
struct box;

// Common box types

using box1 = box<1, f32>;
using box2 = box<2, f32>;
using box3 = box<3, f32>;
using box4 = box<4, f32>;

using fbox1 = box<1, f32>;
using fbox2 = box<2, f32>;
using fbox3 = box<3, f32>;
using fbox4 = box<4, f32>;

using dbox1 = box<1, f64>;
using dbox2 = box<2, f64>;
using dbox3 = box<3, f64>;
using dbox4 = box<4, f64>;

using ibox1 = box<1, i32>;
using ibox2 = box<2, i32>;
using ibox3 = box<3, i32>;
using ibox4 = box<4, i32>;

using ubox1 = box<1, u32>;
using ubox2 = box<2, u32>;
using ubox3 = box<3, u32>;
using ubox4 = box<4, u32>;

using box2in3 = box<2, f32, 3>;
using fbox2in3 = box<2, f32, 3>;
using dbox2in3 = box<2, f64, 3>;
using ibox2in3 = box<2, i32, 3>;
using ubox2in3 = box<2, u32, 3>;

template <int ObjectD, class ScalarT, int DomainD = ObjectD>
using box_boundary = box<ObjectD, ScalarT, DomainD, boundary_tag>;


// ======== IMPLEMENTATION ========

template <int D, class ScalarT, class TraitsT>
struct box<D, ScalarT, D, TraitsT>
{
    using scalar_t = ScalarT;
    using vec_t = vec<D, ScalarT>;
    using pos_t = pos<D, ScalarT>;
    using mat_t = mat<D, D, ScalarT>;

    static const box minus_one_to_one;
    static const box unit_from_zero;
    static const box unit_centered;

    pos_t center;
    mat_t half_extents;

    constexpr box() = default;
    constexpr box(pos_t center, mat_t const& half_extents) : center(center), half_extents(half_extents) {}
    constexpr box(aabb<D, ScalarT, TraitsT> const& b); // requires tg.hh

    template <class OtherT, class OtherTraitsT>
    explicit constexpr box(box<D, OtherT, D, OtherTraitsT> const& v) : center(v.center), half_extents(v.half_extents)
    {
    }

    /// Note that the box goes from -1 to 1 instead of the usual 0 to 1
    [[nodiscard]] constexpr pos_t operator[](comp<D, ScalarT> const& c) const;

    [[nodiscard]] bool operator==(box const& rhs) const { return center == rhs.center && half_extents == rhs.half_extents; }
    [[nodiscard]] bool operator!=(box const& rhs) const { return !operator==(rhs); }
};

template <class ScalarT, class TraitsT>
struct box<2, ScalarT, 3, TraitsT>
{
    using scalar_t = ScalarT;
    using vec_t = vec<3, ScalarT>;
    using dir_t = dir<3, ScalarT>;
    using pos_t = pos<3, ScalarT>;
    using mat_t = mat<2, 3, ScalarT>;

    pos_t center;
    mat_t half_extents;

    constexpr box() = default;
    constexpr box(pos_t center, mat_t const& half_extents) : center(center), half_extents(half_extents) {}
    constexpr box(pos_t center, vec_t half_extent_x, vec_t half_extent_y) : center(center), half_extents(half_extent_x, half_extent_y) {}

    template <class OtherT, class OtherTraitsT>
    explicit constexpr box(box<2, OtherT, 3, OtherTraitsT> const& v) : center(v.center), half_extents(v.half_extents)
    {
    }

    /// Note that the box goes from -1 to 1 instead of the usual 0 to 1
    [[nodiscard]] constexpr pos_t operator[](comp<2, ScalarT> const& c) const;

    [[nodiscard]] bool operator==(box const& rhs) const { return center == rhs.center && half_extents == rhs.half_extents; }
    [[nodiscard]] bool operator!=(box const& rhs) const { return !operator==(rhs); }
};

template <class I, int ObjectD, class ScalarT, int DomainD, class TraitsT>
constexpr void introspect(I&& i, box<ObjectD, ScalarT, DomainD, TraitsT>& v)
{
    i(v.center, "center");
    i(v.half_extents, "half_extents");
}

template <int ObjectD, class ScalarT, int DomainD, class TraitsT>
struct object_traits<box<ObjectD, ScalarT, DomainD, TraitsT>> : detail::finite_object_traits<ObjectD, ScalarT, DomainD, TraitsT>
{
};
} // namespace tg
