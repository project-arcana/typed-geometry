#pragma once

#include "../dir.hh"
#include "../pos.hh"

#include "segment.hh"

namespace tg
{
/**
 * capped circular right cylinder
 *
 * A cylinder is defined by a segment and a radius
 */
template <int D, class ScalarT>
struct cylinder;

// Common cylinder types
using cylinder3 = cylinder<3, f32>;
using fcylinder3 = cylinder<3, f32>;
using dcylinder3 = cylinder<3, f64>;
using icylinder3 = cylinder<3, i32>;
using ucylinder3 = cylinder<3, u32>;

// ======== IMPLEMENTATION ========
template <class ScalarT>
struct cylinder<3, ScalarT>
{
    using scalar_t = ScalarT;
    using pos_t = pos<3, ScalarT>;
    using dir_t = dir<3, ScalarT>;
    using seg_t = segment<3, ScalarT>;

    seg_t axis;
    scalar_t radius = ScalarT(0);

    constexpr cylinder() = default;
    constexpr cylinder(seg_t const& axis, scalar_t radius) : axis(axis), radius(radius) {}
    constexpr cylinder(pos_t const& p0, pos_t const& p1, scalar_t radius) : axis(p0, p1), radius(radius) {}

    TG_NODISCARD bool operator==(cylinder const& rhs) const { return axis == rhs.axis && radius == rhs.radius; }
    TG_NODISCARD bool operator!=(cylinder const& rhs) const { return !operator==(rhs); }
};
} // namespace tg
