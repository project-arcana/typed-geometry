#pragma once

#include "../pos.hh"
#include <typed-geometry/types/scalars/default.hh>
#include "../dir.hh"

// A halfspace has a distance to the origin and a normal direction
// In dimension n, the distance is 1-dimensional and the normal direction is n-dimensional

// Note that there is a semantic difference between hyperplane and halfspace in nD:
//  - the hyperplane describes all points lying on an (n-1)-dimensional plane
//  - the halfspace describes all points lying inside this n-dimensional subspace
namespace tg
{
template <int D, class ScalarT>
struct halfspace;

// Common halfspace types

using halfspace1 = halfspace<1, f32>;
using halfspace2 = halfspace<2, f32>;
using halfspace3 = halfspace<3, f32>;
using halfspace4 = halfspace<4, f32>;

using fhalfspace1 = halfspace<1, f32>;
using fhalfspace2 = halfspace<2, f32>;
using fhalfspace3 = halfspace<3, f32>;
using fhalfspace4 = halfspace<4, f32>;

using dhalfspace1 = halfspace<1, f64>;
using dhalfspace2 = halfspace<2, f64>;
using dhalfspace3 = halfspace<3, f64>;
using dhalfspace4 = halfspace<4, f64>;

using ihalfspace1 = halfspace<1, i32>;
using ihalfspace2 = halfspace<2, i32>;
using ihalfspace3 = halfspace<3, i32>;
using ihalfspace4 = halfspace<4, i32>;

using uhalfspace1 = halfspace<1, u32>;
using uhalfspace2 = halfspace<2, u32>;
using uhalfspace3 = halfspace<3, u32>;
using uhalfspace4 = halfspace<4, u32>;


// ======== IMPLEMENTATION ========

template <int D, class ScalarT>
struct halfspace
{
    using scalar_t = ScalarT;
    using dir_t = dir<D, ScalarT>;
    using pos_t = pos<D, ScalarT>;

    dir_t normal; //< points _away_ from the halfspace
    scalar_t dis;

    constexpr halfspace() = default;
    constexpr halfspace(dir_t n, scalar_t d) : normal(n), dis(d) {}
    constexpr halfspace(dir_t n, pos_t p);

    TG_NODISCARD bool operator==(halfspace const& rhs) const { return normal == rhs.normal && dis == rhs.dis; }
    TG_NODISCARD bool operator!=(halfspace const& rhs) const { return !operator==(rhs); }
};
} // namespace tg
