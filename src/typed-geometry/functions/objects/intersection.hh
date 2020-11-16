#pragma once

#include <typed-geometry/detail/optional.hh>
#include <typed-geometry/feature/assert.hh>
#include <typed-geometry/functions/basic/scalar_math.hh>

#include <typed-geometry/types/objects/aabb.hh>
#include <typed-geometry/types/objects/box.hh>
#include <typed-geometry/types/objects/capsule.hh>
#include <typed-geometry/types/objects/cylinder.hh>
#include <typed-geometry/types/objects/halfspace.hh>
#include <typed-geometry/types/objects/hemisphere.hh>
#include <typed-geometry/types/objects/line.hh>
#include <typed-geometry/types/objects/plane.hh>
#include <typed-geometry/types/objects/ray.hh>
#include <typed-geometry/types/objects/segment.hh>
#include <typed-geometry/types/objects/sphere.hh>
#include <typed-geometry/types/objects/triangle.hh>

#include <typed-geometry/functions/vector/cross.hh>
#include <typed-geometry/functions/vector/dot.hh>
#include <typed-geometry/functions/vector/length.hh>

#include "aabb.hh"
#include "contains.hh"
#include "coordinates.hh"
#include "direction.hh"
#include "normal.hh"

// family of intersection functions:

// intersects(a, b)              -> bool
// intersection(a, b)            -> ???
// intersection_safe(a, b)       -> optional<???>
// intersection_parameter(a, b)  -> coords? (for a ray: ray_hits<N, ScalarT> or optional<ray_interval> (when b is solid))
// intersection_parameters(a, b) -> pair<coords, coords>?
// intersection_exact(a, b)      -> variant
// closest_intersection(a, b)            -> position/object (for a ray: optional<pos>)
// closest_intersection_parameter(a, b)  -> coords (for a ray: optional<ScalarT>)

// "intersects" returns true iff any point lies in a and in b
// "intersection" returns an object describing the intersection (NOTE: does NOT handle degenerate cases)
// "intersection_safe" is the same as "intersection" but returns nullopt for degenerate cases
// "intersection_parameter" returns coordinates for the first object such that a[coords] == intersection(a, b)
// "intersection_parameters" returns coordinates for both objects
// "intersection_exact" returns a variant type describing all possible intersections, including degenerate cases
// the "closest_" variants only return the closest intersection for objects where that concept is applicable (e.g. for rays)

// Notes:
//  - intersection_exact is currently unsupported
//  - intersection_safe is currently unsupported
//  - for more elaborate ray-tracing, a future ray_cast function will exist (which also returns the intersection normal)

// Implementation guidelines:
// explicit intersection_parameter(ray, obj), which gives closest_intersection_parameter and intersection position(s)
//          intersection_parameter(ray, obj_boundary) already gives intersection_parameter(ray, obj) when boundary_of(obj) is defined and object is convex and finite
//          for infinite objects, a separate solid version is needed when the boundary version does not distinguish between completely contained and completely outside
// explicit closest_intersection_parameter(ray, obj) if this is faster than computing all intersections
// explicit intersects(obj, aabb), which gives intersects(aabb, obj)
//
// for convex compound objects (like cylinder or pyramids), decompose the object into primitive shapes and pass them to a helper function:
// - call merge_hits(ray, objPart1, objPart2, ...) in the implementation of intersection_parameter(ray, obj_boundary)
// - call intersects_any(ray, objPart1, objPart2, ...) in the implementation of intersects(ray, obj<TraitsT>), which can shortcut and be faster than the default


namespace tg
{
// ====================================== Result Structs ======================================

/// ordered list of ray intersection hits
/// behaves like a container with
///   .size()
///   operator[]
///   range-based-for
template <int MaxHits, class HitT>
struct ray_hits
{
    static constexpr bool is_ray_hits = true; // tag
    static constexpr int max_hits = MaxHits;

    template <class OtherT>
    using as_ray_hits = ray_hits<MaxHits, OtherT>;

    [[nodiscard]] int size() const { return _size; }
    [[nodiscard]] bool any() const { return _size > 0; }

    HitT const& operator[](int idx) const
    {
        TG_ASSERT(0 <= idx && idx < _size);
        return _hit[idx];
    }
    [[nodiscard]] HitT const& first() const
    {
        TG_ASSERT(_size > 0);
        return _hit[0];
    }

    [[nodiscard]] HitT const* begin() const { return _hit; }
    [[nodiscard]] HitT const* end() const { return _hit + _size; }

    ray_hits() = default;
    ray_hits(HitT* hits, int size) : _size(size)
    {
        for (auto i = 0; i < size; ++i)
            _hit[i] = hits[i];
    }
    template <typename... HitTs>
    ray_hits(HitTs... hits) : _size(sizeof...(HitTs)), _hit{hits...}
    {
    }

private:
    int _size = 0;
    HitT _hit[MaxHits];
};

/// describes a continuous interval on a ray between start and end
template <class ScalarT>
struct ray_interval
{
    ScalarT start;
    ScalarT end;

