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
struct inftube;

// Common infinte tube types
using inftube3 = inftube<3, f32>;
using finttube3 = inftube<3, f32>;
using dinftube3 = inftube<3, f64>;
using iinftube3 = inftube<3, i32>;
using uinftube3 = inftube<3, u32>;

// ======== IMPLEMENTATION ========
template <int D, class ScalarT>
struct inftube
{
    using pos_t = pos<D, ScalarT>;
    using dir_t = dir<D, ScalarT>;

    pos_t center;
    dir_t axis;
    ScalarT radius = ScalarT(0);

    constexpr inftube() = default;
    constexpr inftube(pos_t const& center, dir_t const& axis, ScalarT radius) : center(center), axis(axis), radius(radius) {}
};
} // namespace tg
