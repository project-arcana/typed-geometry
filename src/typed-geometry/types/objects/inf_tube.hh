#pragma once

#include "../dir.hh"
#include "../pos.hh"
#include "line.hh"

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
    using line_t = line<D, ScalarT>;

    line_t axis;
    ScalarT radius = ScalarT(0);

    constexpr inf_tube() = default;
    constexpr inf_tube(line_t const& axis, ScalarT radius) : axis(axis), radius(radius) {}

    TG_NODISCARD bool operator==(inf_tube const& rhs) const { return axis == rhs.axis && radius == rhs.radius; }
    TG_NODISCARD bool operator!=(inf_tube const& rhs) const { return !operator==(rhs); }
};
} // namespace tg
