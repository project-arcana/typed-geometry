#pragma once

#include "../dir.hh"
#include "../pos.hh"

#include "square.hh"
namespace tg
{
/**
 * 3D square based right pyramids
 *
 * pyramid is constructed from center of square (base), length of square sides and height
 * optional: normal of base and rotation around normal
 *
 * TODO (not yet) allow construction of pyramid from given apex and center of base? (would require extra functions)
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
    using pos_t = pos<3, ScalarT>;
    using dir_t = dir<3, ScalarT>;
    using square_t = square<3, ScalarT>;

    ScalarT height;
    square_t base;

    constexpr pyramid() = default;
    // constexpr pyramid(pos_t c, pos_t apex, ScalarT l) : ..
    constexpr pyramid(pos_t c, ScalarT l, ScalarT h, dir_t n = dir_t::pos_y, angle<ScalarT> r = 0) : base(square_t(c, l, n, r)), height(h) {}
};
} // namespace tg
