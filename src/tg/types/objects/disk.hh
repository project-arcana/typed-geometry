#pragma once

#include "../pos.hh"
#include "../vec.hh"

namespace tg
{
/**
 * 2D circular area enclosed by a circle
 *
 * a disk3 is a disk living on a plane in 3D
 */
template <int D, class ScalarT>
struct disk;

// Common disk types

using disk2 = disk<2, f32>;
using disk3 = disk<3, f32>;

using fdisk2 = disk<2, f32>;
using fdisk3 = disk<3, f32>;

using ddisk2 = disk<2, f64>;
using ddisk3 = disk<3, f64>;

using idisk2 = disk<2, i32>;
using idisk3 = disk<3, i32>;

using udisk2 = disk<2, u32>;
using udisk3 = disk<3, u32>;


// ======== IMPLEMENTATION ========

template <class ScalarT>
struct disk<2, ScalarT>
{
    using pos_t = pos<2, ScalarT>;
    using vec_t = vec<2, ScalarT>;

    pos_t center;
    ScalarT radius;

    constexpr disk() = default;
    constexpr disk(pos_t c, ScalarT r) : center(c), radius(r) {}
};

template <class ScalarT>
struct disk<3, ScalarT>
{
    using pos_t = pos<3, ScalarT>;
    using vec_t = vec<3, ScalarT>;

    pos_t center;
    ScalarT radius;
    vec_t normal;

    constexpr disk() = default;
    constexpr disk(pos_t c, ScalarT r, vec_t n) : center(c), radius(r), normal(n) {}
};
} // namespace tg
