#pragma once

#include "../dir.hh"
#include "../pos.hh"

#include "disk.hh"
namespace tg
{
/**
 * capped circular right cylinder
 * pos is center of cylinder
 * height is distance between bases
 * constructable from center points of both caps
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
    using pos_t = pos<3, ScalarT>;
    using dir_t = dir<3, ScalarT>;
    using disk_t = disk<3, ScalarT>;

    pos_t center;
    ScalarT height;
    disk_t base;

    constexpr cylinder() = default;
    constexpr cylinder(pos_t bc, pos_t tc, ScalarT r)
      : height(length(tc - bc)), base(disk_t(bc, r, normalize(tc - bc))), center(bc + height * base.normal)
    {
    }
    constexpr cylinder(pos_t c, ScalarT r, ScalarT h, dir_t n = dir_t::pos_y) : center(c), height(h), base(disk_t(c - h / 2 * n, r, n)) {}
};
} // namespace tg
