#pragma once

#include "../pos.hh"

#include <typed-geometry/types/scalars/default.hh>

namespace tg
{
/**
 * A ObjectD-dimensional sphere embedded in a DomainD space
 */
template <int ObjectD, class ScalarT, int DomainD = ObjectD>
struct sphere;

// Common sphere types

using sphere1 = sphere<1, f32>;
using sphere2 = sphere<2, f32>;
using sphere3 = sphere<3, f32>;
using sphere4 = sphere<4, f32>;

using fsphere1 = sphere<1, f32>;
using fsphere2 = sphere<2, f32>;
using fsphere3 = sphere<3, f32>;
using fsphere4 = sphere<4, f32>;

using dsphere1 = sphere<1, f64>;
using dsphere2 = sphere<2, f64>;
using dsphere3 = sphere<3, f64>;
using dsphere4 = sphere<4, f64>;

using isphere1 = sphere<1, i32>;
using isphere2 = sphere<2, i32>;
using isphere3 = sphere<3, i32>;
using isphere4 = sphere<4, i32>;

using usphere1 = sphere<1, u32>;
using usphere2 = sphere<2, u32>;
using usphere3 = sphere<3, u32>;
using usphere4 = sphere<4, u32>;

// 2D disks embedded in 3D
using sphere2in3 = sphere<2, f32, 3>;
using fsphere2in3 = sphere<2, f32, 3>;
using dsphere2in3 = sphere<2, f64, 3>;
using isphere2in3 = sphere<2, i32, 3>;
using usphere2in3 = sphere<2, u32, 3>;


// ======== IMPLEMENTATION ========

template <int D, class ScalarT>
struct sphere<D, ScalarT, D>
{
    using scalar_t = ScalarT;
    using pos_t = pos<D, ScalarT>;

    static const sphere unit; ///< unit sphere, center zero, radius 1

    pos_t center;
    scalar_t radius = ScalarT(0);

    constexpr sphere() = default;
    constexpr sphere(pos_t c, ScalarT r) : center(c), radius(r) {}

    [[nodiscard]] bool operator==(sphere const& rhs) const { return center == rhs.center && radius == rhs.radius; }
    [[nodiscard]] bool operator!=(sphere const& rhs) const { return !operator==(rhs); }
};

template <class ScalarT>
struct sphere<2, ScalarT, 3>
{
    using scalar_t = ScalarT;
    using pos_t = pos<3, ScalarT>;
    using dir_t = dir<3, ScalarT>;

    pos_t center;
    scalar_t radius;
    dir_t normal;

    constexpr sphere() = default;
    constexpr sphere(pos_t c, ScalarT r, dir_t n) : center(c), radius(r), normal(n) {}

    [[nodiscard]] bool operator==(sphere const& rhs) const { return center == rhs.center && radius == rhs.radius && normal == rhs.normal; }
    [[nodiscard]] bool operator!=(sphere const& rhs) const { return !operator==(rhs); }
};
} // namespace tg
