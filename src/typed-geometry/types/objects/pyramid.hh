#pragma once

#include "../dir.hh"
#include "../pos.hh"

#include "rect.hh"

namespace tg
{
/**
 * 3D rect based right pyramids
 *
 * pyramid is constructed from a rect (base) and height
 */
template <int D, class ScalarT>
struct pyramid;

// Common pyramid types
using pyramid3 = pyramid<3, f32>;
using fpyramid3 = pyramid<3, f32>;
using dpyramid3 = pyramid<3, f64>;
using ipyramid3 = pyramid<3, i32>;
using upyramid3 = pyramid<3, u32>;

// ======== IMPLEMENTATION ========
template <class ScalarT>
struct pyramid<3, ScalarT>
{
    using scalar_t = ScalarT;
    using pos_t = pos<3, ScalarT>;
    using dir_t = dir<3, ScalarT>;
    using rect_t = rect<3, ScalarT>;

    rect_t base;
    scalar_t height = 0;

    constexpr pyramid() = default;
    constexpr pyramid(rect_t const& base, scalar_t height) : base(base), height(height) {}

    TG_NODISCARD bool operator==(pyramid const& rhs) const { return base == rhs.base && height == rhs.height; }
    TG_NODISCARD bool operator!=(pyramid const& rhs) const { return !operator==(rhs); }
};
} // namespace tg