    [[nodiscard]] constexpr bool is_unbounded() const { return end == tg::max<ScalarT>(); }
};


// ====================================== Default Implementations ======================================
// TODO: intersection_parameter from intersection_parameters

// returns whether two objects intersect
template <class A, class B>
[[nodiscard]] constexpr auto intersects(A const& a, B const& b) -> decltype(intersection(a, b).has_value())
{
    return intersection(a, b).has_value();
}

// if closest intersection parameter is available and applicable, use that
template <int D, class ScalarT, class Obj>
[[nodiscard]] constexpr auto intersects(ray<D, ScalarT> const& r, Obj const& obj) -> decltype(closest_intersection_parameter(r, obj).has_value())
{
    return closest_intersection_parameter(r, obj).has_value();
}

// parameters for intersects with aabb can switch order
template <int D, class ScalarT, class Obj>
[[nodiscard]] constexpr bool intersects(aabb<D, ScalarT> const& b, Obj const& obj)
{
    return intersects(obj, b);
}
// Explicit intersects aabb aabb to prevent infinite recursion
template <int D, class ScalarT>
[[nodiscard]] constexpr bool intersects(aabb<D, ScalarT> const& a, aabb<D, ScalarT> const& b)
{
    return intersection(a, b).has_value();
}

// if a value-typed intersection parameter is available and applicable, use that
template <class A, class B>
[[nodiscard]] constexpr auto intersection(A const& a, B const& b) -> decltype(a[intersection_parameter(a, b)])
{
    return a[intersection_parameter(a, b)];
}

// if an optional intersection parameter is available and applicable, use that
template <class A, class B>
[[nodiscard]] constexpr auto intersection(A const& a, B const& b) -> optional<decltype(a[intersection_parameter(a, b).value()])>
{
    if (auto t = intersection_parameter(a, b); t.has_value())
        return a[t.value()];
    return {};
}

// if a value-typed closest intersection parameter is available and applicable, use that
template <class A, class B>
[[nodiscard]] constexpr auto closest_intersection(A const& a, B const& b) -> decltype(a[closest_intersection_parameter(a, b)])
{
    return a[closest_intersection_parameter(a, b)];
}

// if an optional closest intersection parameter is available and applicable, use that
template <class A, class B>
[[nodiscard]] constexpr auto closest_intersection(A const& a, B const& b) -> optional<decltype(a[closest_intersection_parameter(a, b).value()])>
{
    if (auto t = closest_intersection_parameter(a, b); t.has_value())
        return a[t.value()];
    return {};
}

// if ray_hits intersection parameter is available, use that
template <int D, class ScalarT, class Obj>
[[nodiscard]] constexpr auto intersection(ray<D, ScalarT> const& r, Obj const& obj) ->
    typename decltype(intersection_parameter(r, obj))::template as_ray_hits<pos<D, ScalarT>>
{
    auto ts = intersection_parameter(r, obj);
    pos<D, ScalarT> hits[ts.max_hits];
    for (auto i = 0; i < ts.size(); ++i)
        hits[i] = r[ts[i]];
    return {hits, ts.size()};
}

// if an optional ray_interval intersection parameter is available, use that
template <int D, class ScalarT, class Obj, typename = std::enable_if_t<std::is_same_v<decltype(
    intersection_parameter(std::declval<ray<D, ScalarT>>(), std::declval<Obj>())), optional<ray_interval<ScalarT>>>>>
[[nodiscard]] constexpr optional<segment<D, ScalarT>> intersection(ray<D, ScalarT> const& r, Obj const& obj)
{
    auto ts = intersection_parameter(r, obj);
    if (ts.has_value())
        return {r[ts.value().start], r[ts.value().end]};
    return {};
}

// if ray_hits intersection parameter is available, use that
template <int D, class ScalarT, class Obj>
[[nodiscard]] constexpr auto closest_intersection_parameter(ray<D, ScalarT> const& r, Obj const& obj)
    -> enable_if<decltype(intersection_parameter(r, obj))::is_ray_hits, optional<ScalarT>>
{
    auto hits = intersection_parameter(r, obj);
    if (hits.any())
        return hits.first();
    return {};
}

// if ray_interval intersection parameter is available, use that
template <int D, class ScalarT, class Obj>
[[nodiscard]] constexpr auto closest_intersection_parameter(ray<D, ScalarT> const& r, Obj const& obj)
    -> enable_if<std::is_same_v<decltype(intersection_parameter(r, obj)), optional<ray_interval<ScalarT>>>, optional<ScalarT>>
{
    auto hits = intersection_parameter(r, obj);
    if (hits.has_value())
        return hits.value().start;
    return {};
}

// if boundary_of a solid object returns ray_hits, use this to construct the ray_interval result of the solid intersection
template <int D, class ScalarT, class Obj>
[[nodiscard]] constexpr auto intersection_parameter(ray<D, ScalarT> const& r, Obj const& obj)
    -> enable_if<!std::is_same_v<Obj, decltype(boundary_of(obj))>, optional<ray_interval<ScalarT>>>
{
    const auto inter = intersection_parameter(r, boundary_of(obj));

    if (inter.size() == 0)
        return {};

    if (inter.size() == 1)
    {
        TG_ASSERT(contains(obj, r.origin));
        return {{typename Obj::scalar_t(0), inter.first()}};
    }

    TG_ASSERT(inter.size() == 2);
    return {{inter[0], inter[1]}};
}

// intersection between point and obj is same as contains
template <int D, class ScalarT, class Obj, class = void_t<decltype(contains(std::declval<pos<D, ScalarT>>(), std::declval<Obj>()))>>
constexpr optional<pos<D, ScalarT>> intersection(pos<D, ScalarT> const& p, Obj const& obj)
{
    if (contains(obj, p))
        return p;
    return {};
}

// intersection between point and obj is same as contains
template <int D, class ScalarT, class Obj, class = void_t<decltype(contains(std::declval<pos<D, ScalarT>>(), std::declval<Obj>()))>>
constexpr optional<pos<D, ScalarT>> intersection(Obj const& obj, pos<D, ScalarT> const& p)
{
    if (contains(obj, p))
        return p;
    return {};
}


// ====================================== Helper functions ======================================

namespace detail
{
// intersects the given ray with all given objects and returns the concatenated intersections. A maximal number of 2 intersections is assumed.
template <int D, class ScalarT, class... ObjsT>
[[nodiscard]] constexpr ray_hits<2, ScalarT> merge_hits(ray<D, ScalarT> const& ray, ObjsT const&... objs)
{
    ScalarT hits[2];
    hits[0] = tg::max<ScalarT>();
    hits[1] = tg::min<ScalarT>();
    auto numHits = 0;

    const auto find_hits = [&](const auto& obj) {
        const auto inters = intersection_parameter(ray, obj);
        for (const auto& inter : inters)
        {
            hits[0] = tg::min(hits[0], inter);
            hits[1] = tg::max(hits[1], inter);
            numHits++;
        }
    };
    (find_hits(objs), ...);

    TG_ASSERT(numHits <= 2);
    return {hits, numHits};
}

// returns true, iff the given ray intersects any of the given objects (with short-circuiting after the first intersection)
template <int D, class ScalarT, class... ObjsT>
[[nodiscard]] constexpr bool intersects_any(ray<D, ScalarT> const& ray, ObjsT const&... objs)
{
    return (intersects(ray, objs) || ...);
}
}

// ====================================== Ray - Object Intersections ======================================

// ray - point
template <class ScalarT>
[[nodiscard]] constexpr ray_hits<1, ScalarT> intersection_parameter(ray<1, ScalarT> const& r, pos<1, ScalarT> const& p)
{
    const auto t = coordinates(r, p);
    if (t >= ScalarT(0))
        return t;
    return {};
}

// ray - line
template <class ScalarT>
[[nodiscard]] constexpr ray_hits<1, ScalarT> intersection_parameter(ray<2, ScalarT> const& r, line<2, ScalarT> const& l)
{
    // r.origin + r.dir * t.x == l.pos + l.dir * t.y  <=>  (r.dir | -l.dir) * (t.x | t.y)^T == l.pos - r.origin
    auto M = mat<2, 2, ScalarT>::from_cols(r.dir, -l.dir);
    auto t = inverse(M) * (l.pos - r.origin);
    if (t.x < ScalarT(0))
        return {};
    return t.x;
}

// ray - ray
template <class ScalarT>
[[nodiscard]] constexpr ray_hits<1, ScalarT> intersection_parameter(ray<2, ScalarT> const& r0, ray<2, ScalarT> const& r1)
{
    auto M = mat<2, 2, ScalarT>::from_cols(r0.dir, -r1.dir);
    auto t = inverse(M) * (r1.origin - r0.origin);
    if (t.x < ScalarT(0) || t.y < ScalarT(0))
        return {};
    return t.x;
}

// ray - segment
template <class ScalarT>
[[nodiscard]] constexpr ray_hits<1, ScalarT> intersection_parameter(ray<2, ScalarT> const& r, segment<2, ScalarT> const& s)
{
    auto M = mat<2, 2, ScalarT>::from_cols(r.dir, s.pos0 - s.pos1);
    auto t = inverse(M) * (s.pos0 - r.origin);
    if (t.x < ScalarT(0) || t.y < ScalarT(0) || t.y > ScalarT(1))
        return {};
    return t.x;
}

// ray - plane
template <int D, class ScalarT>
[[nodiscard]] constexpr ray_hits<1, ScalarT> intersection_parameter(ray<D, ScalarT> const& r, plane<D, ScalarT> const& p)
{
    // if plane normal and ray direction are orthogonal there is no intersection
    const auto dotND = dot(p.normal, r.dir);
    if (dotND == ScalarT(0))
        return {};

    // plane: <x, p.normal> = p.dis
    // ray: x = r.origin + t * r.dir
    // =>
    // <r.origin + t * r.dir, p.normal> = p.dis
    // t = (p.dis - <r.origin, p.normal>) / <r.dir, p.normal>

    const auto t = (p.dis - dot(p.normal, r.origin)) / dotND;

    // check whether plane lies behind ray
    if (t < ScalarT(0))
        return {};

    return t;
}

// ray - halfspace
template <int D, class ScalarT>
[[nodiscard]] constexpr optional<ray_interval<ScalarT>> intersection_parameter(ray<D, ScalarT> const& r, halfspace<D, ScalarT> const& h)
{
    // check if ray origin is already contained in the halfspace
    const auto dotND = dot(h.normal, r.dir);
    const auto dist = signed_distance(r.origin, h);
    if (dist <= ScalarT(0))
    {
        if (dotND <= ScalarT(0)) // if ray point away from the surface, the entire ray intersects
            return {{ScalarT(0), tg::max<ScalarT>()}};
        return {{ScalarT(0), -dist / dotND}}; // otherwise the segment from ray origin until surface intersection is contained
    }

    // if ray points away from the halfspace there is no intersection
    if (dotND >= ScalarT(0))
        return {};

    return {{-dist / dotND, tg::max<ScalarT>()}};
}
template <int D, class ScalarT>
[[nodiscard]] constexpr optional<ScalarT> closest_intersection_parameter(ray<D, ScalarT> const& r, halfspace<D, ScalarT> const& h)
{
    // check if ray origin is already contained in the halfspace
    const auto dist = signed_distance(r.origin, h);
    if (dist <= ScalarT(0))
        return ScalarT(0);

    // if ray points away from the halfspace there is no intersection
    const auto dotND = dot(h.normal, r.dir);
    if (dotND >= ScalarT(0))
        return {};

    return -dist / dotND;
}

// ray - aabb
template <int D, class ScalarT>
[[nodiscard]] constexpr ray_hits<2, ScalarT> intersection_parameter(ray<D, ScalarT> const& r, aabb_boundary<D, ScalarT> const& b)
{
    auto tFirst = tg::min<ScalarT>();
    auto tSecond = tg::max<ScalarT>();
    for (auto i = 0; i < D; ++i)
    {
        if (abs(r.dir[i]) > ScalarT(100) * tg::epsilon<ScalarT>)
        {
            const auto tMin = (b.min[i] - r.origin[i]) / r.dir[i];
            const auto tMax = (b.max[i] - r.origin[i]) / r.dir[i];
            tFirst = max(tFirst, min(tMin, tMax));
            tSecond = min(tSecond, max(tMin, tMax));
        }
        else if (r.origin[i] < b.min[i] || r.origin[i] > b.max[i])
            return {}; // ray parallel to this axis and outside of the aabb
    }

    // no intersection if ray starts behind the aabb or if it misses the aabb
    if (tSecond < ScalarT(0) || tFirst > tSecond)
        return {};

    if (tFirst < ScalarT(0))
        return tSecond;

    return {tFirst, tSecond};
}

// ray - box
template <int D, class ScalarT>
[[nodiscard]] constexpr ray_hits<2, ScalarT> intersection_parameter(ray<D, ScalarT> const& r, box_boundary<D, ScalarT> const& b)
{
    const auto bMin = b[comp<D, ScalarT>(-1)] - r.origin;
    const auto bMax = b[comp<D, ScalarT>(1)] - r.origin;
    auto tFirst = tg::min<ScalarT>();
    auto tSecond = tg::max<ScalarT>();
    for (auto i = 0; i < D; ++i)
    {
        const auto rDir = dot(r.dir, b.half_extents[i]);
        if (abs(rDir) > ScalarT(100) * tg::epsilon<ScalarT>)
        {
            const auto tMin = dot(bMin, b.half_extents[i]) / rDir;
            const auto tMax = dot(bMax, b.half_extents[i]) / rDir;
            tFirst = max(tFirst, min(tMin, tMax));
            tSecond = min(tSecond, max(tMin, tMax));
        }
        else if (dot(bMin, b.half_extents[i]) > ScalarT(0) || dot(bMax, b.half_extents[i]) < ScalarT(0))
            return {}; // ray parallel to this half_extents axis and outside of the box
    }

    // no intersection if ray starts behind the box or if it misses the box
    if (tSecond < ScalarT(0) || tFirst > tSecond)
        return {};

    if (tFirst < ScalarT(0))
        return tSecond;

    return {tFirst, tSecond};
}

// returns intersection point(s) of ray and sphere
template <int D, class ScalarT>
[[nodiscard]] constexpr ray_hits<2, ScalarT> intersection_parameter(ray<D, ScalarT> const& r, sphere_boundary<D, ScalarT> const& s)
{
    auto t = dot(s.center - r.origin, r.dir);

    auto d_sqr = distance_sqr(r[t], s.center);
    auto r_sqr = s.radius * s.radius;
    if (d_sqr > r_sqr)
        return {};

    auto dt = sqrt(r_sqr - d_sqr);

    if (t - dt >= 0)
        return {t - dt, t + dt};

    if (t + dt >= 0)
        return {t + dt};

    return {};
}

// ray - disk
template <class ScalarT>
[[nodiscard]] constexpr ray_hits<1, ScalarT> intersection_parameter(ray<3, ScalarT> const& r, sphere<2, ScalarT, 3> const& d)
{
    auto const t = intersection_parameter(r, plane<3, ScalarT>(d.normal, d.center));
    if (!t.any())
        return {};

    auto const p = r[t.first()];
    if (distance_sqr(p, d.center) > d.radius * d.radius)
        return {};

    return t;
}

// ray - hemisphere
template <int D, class ScalarT>
[[nodiscard]] constexpr ray_hits<2, ScalarT> intersection_parameter(ray<D, ScalarT> const& r, hemisphere_boundary<D, ScalarT> const& h)
{
    return detail::merge_hits(r, caps_of(h), boundary_no_caps_of(h));
}
template <int D, class ScalarT>
[[nodiscard]] constexpr ray_hits<2, ScalarT> intersection_parameter(ray<D, ScalarT> const& r, hemisphere_boundary_no_caps<D, ScalarT> const& h)
{
    ScalarT hits[2];
    auto numHits = 0;
    const auto sphereHits = intersection_parameter(r, sphere_boundary<D, ScalarT>(h.center, h.radius));
    const auto halfSpace = halfspace<D, ScalarT>(-h.normal, h.center); // the intersection of this halfspace and the sphere is exactly the hemisphere
    for (const auto& hit : sphereHits)
        if (contains(halfSpace, r[hit]))
            hits[numHits++] = hit;

    return {hits, numHits};
}
template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr bool intersects(ray<D, ScalarT> const& r, hemisphere<D, ScalarT, TraitsT> const& h)
{
    if constexpr (std::is_same_v<TraitsT, boundary_no_caps_tag>)
        return intersection_parameter(r, h).any();
    else
        return detail::intersects_any(r, caps_of(h), boundary_no_caps_of(h));
}

// ray - capsule
template <class ScalarT>
[[nodiscard]] constexpr ray_hits<2, ScalarT> intersection_parameter(ray<3, ScalarT> const& r, capsule_boundary<3, ScalarT> const& c)
{
    using caps_t = hemisphere_boundary_no_caps<3, ScalarT>;
    const auto n = direction(c);
    return detail::merge_hits(r, caps_t(c.axis.pos0, c.radius, -n), caps_t(c.axis.pos1, c.radius, n),
                              cylinder_boundary_no_caps<3, ScalarT>(c.axis, c.radius));
}
template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr bool intersects(ray<3, ScalarT> const& r, capsule<3, ScalarT, TraitsT> const& c)
{
    using caps_t = sphere_boundary<3, ScalarT>; // spheres are faster than hemispheres and equivalent for the yes/no decision
    return detail::intersects_any(r, caps_t(c.axis.pos0, c.radius), caps_t(c.axis.pos1, c.radius),
                                  cylinder_boundary_no_caps<3, ScalarT>(c.axis, c.radius));
}

// ray - cylinder
template <class ScalarT>
[[nodiscard]] constexpr ray_hits<2, ScalarT> intersection_parameter(ray<3, ScalarT> const& r, cylinder_boundary<3, ScalarT> const& c)
{
    const auto caps = caps_of(c);
    return detail::merge_hits(r, caps[0], caps[1], boundary_no_caps_of(c));
}
template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr bool intersects(ray<3, ScalarT> const& r, cylinder<3, ScalarT, TraitsT> const& c)
{
    if constexpr (std::is_same_v<TraitsT, boundary_no_caps_tag>)
        return intersection_parameter(r, c).any();
    else
    {
        const auto caps = caps_of(c);
        return detail::intersects_any(r, caps[0], caps[1], boundary_no_caps_of(c));
    }
}

// ray - tube
template <class ScalarT>
[[nodiscard]] constexpr ray_hits<2, ScalarT> intersection_parameter(ray<3, ScalarT> const& r, cylinder_boundary_no_caps<3, ScalarT> const& c)
{
    auto cdir = direction(c);
    auto cosA = dot(cdir, r.dir);
    auto sinA_sqr = 1 - cosA * cosA;

    if (sinA_sqr <= 0)
        return {};

    // compute closest points of the two lines
    auto origDiff = r.origin - c.axis.pos0;
    auto fRay = dot(r.dir, origDiff);
    auto fLine = dot(cdir, origDiff);
    auto tRay = (cosA * fLine - fRay) / sinA_sqr;
    auto tLine = (fLine - cosA * fRay) / sinA_sqr;

    auto closest_on_ray = r.origin + tRay * r.dir;
    auto closest_on_line = c.axis.pos0 + tLine * cdir;
    auto line_ray_dist_sqr = distance_sqr(closest_on_ray, closest_on_line);
    auto cyl_radius_sqr = c.radius * c.radius;

    if (line_ray_dist_sqr > cyl_radius_sqr)
        return {};

    // radius in 2D slice
    auto r_2D = sqrt(cyl_radius_sqr - line_ray_dist_sqr);

    // infinite tube intersection
    auto s = r_2D / sqrt(sinA_sqr);
    auto cyl_intersection_0 = closest_on_ray - s * r.dir;
    auto cyl_intersection_1 = closest_on_ray + s * r.dir;

    // project onto line segment
    auto line_length = length(c.axis);
    auto lambda_0 = dot(cyl_intersection_0 - c.axis.pos0, cdir);
    auto lambda_1 = dot(cyl_intersection_1 - c.axis.pos0, cdir);

    ScalarT hits[2];
    int hit_cnt = 0;

    if (tRay - s >= 0 && 0 <= lambda_0 && lambda_0 < line_length)
        hits[hit_cnt++] = tRay - s;
    if (tRay + s >= 0 && 0 <= lambda_1 && lambda_1 < line_length)
        hits[hit_cnt++] = tRay + s;

    return {hits, hit_cnt};
}

// ray - triangle2
template <class ScalarT>
[[nodiscard]] constexpr optional<ray_interval<ScalarT>> intersection_parameter(ray<2, ScalarT> const& r, triangle<2, ScalarT> const& t)
{
    ScalarT closestIntersection = tg::max<ScalarT>();
    ScalarT furtherIntersection = tg::min<ScalarT>();
    auto numIntersections = 0;
    for (const auto& edge : edges_of(t))
    {
        const auto inter = intersection_parameter(r, edge);
        if (inter.any())
        {
            numIntersections++;
            closestIntersection = min(closestIntersection, inter.first());
            furtherIntersection = max(furtherIntersection, inter.first());
        }
    }

    if (numIntersections == 0)
        return {}; // No intersection
    if (numIntersections == 1)
    {
        // ray started within the triangle
        TG_ASSERT(contains(t, r.origin));
        return {{ScalarT(0), furtherIntersection}};
    }
    return {{closestIntersection, furtherIntersection}};
}

// ray - triangle3
template <class ScalarT>
[[nodiscard]] constexpr ray_hits<1, ScalarT> intersection_parameter(ray<3, ScalarT> const& r,
                                                                 triangle<3, ScalarT> const& t,
                                                                 dont_deduce<ScalarT> eps = 100 * tg::epsilon<ScalarT>)
{
    auto e1 = t.pos1 - t.pos0;
    auto e2 = t.pos2 - t.pos0;

    auto pvec = tg::cross(r.dir, e2);
    auto det = dot(pvec, e1);

    if (det < ScalarT(0))
    {
        std::swap(e1, e2);
        pvec = tg::cross(r.dir, e2);
        det = -det;
    }

    if (det < eps)
        return {};

    auto tvec = r.origin - t.pos0;
    auto u = dot(tvec, pvec);
    if (u < ScalarT(0) || u > det)
        return {};

    auto qvec = cross(tvec, e1);
    auto v = dot(r.dir, qvec);
    if (v < ScalarT(0) || v + u > det)
        return {};

    auto lambda = (ScalarT(1) / det) * dot(e2, qvec);
    return (lambda > ScalarT(0)) ? lambda : ray_hits<1, ScalarT>();
}

// ray - quadric_boundary (as an isosurface, not error quadric)
template <class ScalarT>
[[nodiscard]] constexpr ray_hits<2, ScalarT> intersection_parameter(ray<3, ScalarT> const& r, quadric<3, ScalarT> const& q)
{
    const auto Ad = q.A() * r.dir;
    const auto p = r.origin;

    // Substituting x in Quadric equation x^TAx + 2b^Tx + c = 0 by ray equation x = t * dir + p yields
    // d^TAd t^2 + (2p^TAd + 2bd) t + p^TAp + 2bp + c = 0
    const auto a = dot(r.dir, Ad);
    const auto b = ScalarT(2) * (dot(p, Ad) + dot(q.b(), r.dir));
    const auto c = dot(p, q.A() * vec3(p)) + ScalarT(2) * dot(q.b(), p) + q.c;

    // Solve the quadratic equation ax^2 + bx + c = 0
    const auto discriminant = b * b - ScalarT(4) * a * c;
    if (discriminant < ScalarT(0))
        return {}; // No solution

    const auto sqrtD = sqrt(discriminant);
    const auto t1 = (-b - sqrtD) / (ScalarT(2) * a);
    const auto t2 = (-b + sqrtD) / (ScalarT(2) * a);

    auto tMin = min(t1, t2);
    auto tMax = max(t1, t2);

    if (tMin >= ScalarT(0))
        return {tMin, tMax};

    if (tMax >= ScalarT(0))
        return tMax;

    return {};
}


// ====================================== Object - Object Intersections ======================================

// returns intersection circle of sphere and sphere (normal points from a to b)
// for now does not work if spheres are identical (result would be a sphere3 again)
template <class ScalarT>
[[nodiscard]] constexpr optional<sphere_boundary<2, ScalarT, 3>> intersection(sphere_boundary<3, ScalarT> const& a, sphere_boundary<3, ScalarT> const& b)
{
    auto d2 = distance_sqr(a.center, b.center);

    // TODO: intersection sphere
    if (a.center == b.center && a.radius == b.radius)
        return {};

    auto d = sqrt(d2);

    // no intersection
    if (d > a.radius + b.radius)
        return {};

    // radius and centers of larger sphere (ls) and smaller sphere (ss)
    auto lsr = a.radius;
    auto ssr = b.radius;
    auto lsc = a.center;
    auto ssc = b.center;
    if (b.radius > a.radius)
    {
        // TODO: tg::swap?
        lsr = b.radius;
        ssr = a.radius;
        lsc = b.center;
        ssc = a.center;
    }

    if (d + ssr < lsr)
    {
        // Smaller sphere inside larger one and not touching it
        return {};
    }

    TG_INTERNAL_ASSERT(d > ScalarT(0));

    // squared radii of a and b
    auto ar2 = a.radius * a.radius;
    auto br2 = b.radius * b.radius;

    auto t = ScalarT(0.5) + (ar2 - br2) / (ScalarT(2) * d2);

    // position and radius of intersection circle
    auto ipos = a.center + t * (b.center - a.center);
    auto irad = sqrt(ar2 - t * t * d2);

    // non-empty intersection (circle)
    return sphere_boundary<2, ScalarT, 3>{ipos, irad, dir<3, ScalarT>((b.center - a.center) / d)};
}

// returns intersection points of two circles in 2D
// for now does not work if circles are identical (result would be a circle2 again)
template <class ScalarT>
[[nodiscard]] constexpr optional<pair<pos<2, ScalarT>, pos<2, ScalarT>>> intersection(sphere_boundary<2, ScalarT> const& a, sphere_boundary<2, ScalarT> const& b)
{
    if (a.center == b.center && a.radius == b.radius)
        return {}; // degenerate case

    auto d2 = distance_sqr(a.center, b.center);
    auto d = sqrt(d2);
    auto ar = a.radius;
    auto br = b.radius;
    if (ar + br < d) // no intersection
        return {};

    if (d < abs(ar - br)) // no intersection (one inside the other)
        return {};

    TG_INTERNAL_ASSERT(d > ScalarT(0));

    auto t = (ar * ar - br * br + d2) / (2 * d);
    auto h2 = ar * ar - t * t;
    TG_INTERNAL_ASSERT(h2 >= ScalarT(0));

    auto h = sqrt(h2);
    auto h_by_d = h / d;

    auto p_between = a.center + t / d * (b.center - a.center);

    auto a_to_b = b.center - a.center;
    auto a_to_b_swap = tg::vec2(-a_to_b.y, a_to_b.x);

    // imagining circle a on the left side of circle b...
    auto p_above = p_between + h_by_d * a_to_b_swap;
    auto p_below = p_between - h_by_d * a_to_b_swap;

    return pair{p_above, p_below};
}


template <class ScalarT>
[[nodiscard]] constexpr line<3, ScalarT> intersection(plane<3, ScalarT> const& a, plane<3, ScalarT> const& b)
{
    // see http://mathworld.wolfram.com/Plane-PlaneIntersection.html
    auto dir = normalize(cross(a.normal, b.normal));
    auto p = pos<3, ScalarT>::zero;

    if (abs(dir.z) > abs(dir.x)) // solve with p.z = 0
    {
        auto n0 = tg::vec<2, ScalarT>(a.normal.x, b.normal.x);
        auto n1 = tg::vec<2, ScalarT>(a.normal.y, b.normal.y);
        auto r = tg::vec<2, ScalarT>(a.dis, b.dis);
        auto p2 = inverse(mat<2, 2, ScalarT>::from_cols(n0, n1)) * r;
        p.x = p2.x;
        p.y = p2.y;
    }
    else if (abs(dir.y) > abs(dir.x)) // solve with p.y = 0
    {
        auto n0 = tg::vec<2, ScalarT>(a.normal.x, b.normal.x);
        auto n1 = tg::vec<2, ScalarT>(a.normal.z, b.normal.z);
        auto r = tg::vec<2, ScalarT>(a.dis, b.dis);
        auto p2 = inverse(mat<2, 2, ScalarT>::from_cols(n0, n1)) * r;
        p.x = p2.x;
        p.z = p2.y;
    }
    else // solve with p.x = 0
    {
        auto n0 = tg::vec<2, ScalarT>(a.normal.y, b.normal.y);
        auto n1 = tg::vec<2, ScalarT>(a.normal.z, b.normal.z);
        auto r = tg::vec<2, ScalarT>(a.dis, b.dis);
        auto p2 = inverse(mat<2, 2, ScalarT>::from_cols(n0, n1)) * r;
        p.y = p2.x;
        p.z = p2.y;
    }

    return {p, dir};
}

template <class ScalarT>
[[nodiscard]] constexpr optional<pair<ScalarT, ScalarT>> intersection_parameters(segment<2, ScalarT> const& seg_0, segment<2, ScalarT> const& seg_1)
{
    /// https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection
    auto const denom = (seg_0.pos0.x - seg_0.pos1.x) * (seg_1.pos0.y - seg_1.pos1.y) - (seg_0.pos0.y - seg_0.pos1.y) * (seg_1.pos0.x - seg_1.pos1.x);

    // todo: might want to check == 0 with an epsilon corridor
    // todo: colinear line segments can still intersect in a point or a line segment.
    //       This might require api changes, as either a point or a line segment can be returned!
    //       Possible solution: return a segment where pos0 == pos1
    if (denom == ScalarT(0))
        return {}; // colinear

    auto const num0 = (seg_0.pos0.x - seg_1.pos0.x) * (seg_1.pos0.y - seg_1.pos1.y) - (seg_0.pos0.y - seg_1.pos0.y) * (seg_1.pos0.x - seg_1.pos1.x);
    auto const num1 = (seg_0.pos0.x - seg_0.pos1.x) * (seg_0.pos0.y - seg_1.pos0.y) - (seg_0.pos0.y - seg_0.pos1.y) * (seg_0.pos0.x - seg_1.pos0.x);
    auto const t = num0 / denom;
    auto const u = -num1 / denom;
    if (ScalarT(0) <= t && t <= ScalarT(1) && ScalarT(0) <= u && u <= ScalarT(1))
        return pair<ScalarT, ScalarT>{t, u};
    return {};
}

template <class ScalarT>
[[nodiscard]] constexpr optional<ScalarT> intersection_parameter(segment<2, ScalarT> const& seg_0, segment<2, ScalarT> const& seg_1)
{
    auto ip = intersection_parameters(seg_0, seg_1);
    if (ip.has_value())
        return ip.value().first;
    return {};
}

template <class ScalarT>
[[nodiscard]] constexpr ScalarT intersection_parameter(line<2, ScalarT> const& l0, line<2, ScalarT> const& l1)
{
    auto M = tg::mat<2, 2, ScalarT>::from_cols(l0.dir, -l1.dir);
    auto t = inverse(M) * (l1.pos - l0.pos);
    return t.x;
}

template <class ScalarT>
[[nodiscard]] constexpr pair<ScalarT, ScalarT> intersection_parameters(line<2, ScalarT> const& l0, line<2, ScalarT> const& l1)
{
    auto M = tg::mat<2, 2, ScalarT>::from_cols(l0.dir, -l1.dir);
    auto t = inverse(M) * (l1.pos - l0.pos);
    return {t.x, t.y};
}

template <int D, class ScalarT>
[[nodiscard]] constexpr optional<aabb<D, ScalarT>> intersection(aabb<D, ScalarT> const& a, aabb<D, ScalarT> const& b)
{
    for (auto i = 0; i < D; ++i)
    {
        if (a.max[i] < b.min[i])
            return {};

        if (b.max[i] < a.min[i])
            return {};
    }

    // overlap!
    aabb<D, ScalarT> res;

    for (auto i = 0; i < D; ++i)
    {
        res.min[i] = max(a.min[i], b.min[i]);
        res.max[i] = min(a.max[i], b.max[i]);
    }

    return {res};
}

template <class ScalarT>
[[nodiscard]] constexpr ScalarT intersection_parameter(line<3, ScalarT> const& l, plane<3, ScalarT> const& p)
{
    return (p.dis - dot(l.pos, p.normal)) / dot(l.dir, p.normal);
}

template <class ScalarT>
[[nodiscard]] constexpr pos<3, ScalarT> intersection(plane<3, ScalarT> const& a, plane<3, ScalarT> const& b, plane<3, ScalarT> const& c)
{
    return intersection(intersection(a, b), c);
}

template <int D, class ScalarT>
[[nodiscard]] constexpr optional<ScalarT> intersection_parameter(segment<D, ScalarT> const& a, plane<D, ScalarT> const& p)
{
    auto denom = dot(p.normal, a.pos1 - a.pos0);
    if (denom == ScalarT(0))
        return {};

    auto t = (p.dis - dot(p.normal, a.pos0)) / denom;
    if (t < 0 || t > 1)
        return {};
    return t;
}

template <int D, class ScalarT>
[[nodiscard]] constexpr optional<segment<D, ScalarT>> intersection(segment<D, ScalarT> const& a, sphere<D, ScalarT> const& b)
{
    static_assert(always_false<ScalarT>, "not implemented");
    (void)a;
    (void)b;
    return {}; // TODO
}

template <int D, class ScalarT>
[[nodiscard]] constexpr optional<segment<D, ScalarT>> intersection(sphere<D, ScalarT> const& b, segment<D, ScalarT> const& a)
{
    return intersection(b, a);
}


// ====================================== Checks if Object Intersects aabb ======================================

template <int D, class ScalarT>
[[nodiscard]] constexpr bool intersects(sphere<D, ScalarT> const& a, aabb<D, ScalarT> const& b)
{
    auto const b_min = b.min;
    auto const b_max = b.max;
    auto const c = a.center;
    auto const clamped_sqr = [](ScalarT v) {
        v = tg::max(ScalarT(0), v);
        return v * v;
    };

    auto d_min = ScalarT(0);

    if constexpr (D >= 1)
    {
        d_min += clamped_sqr(b_min.x - c.x);
        d_min += clamped_sqr(c.x - b_max.x);
    }

    if constexpr (D >= 2)
    {
        d_min += clamped_sqr(b_min.y - c.y);
        d_min += clamped_sqr(c.y - b_max.y);
    }

    if constexpr (D >= 3)
    {
        d_min += clamped_sqr(b_min.z - c.z);
        d_min += clamped_sqr(c.z - b_max.z);
    }

    if constexpr (D >= 4)
    {
        d_min += clamped_sqr(b_min.w - c.w);
        d_min += clamped_sqr(c.w - b_max.w);
    }

    return d_min <= a.radius * a.radius;
}

template <class ScalarT>
[[nodiscard]] constexpr bool intersects(triangle<2, ScalarT> const& a, aabb<2, ScalarT> const& b)
{
    if (!intersects(aabb_of(a), b))
        return false;

    auto p0 = a.pos0;
    auto p1 = a.pos1;
    auto p2 = a.pos2;
    if (contains(b, p0) || contains(b, p1) || contains(b, p2))
        return true;

    auto aabb_pts = vertices_of(b);

    auto const is_separate = [&](pos<2, ScalarT> pa, vec<2, ScalarT> n, pos<2, ScalarT> pb) {
        auto da = dot(n, pa);
        auto db = dot(n, pb);

        // TODO: faster
        auto a_min = min(da, db);
        auto a_max = max(da, db);

        auto b_min = dot(n, aabb_pts[0]);
        auto b_max = b_min;
        for (auto i = 1; i < 4; ++i)
        {
            auto d = dot(n, aabb_pts[i]);
            b_min = min(b_min, d);
            b_max = max(b_max, d);
        }

        if (b_max < a_min || b_min > a_max)
            return true;

        return false;
    };

    if (is_separate(p0, perpendicular(p1 - p0), p2))
        return false;
    if (is_separate(p1, perpendicular(p2 - p1), p0))
        return false;
    if (is_separate(p2, perpendicular(p0 - p2), p1))
        return false;

    return true;
}

// NOTE: does NOT work for integer objects
template <class ScalarT>
[[nodiscard]] constexpr bool intersects(triangle<3, ScalarT> const& tri_in, aabb<3, ScalarT> const& bb_in)
{
    using pos_t = pos<3, ScalarT>;
    using vec_t = vec<3, ScalarT>;

    auto const center = centroid_of(bb_in);
    auto const amin = pos_t(bb_in.min - center);
    auto const amax = pos_t(bb_in.max - center);
    auto const bb = aabb<3, ScalarT>(amin, amax);

    auto const p0 = pos_t(tri_in.pos0 - center);
    auto const p1 = pos_t(tri_in.pos1 - center);
    auto const p2 = pos_t(tri_in.pos2 - center);

    // early out: AABB vs tri AABB
    auto tri_aabb = aabb_of(p0, p1, p2);
    if (tri_aabb.max.x < amin.x || tri_aabb.max.y < amin.y || tri_aabb.max.z < amin.z || //
        tri_aabb.min.x > amax.x || tri_aabb.min.y > amax.y || tri_aabb.min.z > amax.z)
        return false;

    auto const proper_contains = [](aabb<3, ScalarT> const& b, pos_t const& p) {
        return b.min.x < p.x && p.x < b.max.x && //
               b.min.y < p.y && p.y < b.max.y && //
               b.min.z < p.z && p.z < b.max.z;
    };

    // early in: tri points vs AABB
    if (proper_contains(bb, p0) || proper_contains(bb, p1) || proper_contains(bb, p2))
        return true;

    // get adjusted tri base plane
    auto plane = tg::plane<3, ScalarT>(normal_of(tri_in), p0);

    // fast plane / AABB test
    {
        auto pn = plane.normal;
        auto bn = abs(pn.x * amax.x) + abs(pn.y * amax.y) + abs(pn.z * amax.z);

        // min dis: d - bn
        if (bn < -plane.dis)
            return false;

        // max dis: d + bn
        if (-plane.dis < -bn)
            return false;
    }

    // 9 axis SAT test
    {
        auto const is_seperating = [amax](vec<3, ScalarT> const& n, pos_t const& tp0, pos_t const& tp1) -> bool {
            if (tg::is_zero_vector(n))
                return false; // not a real candidate axis

            // fast point / AABB separation test
            auto bn = abs(n.x * amax.x) + abs(n.y * amax.y) + abs(n.z * amax.z);

            auto tn0 = dot(n, tp0);
            auto tn1 = dot(n, tp1);

            auto tmin = min(tn0, tn1);
            auto tmax = max(tn0, tn1);

            auto bmin = -bn;
            auto bmax = bn;

            if (tmax < bmin)
                return true;
            if (bmax < tmin)
                return true;

            return false;
        };

        if (is_seperating(cross(p1 - p0, vec_t::unit_x), p0, p2))
            return false;
        if (is_seperating(cross(p1 - p0, vec_t::unit_y), p0, p2))
            return false;
        if (is_seperating(cross(p1 - p0, vec_t::unit_z), p0, p2))
            return false;

        if (is_seperating(cross(p2 - p0, vec_t::unit_x), p0, p1))
            return false;
        if (is_seperating(cross(p2 - p0, vec_t::unit_y), p0, p1))
            return false;
        if (is_seperating(cross(p2 - p0, vec_t::unit_z), p0, p1))
            return false;

        if (is_seperating(cross(p1 - p2, vec_t::unit_x), p0, p2))
            return false;
        if (is_seperating(cross(p1 - p2, vec_t::unit_y), p0, p2))
            return false;
        if (is_seperating(cross(p1 - p2, vec_t::unit_z), p0, p2))
            return false;
    }

    // found no separating axis? -> intersection
    return true;
}

} // namespace tg
