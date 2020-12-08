#pragma once

#include "../angle.hh"
#include "../dir.hh"
#include "../pos.hh"
#include "traits.hh"

#include "cone.hh"

namespace tg
{
/**
 * right circular infinite cone
 */
template <int D, class ScalarT, class TraitsT = default_object_tag>
struct inf_cone;

// Common cone types
using inf_cone2 = inf_cone<2, f32>;
using finf_cone2 = inf_cone<2, f32>;
using dinf_cone2 = inf_cone<2, f64>;
using iinf_cone2 = inf_cone<2, i32>;
using uinf_cone2 = inf_cone<2, u32>;

using inf_cone3 = inf_cone<3, f32>;
using finf_cone3 = inf_cone<3, f32>;
using dinf_cone3 = inf_cone<3, f64>;
using iinf_cone3 = inf_cone<3, i32>;
using uinf_cone3 = inf_cone<3, u32>;

template <int D, class ScalarT>
using inf_cone_boundary = inf_cone<D, ScalarT, boundary_tag>;


// ======== IMPLEMENTATION ========
template <int D, class ScalarT, class TraitsT>
struct inf_cone
{
    using dir_t = dir<D, ScalarT>;
    using pos_t = pos<D, ScalarT>;

    pos_t apex;
    dir_t opening_dir;
    angle_t<ScalarT> opening_angle;

    constexpr inf_cone() = default;
    constexpr inf_cone(pos_t const& apex, dir_t const& dir, angle_t<ScalarT> const& opening_angle)
      : apex(apex), opening_dir(dir), opening_angle(opening_angle)
    {
    }

    template <class OtherT, class OtherTraitsT>
    explicit constexpr inf_cone(inf_cone<D, OtherT, OtherTraitsT> const& v) : apex(v.apex), opening_dir(v.opening_dir), opening_angle(v.opening_angle)
    {
    }

    [[nodiscard]] bool operator==(inf_cone const& rhs) const
    {
        return apex == rhs.apex && opening_dir == rhs.opening_dir && opening_angle == rhs.opening_angle;
    }
    [[nodiscard]] bool operator!=(inf_cone const& rhs) const { return !operator==(rhs); }
};

template <class I, int D, class ScalarT, class TraitsT>
constexpr void introspect(I&& i, inf_cone<D, ScalarT, TraitsT>& v)
{
    i(v.apex, "apex");
    i(v.opening_dir, "opening_dir");
    i(v.opening_angle, "opening_angle");
}

template <int D, class ScalarT, class TraitsT>
struct object_traits<inf_cone<D, ScalarT, TraitsT>> : detail::infinite_object_traits<D, ScalarT, D, TraitsT>
{
};
} // namespace tg
