#pragma once

#include "../pos.hh"
#include "../dir.hh"

namespace tg
{
/**
 * 1D line that surrounds a disk
 *
 * a circle3 is a circle living on a plane in 3D
 */
template <int D, class ScalarT>
struct circle;

// Common circle types

using circle2 = circle<2, f32>;
using circle3 = circle<3, f32>;

using fcircle2 = circle<2, f32>;
using fcircle3 = circle<3, f32>;

using dcircle2 = circle<2, f64>;
using dcircle3 = circle<3, f64>;

using icircle2 = circle<2, i32>;
using icircle3 = circle<3, i32>;

using ucircle2 = circle<2, u32>;
using ucircle3 = circle<3, u32>;


// ======== IMPLEMENTATION ========

template <class ScalarT>
struct circle<2, ScalarT>
{
    using scalar_t = ScalarT;
    using pos_t = pos<2, ScalarT>;

    pos_t center;
    scalar_t radius = ScalarT(0);

    constexpr circle() = default;
    constexpr circle(pos_t c, ScalarT r) : center(c), radius(r) {}

    TG_NODISCARD bool operator==(circle const& rhs) const { return center == rhs.center && radius == rhs.radius; }
    TG_NODISCARD bool operator!=(circle const& rhs) const { return !operator==(rhs); }
};

template <class ScalarT>
struct circle<3, ScalarT>
{
    using scalar_t = ScalarT;
    using pos_t = pos<3, ScalarT>;
    using dir_t = dir<3, ScalarT>;

    pos_t center;
    scalar_t radius;
    dir_t normal;

    constexpr circle() = default;
    constexpr circle(pos_t c, ScalarT r, dir_t n) : center(c), radius(r), normal(n) {}

    TG_NODISCARD bool operator==(circle const& rhs) const { return center == rhs.center && radius == rhs.radius && normal == rhs.normal; }
    TG_NODISCARD bool operator!=(circle const& rhs) const { return !operator==(rhs); }
};
} // namespace tg
