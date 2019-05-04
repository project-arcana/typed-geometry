#pragma once

#include "../dir.hh"
#include "../pos.hh"

#include "disk.hh"
namespace tg
{
/**
 * right circular cone
 * apex not stored explicitly, can be deduced from height and normal of base
 * (not yet) can construct cone from given apex and center of base (TODO would require extra functions here, do we want that?)
 */
template <int D, class ScalarT>
struct cone;

// Common cone types
using cone3 = cone<3, f32>;
using fcone3 = cone<3, f32>;
using dcone3 = cone<3, f64>;
using icone3 = cone<3, i32>;
using ucone3 = cone<3, u32>;

// ======== IMPLEMENTATION ========
template <class ScalarT>
struct cone<3, ScalarT>
{
    using pos_t = pos<3, ScalarT>;
    using dir_t = dir<3, ScalarT>;
    using disk_t = disk<3, ScalarT>;

    disk_t base;
    ScalarT height;

    constexpr cone() = default;
    constexpr cone(pos_t c, ScalarT r, ScalarT h, dir_t n = dir_t::pos_y) : base(disk_t(c, r, n)), height(h) {}
};
} // namespace tg
