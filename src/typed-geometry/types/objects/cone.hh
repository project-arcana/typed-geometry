#pragma once

#include "../dir.hh"

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
    using scalar_t = ScalarT;
    using dir_t = dir<3, ScalarT>;
    using disk_t = disk<3, ScalarT>;

    disk_t base;
    scalar_t height;

    constexpr cone() = default;
    constexpr cone(disk_t const& base, scalar_t height) : base(base), height(height) {}

    TG_NODISCARD bool operator==(cone const& rhs) const { return base == rhs.base && height == rhs.height; }
    TG_NODISCARD bool operator!=(cone const& rhs) const { return !operator==(rhs); }
};
} // namespace tg
