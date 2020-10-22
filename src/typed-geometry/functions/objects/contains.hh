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
        return distance_sqr(b, o) <= pow2(eps);
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
    return distance_sqr(s.center, p) <= pow2(r);
}
template <int D, class ScalarT>
[[nodiscard]] constexpr bool contains(sphere_boundary<D, ScalarT> const& s, pos<D, ScalarT> const& p, dont_deduce<ScalarT> eps = ScalarT(0))
{
    auto d2 = distance_sqr(s.center, p);
    return pow2(s.radius - eps) <= d2 && d2 <= pow2(s.radius + eps);
}

template <int D, class ScalarT>
[[nodiscard]] constexpr bool contains(hemisphere<D, ScalarT> const& s, pos<D, ScalarT> const& p, dont_deduce<ScalarT> eps = ScalarT(0))
{
    auto c = s.center - eps * s.normal;
    if (dot(s.normal, p - c) < 0)
        return false;

    return distance_sqr(s.center, p) <= pow2(s.radius + eps);
}
template <int D, class ScalarT>
[[nodiscard]] constexpr bool contains(hemisphere_boundary_no_caps<D, ScalarT> const& s, pos<D, ScalarT> const& p, dont_deduce<ScalarT> eps = ScalarT(0))
{
    auto c = s.center - eps * s.normal;
    if (dot(s.normal, p - c) < 0)
        return false;

    auto d2 = distance_sqr(s.center, p);
    return pow2(s.radius - eps) <= d2 && d2 <= pow2(s.radius + eps);
}
// contains(hemisphere_boundary, ...) is not explicitly implemented here because it is not better than the default contains->distance->project implementation

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

template <int D, class ScalarT>
[[nodiscard]] constexpr bool contains(inf_cylinder<D, ScalarT> const& c, pos<D, ScalarT> const& p, dont_deduce<ScalarT> eps = ScalarT(0))
{
    return distance_sqr(p, c.axis) <= pow2(c.radius + eps);
}
template <int D, class ScalarT>
[[nodiscard]] constexpr bool contains(inf_cylinder_boundary<D, ScalarT> const& c, pos<D, ScalarT> const& p, dont_deduce<ScalarT> eps = ScalarT(0))
{
    auto d2 = distance_sqr(p, c.axis);
    return pow2(c.radius - eps) <= d2 && d2 <= pow2(c.radius + eps);
}

template <class BaseT, typename = std::enable_if_t<!std::is_same_v<BaseT, sphere<2, typename BaseT::scalar_t, 3>>>>
[[nodiscard]] constexpr bool contains(pyramid<BaseT> const& py, pos<3, typename BaseT::scalar_t> const& p,
                                      dont_deduce<typename BaseT::scalar_t> eps = typename BaseT::scalar_t(0))
{
    using ScalarT = typename BaseT::scalar_t;
    const auto c = centroid_of(py.base);
    auto n = normal(py.base);

    if (dot(p - c + eps * n, n) < ScalarT(0))
        return false; // Not inside if on the other side of the base

    // Check if inside for each pyramid side
    const auto apex = apex_of(py);
    const auto verts = vertices_of(py.base);
    for (size_t i = 0; i < verts.size(); ++i)
    {
        n = normalize(cross(apex - verts[i], verts[(i + 1) % verts.size()] - verts[i]));
        if (dot(p - apex + eps * n, n) < ScalarT(0))
            return false;
    }
    return true;
}

template <class BaseT, typename = std::enable_if_t<!std::is_same_v<BaseT, sphere<2, typename BaseT::scalar_t, 3>>>>
[[nodiscard]] constexpr bool contains(pyramid_boundary_no_caps<BaseT> const& py, pos<3, typename BaseT::scalar_t> const& p,
                                      dont_deduce<typename BaseT::scalar_t> eps = typename BaseT::scalar_t(0))
{
    // Check if contained in any pyramid side
    using tri_t = triangle<3, typename BaseT::scalar_t>;
    const auto apex = apex_of(py);
    const auto verts = vertices_of(py.base);
    for (size_t i = 0; i < verts.size(); ++i)
        if(contains(tri_t(apex, verts[i], verts[(i + 1) % verts.size()]), p, eps))
            return true;

    return false;
}

template <class BaseT>
[[nodiscard]] constexpr bool contains(pyramid_boundary<BaseT> const& py, pos<3, typename BaseT::scalar_t> const& p,
                                      dont_deduce<typename BaseT::scalar_t> eps = typename BaseT::scalar_t(0))
{
    if (contains(caps_of(py), p, eps))
        return true;

    return contains(boundary_no_caps_of(py), p, eps);
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

        auto apex = apex_of(c);
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

template <int D, class ScalarT>
[[nodiscard]] constexpr bool contains(inf_cone<D, ScalarT> const& c, pos<D, ScalarT> const& p, dont_deduce<ScalarT> eps = ScalarT(0))
{
    auto apex = c.apex - (c.opening_dir * eps); // Shift apex outwards to add eps
    auto apexToP = normalize_safe(p - apex);
    if (apexToP == vec<D, ScalarT>::zero)
        return true;
    return angle_between(dir<D, ScalarT>(apexToP), c.opening_dir) <= ScalarT(0.5) * c.opening_angle; // opening_angle is between the cone surfaces
}
template <int D, class ScalarT>
[[nodiscard]] constexpr bool contains(inf_cone_boundary<D, ScalarT> const& c, pos<D, ScalarT> const& p, dont_deduce<ScalarT> eps = ScalarT(0))
{
    auto apexOuter = c.apex - (c.opening_dir * eps); // Shift apex outwards to add eps
    auto apexInner = c.apex + (c.opening_dir * eps); // Shift apex inwards to subtract eps
    auto apexOuterToP = normalize_safe(p - apexOuter);
    auto apexInnerToP = normalize_safe(p - apexInner);
    if (apexOuterToP == vec<D, ScalarT>::zero || apexInnerToP == vec<D, ScalarT>::zero)
        return true;
    return angle_between(dir<D, ScalarT>(apexOuterToP), c.opening_dir) <= ScalarT(0.5) * c.opening_angle
           && angle_between(dir<D, ScalarT>(apexInnerToP), c.opening_dir) >= ScalarT(0.5) * c.opening_angle;
}
} // namespace tg
