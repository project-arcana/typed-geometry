#pragma once

#include "../dir.hh"
#include "../pos.hh"

namespace tg
{
/**
 * infinite tube
 *
 * An infinite tube is defined by a position and an axis
 */
template <int D, class ScalarT>
struct inf_tube;

// Common infinte tube types
using inf_tube3 = inf_tube<3, f32>;
using finttube3 = inf_tube<3, f32>;
using dinf_tube3 = inf_tube<3, f64>;
using iinf_tube3 = inf_tube<3, i32>;
using uinf_tube3 = inf_tube<3, u32>;

// ======== IMPLEMENTATION ========
template <int D, class ScalarT>
struct inf_tube
{
    using pos_t = pos<D, ScalarT>;
    using dir_t = dir<D, ScalarT>;

    pos_t center;
    dir_t axis;
    ScalarT radius = ScalarT(0);

    constexpr inf_tube() = default;
    constexpr inf_tube(pos_t const& center, dir_t const& axis, ScalarT radius) : center(center), axis(axis), radius(radius) {}
};
} // namespace tg
