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
    using dir_tc = dir<3, ScalarT>;
    using pos_tc = pos<3, ScalarT>;
    using angle_tc = angle_t<ScalarT>;

    pos_tc apex;
    dir_tc opening_dir;
    angle_tc opening_angle;

    constexpr infcone() = default;
    constexpr infcone(pos_tc const& apex, dir_tc const& dir, angle_tc const& opening_angle) : apex(apex), opening_dir(dir), opening_angle(opening_angle) {}
};
} // namespace tg
