#pragma once

#include "../pos.hh"
#include <typed-geometry/types/scalars/default.hh>

namespace tg
{
/**
 * Volume bounded by a sphere
 */
template <int D, class ScalarT>
struct ball;

// Common ball types

using ball1 = ball<1, f32>;
using ball2 = ball<2, f32>;
using ball3 = ball<3, f32>;
using ball4 = ball<4, f32>;

using fball1 = ball<1, f32>;
using fball2 = ball<2, f32>;
using fball3 = ball<3, f32>;
using fball4 = ball<4, f32>;

using dball1 = ball<1, f64>;
using dball2 = ball<2, f64>;
using dball3 = ball<3, f64>;
using dball4 = ball<4, f64>;

using iball1 = ball<1, i32>;
using iball2 = ball<2, i32>;
using iball3 = ball<3, i32>;
using iball4 = ball<4, i32>;

using uball1 = ball<1, u32>;
using uball2 = ball<2, u32>;
using uball3 = ball<3, u32>;
using uball4 = ball<4, u32>;


// ======== IMPLEMENTATION ========

template <int D, class ScalarT>
struct sphere;

template <int D, class ScalarT>
struct ball
{
    using scalar_t = ScalarT;
    using pos_t = pos<D, ScalarT>;
    using sphere_t = sphere<D, ScalarT>;

    static const ball unit; ///< unit sphere, center zero, radius 1

    pos_t center;
    scalar_t radius = ScalarT(0);

    constexpr ball() = default;
    constexpr ball(pos_t c, ScalarT r) : center(c), radius(r) {}
    constexpr ball(sphere_t const& s) : center(s.center), radius(s.radius) {}

    TG_NODISCARD bool operator==(ball const& rhs) const { return center == rhs.center && radius == rhs.radius; }
    TG_NODISCARD bool operator!=(ball const& rhs) const { return !operator==(rhs); }
};
} // namespace tg
