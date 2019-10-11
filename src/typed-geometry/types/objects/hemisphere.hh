#pragma once

#include "../dir.hh"
#include "../pos.hh"
#include <typed-geometry/types/scalars/default.hh>

namespace tg
{
/**
 * Half of the surface of a completely round ball
 * (which half is given by direction vector)
 */
template <int D, class ScalarT>
struct hemisphere;

// Common hemisphere types

using hemisphere1 = hemisphere<1, f32>;
using hemisphere2 = hemisphere<2, f32>;
using hemisphere3 = hemisphere<3, f32>;
using hemisphere4 = hemisphere<4, f32>;

using fhemisphere1 = hemisphere<1, f32>;
using fhemisphere2 = hemisphere<2, f32>;
using fhemisphere3 = hemisphere<3, f32>;
using fhemisphere4 = hemisphere<4, f32>;

using dhemisphere1 = hemisphere<1, f64>;
using dhemisphere2 = hemisphere<2, f64>;
using dhemisphere3 = hemisphere<3, f64>;
using dhemisphere4 = hemisphere<4, f64>;

using ihemisphere1 = hemisphere<1, i32>;
using ihemisphere2 = hemisphere<2, i32>;
using ihemisphere3 = hemisphere<3, i32>;
using ihemisphere4 = hemisphere<4, i32>;

using uhemisphere1 = hemisphere<1, u32>;
using uhemisphere2 = hemisphere<2, u32>;
using uhemisphere3 = hemisphere<3, u32>;
using uhemisphere4 = hemisphere<4, u32>;


// ======== IMPLEMENTATION ========

template <int D, class ScalarT>
struct hemisphere
{
    using scalar_t = ScalarT;
    using pos_t = pos<D, ScalarT>;
    using dir_t = dir<D, ScalarT>;

    pos_t center;
    scalar_t radius;
    dir_t normal;

    constexpr hemisphere() = default;
    constexpr hemisphere(pos_t const& c, ScalarT r, dir_t const& normal) : center(c), radius(r), normal(normal) {}

    TG_NODISCARD bool operator==(hemisphere const& rhs) const { return center == rhs.center && radius == rhs.radius && normal == rhs.normal; }
    TG_NODISCARD bool operator!=(hemisphere const& rhs) const { return !operator==(rhs); }
};
} // namespace tg
