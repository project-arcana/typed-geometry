#pragma once

#include "../dir.hh"
#include "../pos.hh"
#include "../angle.hh"

namespace tg
{
/**
 * right circular infinite cone
 */
template <int D, class ScalarT>
struct infcone;

// Common cone types
using infcone3 = infcone<3, f32>;
using finfcone3 = infcone<3, f32>;
using dinfcone3 = infcone<3, f64>;
using iinfcone3 = infcone<3, i32>;
using uinfcone3 = infcone<3, u32>;

// ======== IMPLEMENTATION ========
template <class ScalarT>
struct infcone<3, ScalarT>
{
    using dir_t = dir<3, ScalarT>;
    using pos_t = pos<3, ScalarT>;
    using angle_t = angle_t<ScalarT>;

    pos_t apex;
    dir_t dir;
    angle_t opening_angle;

    constexpr infcone() = default;
    constexpr infcone(pos_t const& apex, dir_t const& dir, angle_t const& opening_angle) : apex(apex), dir(dir), opening_angle(opening_angle) {}
};
} // namespace tg
