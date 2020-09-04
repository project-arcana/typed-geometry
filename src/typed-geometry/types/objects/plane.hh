#pragma once

#include <typed-geometry/types/scalars/default.hh>
#include "../dir.hh"
#include "../pos.hh"
#include "traits.hh"

// An unoriented plane has a distance to the origin and a normal direction
// In dimension n, the distance is 1-dimensional and the normal direction is n-dimensional

// Note that there is a semantic difference between plane and halfspace in nD:
//  - the plane describes all points lying on an (n-1)-dimensional plane
//  - the halfspace describes all points lying inside this n-dimensional subspace
namespace tg
{
template <int D, class ScalarT>
struct plane;

// Common plane types

using plane1 = plane<1, f32>;
using plane2 = plane<2, f32>;
using plane3 = plane<3, f32>;
using plane4 = plane<4, f32>;

using fplane1 = plane<1, f32>;
using fplane2 = plane<2, f32>;
using fplane3 = plane<3, f32>;
using fplane4 = plane<4, f32>;

using dplane1 = plane<1, f64>;
using dplane2 = plane<2, f64>;
using dplane3 = plane<3, f64>;
using dplane4 = plane<4, f64>;

using iplane1 = plane<1, i32>;
using iplane2 = plane<2, i32>;
using iplane3 = plane<3, i32>;
using iplane4 = plane<4, i32>;

using uplane1 = plane<1, u32>;
using uplane2 = plane<2, u32>;
using uplane3 = plane<3, u32>;
using uplane4 = plane<4, u32>;


// ======== IMPLEMENTATION ========

template <int D, class ScalarT>
struct plane
{
    using scalar_t = ScalarT;
    using dir_t = dir<D, ScalarT>;
    using pos_t = pos<D, ScalarT>;

    // points x on the plane satisfy:
    // <x, normal> = dis

    dir_t normal;
    scalar_t dis; // NOTE: this is not the "d" from the plane equation, but -d

    constexpr plane() = default;
    constexpr plane(dir_t n, scalar_t d) : normal(n), dis(d) {}
    constexpr plane(dir_t n, pos_t p);

    template <class OtherT>
    explicit constexpr plane(plane<D, OtherT> const& v) : normal(v.normal), dis(v.dis)
    {
    }

    [[nodiscard]] bool operator==(plane const& rhs) const { return normal == rhs.normal && dis == rhs.dis; }
    [[nodiscard]] bool operator!=(plane const& rhs) const { return !operator==(rhs); }
};

template <class I, int D, class ScalarT>
constexpr void introspect(I&& i, plane<D, ScalarT>& v)
{
    i(v.normal, "normal");
    i(v.dis, "dis");
}

template <int D, class ScalarT>
struct object_traits<plane<D, ScalarT>> : detail::infinite_object_traits<D - 1, ScalarT, D, default_object_tag>
{
};
} // namespace tg
