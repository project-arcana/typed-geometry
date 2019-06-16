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
template <int D, class ScalarT>
struct infcone
{
    using dir_t = dir<D, ScalarT>;
    using pos_t = pos<D, ScalarT>;

    pos_t apex;
    dir_t opening_dir;
    angle_t<ScalarT> opening_angle;

    constexpr infcone() = default;
    constexpr infcone(pos_t const& apex, dir_t const& dir, angle_t<ScalarT> const& opening_angle) : apex(apex), opening_dir(dir), opening_angle(opening_angle) {}
};
} // namespace tg
