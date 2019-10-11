#pragma once

#include "../pos.hh"
#include <typed-geometry/types/scalars/default.hh>
#include "../size.hh"
#include "../dir.hh"

namespace tg
{
template <int D, class ScalarT>
struct ray;

// Common ray types

using ray1 = ray<1, f32>;
using ray2 = ray<2, f32>;
using ray3 = ray<3, f32>;
using ray4 = ray<4, f32>;

using fray1 = ray<1, f32>;
using fray2 = ray<2, f32>;
using fray3 = ray<3, f32>;
using fray4 = ray<4, f32>;

using dray1 = ray<1, f64>;
using dray2 = ray<2, f64>;
using dray3 = ray<3, f64>;
using dray4 = ray<4, f64>;

using iray1 = ray<1, i32>;
using iray2 = ray<2, i32>;
using iray3 = ray<3, i32>;
using iray4 = ray<4, i32>;

using uray1 = ray<1, u32>;
using uray2 = ray<2, u32>;
using uray3 = ray<3, u32>;
using uray4 = ray<4, u32>;


// ======== IMPLEMENTATION ========

template <int D, class ScalarT>
struct ray
{
    using dir_t = tg::dir<D, ScalarT>;
    using pos_t = tg::pos<D, ScalarT>;

    pos_t origin;
    dir_t dir;

    constexpr ray() = default;
    constexpr ray(pos_t origin, dir_t dir) : origin(origin), dir(dir) {}

    TG_NODISCARD constexpr pos_t operator[](ScalarT t) const;

    TG_NODISCARD bool operator==(ray const& rhs) const { return origin == rhs.origin && dir == rhs.dir; }
    TG_NODISCARD bool operator!=(ray const& rhs) const { return !operator==(rhs); }
};
} // namespace tg
