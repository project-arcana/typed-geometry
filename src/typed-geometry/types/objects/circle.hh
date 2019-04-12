#pragma once

#include "../pos.hh"
#include "../vec.hh"

namespace tg
{
/**
 * 1D line that surrounds a disk
 *
 * a circle3 is a circle living on a plane in 3D
 */
template <int D, class ScalarT>
struct circle;

// Common circle types

using circle2 = circle<2, f32>;
using circle3 = circle<3, f32>;

using fcircle2 = circle<2, f32>;
using fcircle3 = circle<3, f32>;

using dcircle2 = circle<2, f64>;
using dcircle3 = circle<3, f64>;

using icircle2 = circle<2, i32>;
using icircle3 = circle<3, i32>;

using ucircle2 = circle<2, u32>;
using ucircle3 = circle<3, u32>;


// ======== IMPLEMENTATION ========

template <class ScalarT>
struct circle<2, ScalarT>
{
    using pos_t = pos<2, ScalarT>;
    using vec_t = vec<2, ScalarT>;

    pos_t center;
    ScalarT radius;

    constexpr circle() = default;
    constexpr circle(pos_t c, ScalarT r) : center(c), radius(r) {}
};

template <class ScalarT>
struct circle<3, ScalarT>
{
    using pos_t = pos<3, ScalarT>;
    using vec_t = vec<3, ScalarT>;

    pos_t center;
    ScalarT radius;
    vec_t normal;

    constexpr circle() = default;
    constexpr circle(pos_t c, ScalarT r, vec_t n) : center(c), radius(r), normal(n) {}
};
} // namespace tg
