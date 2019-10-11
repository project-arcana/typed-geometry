#pragma once

#include "../angle.hh"
#include "../dir.hh"
#include "../pos.hh"
#include <typed-geometry/types/scalars/default.hh>

namespace tg
{
template <int D, class ScalarT>
struct rect;

// Common rect types

using rect2 = rect<2, f32>;
using rect3 = rect<3, f32>;

using frect2 = rect<2, f32>;
using frect3 = rect<3, f32>;

using drect2 = rect<2, f64>;
using drect3 = rect<3, f64>;

using irect2 = rect<2, i32>;
using irect3 = rect<3, i32>;

using urect2 = rect<2, u32>;
using urect3 = rect<3, u32>;

// ======== IMPLEMENTATION ========

// TODO: define by two axes!

template <class ScalarT>
struct rect<2, ScalarT>
{
    using pos_t = pos<2, ScalarT>;

    pos_t center;
    ScalarT length;
    angle_t<ScalarT> rotation; // about z-axis

    constexpr rect() = default;
    constexpr rect(pos_t c, ScalarT l, angle_t<ScalarT> r) : center(c), length(l), rotation(r){};

    TG_NODISCARD bool operator==(rect const& rhs) const { return center == rhs.center && length == rhs.length && rotation == rhs.rotation; }
    TG_NODISCARD bool operator!=(rect const& rhs) const { return !operator==(rhs); }
};

template <class ScalarT>
struct rect<3, ScalarT>
{
    using dir_t = dir<3, ScalarT>;
    using pos_t = pos<3, ScalarT>;

    pos_t center;
    ScalarT length;
    dir_t normal;

    angle_t<ScalarT> rotation; // about axis defined by normal

    constexpr rect() = default;
    constexpr rect(pos_t c, ScalarT l, dir_t n = dir_t::pos_y, angle_t<ScalarT> r = angle_t<ScalarT>::from_radians(0))
      : center(c), length(l), normal(n), rotation(r)
    {
    }

    TG_NODISCARD bool operator==(rect const& rhs) const
    {
        return center == rhs.center && length == rhs.length && rotation == rhs.rotation && normal == rhs.normal;
    }
    TG_NODISCARD bool operator!=(rect const& rhs) const { return !operator==(rhs); }
};
} // namespace tg
