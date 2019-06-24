#pragma once

#include "../angle.hh"
#include "../dir.hh"
#include "../pos.hh"

namespace tg
{
/**
 * right circular infinite cone
 */
template <int D, class ScalarT>
struct inf_cone;

// Common cone types
using inf_cone3 = inf_cone<3, f32>;
using finf_cone3 = inf_cone<3, f32>;
using dinf_cone3 = inf_cone<3, f64>;
using iinf_cone3 = inf_cone<3, i32>;
using uinf_cone3 = inf_cone<3, u32>;

// ======== IMPLEMENTATION ========
template <int D, class ScalarT>
struct inf_cone
{
    using dir_t = dir<D, ScalarT>;
    using pos_t = pos<D, ScalarT>;

    pos_t apex;
    dir_t opening_dir;
    angle_t<ScalarT> opening_angle;

    constexpr inf_cone() = default;
    constexpr inf_cone(pos_t const& apex, dir_t const& dir, angle_t<ScalarT> const& opening_angle)
      : apex(apex), opening_dir(dir), opening_angle(opening_angle)
    {
    }

    TG_NODISCARD bool operator==(inf_cone const& rhs) const { return apex == rhs.apex && opening_dir == rhs.opening_dir && opening_angle == rhs.opening_angle; }
    TG_NODISCARD bool operator!=(inf_cone const& rhs) const { return !operator==(rhs); }
};
} // namespace tg
