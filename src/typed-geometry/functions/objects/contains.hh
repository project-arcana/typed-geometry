#pragma once

#include <cmath>

#include <typed-geometry/types/objects/aabb.hh>
#include <typed-geometry/types/objects/box.hh>
#include <typed-geometry/types/objects/cylinder.hh>
#include <typed-geometry/types/objects/sphere.hh>
#include <typed-geometry/types/objects/triangle.hh>

#include "distance.hh"

// For a given primitive and a position, return whether the first contains the latter
// contains(a, b) is true iff a contains b

// NOTE:
// contains takes an epsilon parameter that roughly means:
//   contains(a, b) if distance(a, p) <= eps
//   NOTE: some functions interpret eps differently for performance reasons
//         if you really want an epsilon environment, use distance(a, p) <= eps

// Contained functions:
// - contains
//      - aabb
//      - box
//      - cone
//      - inf_cone
//      - cylinder
//      - sphere
//      - triangle

namespace tg
{
// default implementation if distance(a, pos) is available
template <class A, int D, class ScalarT>
[[nodiscard]] constexpr auto contains(A const& a, pos<D, ScalarT> const& p, dont_deduce<ScalarT> eps = ScalarT(0)) -> decltype(ScalarT(distance(a, p)), false)
{
    return distance(a, p) <= eps;
}

template <int D, class ScalarT>
[[nodiscard]] constexpr bool contains(pos<D, ScalarT> const& b, pos<D, ScalarT> const& o, dont_deduce<ScalarT> eps = ScalarT(0))
{
    if (eps > ScalarT(0))
        return distance_sqr(b, o) < eps * eps;
    return b == o;
}

template <class ScalarT>
[[nodiscard]] constexpr bool contains(aabb<1, ScalarT> const& b, ScalarT const& o, dont_deduce<ScalarT> eps = ScalarT(0))
{
    return b.min.x - eps <= o && o <= b.max.x + eps;
}
template <class ScalarT>
[[nodiscard]] constexpr bool contains(aabb<1, ScalarT> const& b, pos<1, ScalarT> const& o, dont_deduce<ScalarT> eps = ScalarT(0))
{
    return b.min.x - eps <= o.x && o.x <= b.max.x + eps;
}
template <class ScalarT>
[[nodiscard]] constexpr bool contains(aabb<2, ScalarT> const& b, pos<2, ScalarT> const& o, dont_deduce<ScalarT> eps = ScalarT(0))
{
    return b.min.x - eps <= o.x && o.x <= b.max.x + eps && //
           b.min.y - eps <= o.y && o.y <= b.max.y + eps;
}
template <class ScalarT>
[[nodiscard]] constexpr bool contains(aabb<3, ScalarT> const& b, pos<3, ScalarT> const& o, dont_deduce<ScalarT> eps = ScalarT(0))
{
    return b.min.x - eps <= o.x && o.x <= b.max.x + eps && //
           b.min.y - eps <= o.y && o.y <= b.max.y + eps && //
           b.min.z - eps <= o.z && o.z <= b.max.z + eps;
}
template <class ScalarT>
[[nodiscard]] constexpr bool contains(aabb<4, ScalarT> const& b, pos<4, ScalarT> const& o, dont_deduce<ScalarT> eps = ScalarT(0))
{
    return b.min.x - eps <= o.x && o.x <= b.max.x + eps && //
           b.min.y - eps <= o.y && o.y <= b.max.y + eps && //
           b.min.z - eps <= o.z && o.z <= b.max.z + eps && //
           b.min.w - eps <= o.w && o.w <= b.max.w + eps;
}
template <int D, class ScalarT>
[[nodiscard]] constexpr bool contains(aabb_boundary<D, ScalarT> const& b, pos<D, ScalarT> const& p, dont_deduce<ScalarT> eps = ScalarT(0))
{
    auto onSomeBoundary = false;
    for (auto i = 0; i < D; ++i)
    {
        if (p[i] < b.min[i] - eps || p[i] > b.max[i] + eps)
            return false; // False if outside of the aabb in any dimension

        if (!onSomeBoundary && (p[i] <= b.min[i] + eps || p[i] >= b.max[i] - eps))
            onSomeBoundary = true;
    }
    return onSomeBoundary; // True, if at on the boundary in at least one dimension
}

template <int D, class ScalarT>
[[nodiscard]] constexpr bool contains(box<D, ScalarT> const& b, pos<D, ScalarT> const& p, dont_deduce<ScalarT> eps = ScalarT(0))
{
    auto r = p - b.center;
    // TODO: unroll
    for (auto i = 0; i < D; ++i)
        if (abs(dot(b.half_extents[i], r)) > length_sqr(b.half_extents[i]) + eps)
            return false;
    return true;
}
template <int D, class ScalarT>
[[nodiscard]] constexpr bool contains(box_boundary<D, ScalarT> const& b, pos<D, ScalarT> const& p, dont_deduce<ScalarT> eps = ScalarT(0))
{
    auto onSomeBoundary = false;
    auto r = p - b.center;
    for (auto i = 0; i < D; ++i)
    {
        auto ri = abs(dot(b.half_extents[i], r));
        auto bi = length_sqr(b.half_extents[i]);
        if (ri > bi + eps)
            return false; // False if outside of the aabb in any dimension

        if (!onSomeBoundary && (ri >= bi - eps))
            onSomeBoundary = true;
    }
    return onSomeBoundary; // True, if at on the boundary in at least one dimension
}

template <class ScalarT>
[[nodiscard]] constexpr bool contains(box<2, ScalarT, 3> const& b, pos<3, ScalarT> const& p, dont_deduce<ScalarT> eps = ScalarT(0))
{
    auto r = p - b.center;

    if (abs(dot(normal(b), r)) > eps)
        return false; // Not in the spanned plane

    if (abs(dot(b.half_extents[0], r)) > length_sqr(b.half_extents[0]) + eps || abs(dot(b.half_extents[1], r)) > length_sqr(b.half_extents[1]) + eps)
        return false; // Outside of the box

    return true;
}
template <class ScalarT>
[[nodiscard]] constexpr bool contains(box_boundary<2, ScalarT, 3> const& b, pos<3, ScalarT> const& p, dont_deduce<ScalarT> eps = ScalarT(0))
{
    auto r = p - b.center;

    if (abs(dot(normal(b), r)) > eps)
        return false; // Not in the spanned plane

    // Rest is the same as for box2
    auto onSomeBoundary = false;
    for (auto i = 0; i < 2; ++i)
    {
        auto ri = abs(dot(b.half_extents[i], r));
        auto bi = length_sqr(b.half_extents[i]);
        if (ri > bi + eps)
            return false;

        if (!onSomeBoundary && (ri >= bi - eps))
            onSomeBoundary = true;
    }
    return onSomeBoundary;
}

template <int D, class ScalarT>
[[nodiscard]] constexpr bool contains(aabb<D, ScalarT> const& b, aabb<D, ScalarT> const& o, dont_deduce<ScalarT> eps = ScalarT(0))
{
    return contains(b, o.min, eps) && contains(b, o.max, eps);
}

template <int D, class ScalarT>
[[nodiscard]] constexpr bool contains(sphere<D, ScalarT> const& s, pos<D, ScalarT> const& p, dont_deduce<ScalarT> eps = ScalarT(0))
{
    auto r = s.radius + eps;
    return distance_sqr(s.center, p) <= r * r;
}
template <int D, class ScalarT>
[[nodiscard]] constexpr bool contains(sphere_boundary<D, ScalarT> const& s, pos<D, ScalarT> const& p, dont_deduce<ScalarT> eps = ScalarT(0))
{
    auto d2 = distance_sqr(s.center, p);
    return pow2(s.radius - eps) <= d2 && d2 <= pow2(s.radius + eps);
}

// Note that eps is used to compare 2D areas, not 1D lengths
template <class ScalarT>
[[nodiscard]] constexpr bool contains(triangle<2, ScalarT> const& t, pos<2, ScalarT> const& p, dont_deduce<ScalarT> eps = ScalarT(0))
{
    auto pv0 = t.pos0 - p;
    auto pv1 = t.pos1 - p;
    auto pv2 = t.pos2 - p;
    auto A0 = cross(pv1, pv2);
    auto A1 = cross(pv2, pv0);
    auto A2 = cross(pv0, pv1);

    auto limit = eps;
    if (eps != ScalarT(0))
    {
        auto aMax = abs(A0) >= abs(A1) ? (abs(A0) >= abs(A2) ? A0 : A2) : (abs(A1) >= abs(A2) ? A1 : A2);
        limit = -std::copysign(eps, aMax) * sign(eps);
    }

    return ((A0 >= limit) == (A1 >= limit)) && //
           ((A1 >= limit) == (A2 >= limit));
}

template <class ScalarT>
[[nodiscard]] constexpr bool contains(triangle<3, ScalarT> const& t, pos<3, ScalarT> const& p, dont_deduce<ScalarT> eps = ScalarT(0))
{
    // TODO: does this also work for triangles where vertices are not ordered cc? should it?

    auto n = normal(t);

    if (!contains(plane<3, ScalarT>(n, t.pos0), p, eps))
        return false;

    // checking whether point lies on left side of the given edge
    auto isLeftOfEdge = [&](segment<3, ScalarT> const& edge) {
        auto pEdge = project(p, edge);
        auto edgeNormal = normalize(cross(edge.pos1 - edge.pos0, n));
        return dot(edgeNormal, p - pEdge) <= eps;
    };

    // Check if the point is on the left side of all edges
    return isLeftOfEdge(segment<3, ScalarT>(t.pos0, t.pos1)) && isLeftOfEdge(segment<3, ScalarT>(t.pos1, t.pos2))
           && isLeftOfEdge(segment<3, ScalarT>(t.pos2, t.pos0));
}

template <class ScalarT>
[[nodiscard]] constexpr bool contains(cylinder<3, ScalarT> const& c, pos<3, ScalarT> const& p)
{
    // https://www.flipcode.com/archives/Fast_Point-In-Cylinder_Test.shtml

    // check whether p is between disks
    auto pd = p - c.axis.pos0;
    auto ad = c.axis.pos1 - c.axis.pos0;
    auto d0 = dot(pd, ad);

    auto hsqd = length_sqr(ad);
    auto rsqd = pow2(c.radius);

    if (d0 < ScalarT(0) || d0 > hsqd) // behind a cap
        return false;

    // check whether distance from p to axis is less or equal to radius
    auto dsqd = dot(pd, pd) - pow2(d0) / hsqd;
    if (dsqd > rsqd)
        return false;

    return true;
}

template <class ScalarT>
[[nodiscard]] constexpr bool contains(cone<3, ScalarT> const& c, pos<3, ScalarT> const& p, dont_deduce<ScalarT> eps = ScalarT(0))
{
    auto center = c.base.center - eps * c.base.normal;

    if (dot(p - center, c.base.normal) < ScalarT(0))
        return false; // Not inside if on the other side of the base

    auto apex = c.base.center + (c.height + eps) * c.base.normal;
    auto pRing = c.base.center + (c.base.radius + eps) * any_normal(c.base.normal);

    // Inside iff the point is closer to the axis (in terms of angle wrt. the apex) than some point on the outer boundary
    return dot(-c.base.normal, normalize(p - apex)) >= dot(-c.base.normal, normalize(pRing - apex));
}
template <class ScalarT>
[[nodiscard]] constexpr bool contains(cone_boundary_no_caps<3, ScalarT> const& c, pos<3, ScalarT> const& p, dont_deduce<ScalarT> eps = ScalarT(0))
{
    if (eps == ScalarT(0))
    {
        if (dot(p - c.base.center, c.base.normal) < ScalarT(0))
            return false; // Not inside if on the other side of the base

        auto apex = c.base.center + c.height * c.base.normal;
        auto pRing = c.base.center + c.base.radius * any_normal(c.base.normal);

        // On the surface iff the point has the same angle to the axis (wrt. the apex) as some point on the outer boundary
        return dot(-c.base.normal, normalize(p - apex)) == dot(-c.base.normal, normalize(pRing - apex));
    }
    else
    {
        // On the boundary within eps if inside an by eps larger and outside of an by eps smaller cone
        auto solidCone = cone<3, ScalarT, default_object_tag>(c.base, c.height);
        return contains(solidCone, p, eps) && !contains(solidCone, p, -eps);
    }
}

template <class ScalarT>
[[nodiscard]] constexpr bool contains(inf_cone<3, ScalarT> const& c, pos<3, ScalarT> const& p, dont_deduce<ScalarT> eps = ScalarT(0))
{
    auto apex = c.apex - (c.opening_dir * eps); // Shift apex outwards to add eps
    auto apexToP = normalize_safe(p - apex);
    if (apexToP == vec<3, ScalarT>::zero)
        return true;
    return angle_between(dir<3, ScalarT>(apexToP), c.opening_dir) <= c.opening_angle;
}
template <class ScalarT>
[[nodiscard]] constexpr bool contains(inf_cone_boundary<3, ScalarT> const& c, pos<3, ScalarT> const& p, dont_deduce<ScalarT> eps = ScalarT(0))
{
    auto apexOuter = c.apex - (c.opening_dir * eps); // Shift apex outwards to add eps
    auto apexInner = c.apex + (c.opening_dir * eps); // Shift apex inwards to subtract eps
    auto apexOuterToP = normalize_safe(p - apexOuter);
    auto apexInnerToP = normalize_safe(p - apexInner);
    if (apexOuterToP == vec<3, ScalarT>::zero || apexInnerToP == vec<3, ScalarT>::zero)
        return true;
    return angle_between(dir<3, ScalarT>(apexOuterToP), c.opening_dir) <= c.opening_angle
           && angle_between(dir<3, ScalarT>(apexInnerToP), c.opening_dir) >= c.opening_angle;
}

template <class ScalarT>
[[nodiscard]] constexpr bool contains(pyramid<triangle<3, ScalarT>> const& py, pos<3, ScalarT> const& p, dont_deduce<ScalarT> eps = ScalarT(0))
{
    const auto c = centroid(py.base);
    auto n = normal(py.base);

    if (dot(p - c + eps * n, n) < ScalarT(0))
        return false; // Not inside if on the other side of the base

    // Check if inside for each pyramid side
    const auto apex = c + n * py.height;
    n = normalize(cross(apex - py.base.pos0, py.base.pos1 - py.base.pos0));
    if (dot(p - apex + eps * n, n) < ScalarT(0))
        return false;
    n = normalize(cross(apex - py.base.pos1, py.base.pos2 - py.base.pos1));
    if (dot(p - apex + eps * n, n) < ScalarT(0))
        return false;
    n = normalize(cross(apex - py.base.pos2, py.base.pos0 - py.base.pos2));
    if (dot(p - apex + eps * n, n) < ScalarT(0))
        return false;

    return true;
}

template <class ScalarT>
[[nodiscard]] constexpr bool contains(pyramid<box<2, ScalarT, 3>> const& py, pos<3, ScalarT> const& p, dont_deduce<ScalarT> eps = ScalarT(0))
{
    const auto c = centroid(py.base);
    auto n = normal(py.base);

    if (dot(p - c + eps * n, n) < ScalarT(0))
        return false; // Not inside if on the other side of the base

    // Check if inside for each pyramid side
    const auto apex = c + n * py.height;
    const auto p0 = py.base[comp<2, ScalarT>(-1, -1)];
    const auto p1 = py.base[comp<2, ScalarT>(1, -1)];
    const auto p2 = py.base[comp<2, ScalarT>(1, 1)];
    const auto p3 = py.base[comp<2, ScalarT>(-1, 1)];

    n = normalize(cross(apex - p0, p1 - p0));
    if (dot(p - apex + eps * n, n) < ScalarT(0))
        return false;
    n = normalize(cross(apex - p1, p2 - p1));
    if (dot(p - apex + eps * n, n) < ScalarT(0))
        return false;
    n = normalize(cross(apex - p2, p3 - p2));
    if (dot(p - apex + eps * n, n) < ScalarT(0))
        return false;
    n = normalize(cross(apex - p3, p0 - p3));
    if (dot(p - apex + eps * n, n) < ScalarT(0))
        return false;

    return true;
}

} // namespace tg
