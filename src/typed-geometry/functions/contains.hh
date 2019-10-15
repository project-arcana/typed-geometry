#pragma once

#include <cmath>

#include <typed-geometry/types/objects/aabb.hh>
#include <typed-geometry/types/objects/ball.hh>
#include <typed-geometry/types/objects/box.hh>
#include <typed-geometry/types/objects/cylinder.hh>
#include <typed-geometry/types/objects/sphere.hh>
#include <typed-geometry/types/objects/triangle.hh>

#include "distance.hh"
#include "transpose.hh"

// For a given primitive and a position, return whether the first contains the latter
// contains(a, b) is true iff a contains b

// NOTE:
// contains takes an epsilon parameter that roughly means:
//   contains(a, b) if distance(a, p) <= eps
//   NOTE: some functions interpret eps differently for performance reasons
//         if you really want an epsilon environment, use distance(a, p) <= eps

// Contained functions:
// - contains
//      - baabb
//      - triangle

namespace tg
{
// default implementation if distance(a, pos) is available
template <class A, int D, class ScalarT>
TG_NODISCARD constexpr auto contains(A const& a, pos<D, ScalarT> const& p, ScalarT eps = ScalarT(0)) -> decltype(ScalarT(distance(a, p)), false)
{
    return distance(a, p) <= eps;
}

template <int D, class ScalarT>
TG_NODISCARD constexpr bool contains(pos<D, ScalarT> const& b, pos<D, ScalarT> const& o, ScalarT eps = ScalarT(0))
{
    if (eps > 0)
        return distance_sqr(b, o) < eps * eps;
    return b == o;
}

template <class ScalarT>
TG_NODISCARD constexpr bool contains(aabb<1, ScalarT> const& b, ScalarT const& o, ScalarT eps = ScalarT(0))
{
    return b.min.x - eps <= o && o <= b.max.x + eps;
}
template <class ScalarT>
TG_NODISCARD constexpr bool contains(aabb<1, ScalarT> const& b, pos<1, ScalarT> const& o, ScalarT eps = ScalarT(0))
{
    return b.min.x - eps <= o.x && o.x <= b.max.x + eps;
}
template <class ScalarT>
TG_NODISCARD constexpr bool contains(aabb<2, ScalarT> const& b, pos<2, ScalarT> const& o, ScalarT eps = ScalarT(0))
{
    return b.min.x - eps <= o.x && o.x <= b.max.x + eps && //
           b.min.y - eps <= o.y && o.y <= b.max.y + eps;
}
template <class ScalarT>
TG_NODISCARD constexpr bool contains(aabb<3, ScalarT> const& b, pos<3, ScalarT> const& o, ScalarT eps = ScalarT(0))
{
    return b.min.x - eps <= o.x && o.x <= b.max.x + eps && //
           b.min.y - eps <= o.y && o.y <= b.max.y + eps && //
           b.min.z - eps <= o.z && o.z <= b.max.z + eps;
}
template <class ScalarT>
TG_NODISCARD constexpr bool contains(aabb<4, ScalarT> const& b, pos<4, ScalarT> const& o, ScalarT eps = ScalarT(0))
{
    return b.min.x - eps <= o.x && o.x <= b.max.x + eps && //
           b.min.y - eps <= o.y && o.y <= b.max.y + eps && //
           b.min.z - eps <= o.z && o.z <= b.max.z + eps && //
           b.min.w - eps <= o.w && o.w <= b.max.w + eps;
}

template <int D, class ScalarT>
TG_NODISCARD constexpr bool contains(box<D, ScalarT> const& b, pos<D, ScalarT> const& o, ScalarT eps = ScalarT(0))
{
    auto r = o - b.center;
    // TODO: unroll
    for (auto i = 0; i < D; ++i)
        if (abs(dot(b.half_extents[i], r)) > length_sqr(b.half_extents[i]) + eps)
            return false;
    return true;
}

template <int D, class ScalarT>
TG_NODISCARD constexpr bool contains(aabb<D, ScalarT> const& b, aabb<D, ScalarT> const& o, ScalarT eps = ScalarT(0))
{
    return contains(b, o.min, eps) && contains(b, o.max, eps);
}

template <int D, class ScalarT>
TG_NODISCARD constexpr bool contains(ball<D, ScalarT> const& s, pos<D, ScalarT> const& p, ScalarT eps = ScalarT(0))
{
    auto r = s.radius + eps;
    return distance_sqr(s.center, p) <= r * r;
}

template <int D, class ScalarT>
TG_NODISCARD constexpr bool contains(sphere<D, ScalarT> const& s, pos<D, ScalarT> const& p, ScalarT eps = ScalarT(0))
{
    auto const d2 = distance_sqr(s.center, p);
    auto const s2 = s.radius * s.radius;
    auto const e2 = eps * eps;
    return d2 - e2 <= s2 && s2 <= d2 + e2;
}

// Note that eps is used to compare 2D areas, not 1D lengths
template <class ScalarT>
TG_NODISCARD constexpr bool contains(triangle<2, ScalarT> const& t, pos<2, ScalarT> const& p, ScalarT eps = ScalarT(0))
{
    auto pv0 = t.pos0 - p;
    auto pv1 = t.pos1 - p;
    auto pv2 = t.pos2 - p;
    auto A0 = cross(pv1, pv2);
    auto A1 = cross(pv2, pv0);
    auto A2 = cross(pv0, pv1);

    if (eps > 0)
        return ((A0 >= -std::copysign(eps, A0)) == (A1 >= -std::copysign(eps, A0))) && //
               ((A1 >= -std::copysign(eps, A0)) == (A2 >= -std::copysign(eps, A0)));

    return ((A0 >= -eps) == (A1 >= -eps)) && ((A1 >= -eps) == (A2 >= -eps));
}

template <class ScalarT>
TG_NODISCARD constexpr bool contains(triangle<3, ScalarT> const& t, pos<3, ScalarT> const& p)
{
    // TODO
    // use eps?
    // does this also work for triangles where vertices are not ordered cc? should it?

    auto n = normal(t);

    // checking whether point lies on right side of any edge
    auto e = t.pos1 - t.pos0;
    auto C = cross(e, p - t.pos0);
    if (dot(n, C) < 0)
        return false;

    e = t.pos2 - t.pos1;
    C = cross(e, p - t.pos1);
    if (dot(n, C) < 0)
        return false;

    e = t.pos0 - t.pos2;
    C = cross(e, p - t.pos2);
    if (dot(n, C) < 0)
        return false;

    // point always on left side
    return true;
}

template <class ScalarT>
TG_NODISCARD constexpr bool contains(cylinder<3, ScalarT> const& c, pos<3, ScalarT> const& p)
{
    // https://www.flipcode.com/archives/Fast_Point-In-Cylinder_Test.shtml

    // check whether p is between disks
    auto pd = p - c.axis.pos0;
    auto ad = c.axis.pos1 - c.axis.pos0;
    auto d0 = dot(pd, ad);

    auto hsqd = length_sqr(ad);
    auto rsqd = pow2(c.radius);

    if (d0 < 0 || d0 > hsqd) // behind a cap
        return false;

    // check whether distance from p to axis is less or equal to radius
    auto dsqd = dot(pd, pd) - pow2(d0) / hsqd;
    if (dsqd > rsqd)
        return false;

    return true;
}

template <class ScalarT>
TG_NODISCARD constexpr bool contains(circle<2, ScalarT> const& c, pos<2, ScalarT> const& p, ScalarT eps = ScalarT(0))
{
    return abs(distance_sqr(c.center, p) - c.radius * c.radius) <= eps * eps;
}
template <class ScalarT>
TG_NODISCARD constexpr bool contains(disk<2, ScalarT> const& d, pos<2, ScalarT> const& p, ScalarT eps = ScalarT(0))
{
    auto r = d.radius + eps;
    return distance_sqr(d.center, p) <= r * r;
}

} // namespace tg
