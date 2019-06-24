#pragma once

#include "../dir.hh"
#include "../pos.hh"

#include "segment.hh"

namespace tg
{
/**
 * capped circular right hollow cylinder (no caps)
 *
 * A tube is defined by a segment and a radius
 */
template <int D, class ScalarT>
struct tube;

// Common tube types
using tube3 = tube<3, f32>;
using ftube3 = tube<3, f32>;
using dtube3 = tube<3, f64>;
using itube3 = tube<3, i32>;
using utube3 = tube<3, u32>;

// ======== IMPLEMENTATION ========
template <class ScalarT>
struct tube<3, ScalarT>
{
    using scalar_t = ScalarT;
    using pos_t = pos<3, ScalarT>;
    using dir_t = dir<3, ScalarT>;
    using seg_t = segment<3, ScalarT>;

    seg_t axis;
    scalar_t radius = ScalarT(0);

    constexpr tube() = default;
    constexpr tube(seg_t const& axis, scalar_t radius) : axis(axis), radius(radius) {}
    constexpr tube(pos_t const& p0, pos_t const& p1, scalar_t radius) : axis(p0, p1), radius(radius) {}

    TG_NODISCARD bool operator==(tube const& rhs) const { return axis == rhs.axis && radius == rhs.radius; }
    TG_NODISCARD bool operator!=(tube const& rhs) const { return !operator==(rhs); }
};
} // namespace tg
