#pragma once

#include <typed-geometry/detail/optional.hh>
#include <typed-geometry/feature/assert.hh>
#include <typed-geometry/functions/basic/scalar_math.hh>

#include <typed-geometry/types/objects/capsule.hh>
#include <typed-geometry/types/objects/cylinder.hh>
#include <typed-geometry/types/objects/plane.hh>
#include <typed-geometry/types/objects/ray.hh>
#include <typed-geometry/types/objects/segment.hh>
#include <typed-geometry/types/objects/sphere.hh>
#include <typed-geometry/types/objects/triangle.hh>

#include <typed-geometry/functions/vector/cross.hh>
#include <typed-geometry/functions/vector/dot.hh>
#include <typed-geometry/functions/vector/length.hh>

#include "aabb.hh"
#include "closest_points.hh"
#include "contains.hh"
#include "direction.hh"
#include "normal.hh"

// family of intersection functions:

// intersects(a, b)              -> bool
// intersection(a, b)            -> ???
// intersection_safe(a, b)       -> optional<???>
// intersection_parameter(a, b)  -> coords?
// intersection_parameters(a, b) -> pair<coords, coords>?
// intersection_exact(a, b)      -> variant
// closest_intersection(a, b)            -> ???
// closest_intersection_parameter(a, b)  -> coords?

// "intersects" returns true iff any point lies in a and in b
// "intersection" returns an object describing the intersection (NOTE: does NOT handle degenerate cases)
// "intersection_safe" is the same as "intersection" but returns nullopt for degenerate cases
// "intersection_parameter" returns a coordinates for the first object such that a[coords] == intersection(a, b)
// "intersection_parameters" returns coordinates for both objects
// "intersection_exact" returns a variant type describing all possible intersections, including degenerate cases
// the "closest_" variants only return the closest intersection for objects where that concept is applicable (e.g. for rays)

// Notes:
//  - intersection_exact is currently unsupported
//  - intersection_safe is currently unsupported
//  - for more elaborate ray-tracing, a future ray_cast function will exist (which also returns the intersection normal)


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

    int size() const { return _size; }
    bool any() const { return _size > 0; }

    HitT const& operator[](int idx)
    {
        TG_ASSERT(0 <= idx && idx < _size);
        return _hit[idx];
    }

    HitT const* begin() const { return _hit; }
    HitT const* end() const { return _hit + _size; }

    ray_hits() = default;
    ray_hits(HitT* hits, int size) : _size(size)
    {
        for (auto i = 0; i < size; ++i)
            _hit[i] = hits[i];
    }

private:
    int _size = 0;
    HitT _hit[MaxHits];
};


// ====================================== Default Implementations ======================================
// TODO: intersection_parameter from intersection_parameters

// returns whether two objects intersect
template <class A, class B>
[[nodiscard]] constexpr auto intersects(A const& a, B const& b) -> decltype(intersection(a, b).has_value())
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

// if ray_hits intersection parameter is available, use that
template <int D, class ScalarT, class Obj>
[[nodiscard]] constexpr auto closest_intersection_parameter(ray<D, ScalarT> const& r, Obj const& obj)
    -> enable_if<decltype(intersection_parameter(r, obj))::is_ray_hits, optional<ScalarT>>
{
    auto hits = intersection_parameter(r, obj);
    if (hits.any())
        return hits[0];
    return {};
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


// ====================================== Ray - Object Intersections ======================================

// ray - plane
template <int D, class ScalarT>
[[nodiscard]] constexpr optional<ScalarT> intersection_parameter(ray<D, ScalarT> const& r, plane<D, ScalarT> const& p)
{
    // if plane normal and ray direction are parallel there is no intersection
    auto dotND = dot(p.normal, r.dir);
    if (dotND == ScalarT(0))
        return {};

    // plane: <x, p.normal> = p.dis
    // ray: x = r.origin + t * r.dir
    // =>
    // <r.origin + t * r.dir, p.normal> = p.dis
    // t = (p.dis - <r.origin, p.normal>) / <r.dir, p.normal>

    auto t = (p.dis - dot(p.normal, r.origin)) / dotND;

    // check whether plane lies behind ray
    if (t < 0)
        return {};

    return t;
}

// returns closest intersection point(s) of ray and sphere
template <int D, class ScalarT>
[[nodiscard]] constexpr optional<ScalarT> closest_intersection_parameter(ray<D, ScalarT> const& r, sphere<D, ScalarT> const& s)
{
    auto t = dot(s.center - r.origin, r.dir);

    auto d_sqr = distance_sqr(r[t], s.center);
    auto r_sqr = s.radius * s.radius;
    if (d_sqr > r_sqr)
        return {};

    auto dt = sqrt(r_sqr - d_sqr);

    if (t - dt >= 0)
        return t - dt;
    if (t + dt >= 0)
        return t + dt;

    return {};
}

// returns intersection point(s) of ray and sphere
template <int D, class ScalarT>
[[nodiscard]] constexpr ray_hits<2, ScalarT> intersection_parameter(ray<D, ScalarT> const& r, sphere<D, ScalarT> const& s)
{
    auto t = dot(s.center - r.origin, r.dir);

    auto d_sqr = distance_sqr(r[t], s.center);
    auto r_sqr = s.radius * s.radius;
    if (d_sqr > r_sqr)
        return {};

    auto dt = sqrt(r_sqr - d_sqr);

    ScalarT hits[2];

    if (t - dt >= 0)
    {
        hits[0] = t - dt;
        hits[1] = t + dt;
        return {hits, 2};
    }

    if (t + dt >= 0)
    {
        hits[0] = t + dt;
        return {hits, 1};
    }

    return {};
}

// ray - disk
template <class ScalarT>
[[nodiscard]] constexpr optional<ScalarT> intersection_parameter(ray<3, ScalarT> const& r, sphere<2, ScalarT, 3> const& d)
{
    auto const t = intersection_parameter(r, plane<3, ScalarT>(d.normal, d.center));
    if (!t.has_value())
        return {};

    auto const p = r[t.value()];
    if (distance_sqr(p, d.center) > d.radius * d.radius)
        return {};

    return t;
}

// ray - quadric (as an isosurface, not error quadric)
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

    ScalarT hits[2];

    if (tMin >= ScalarT(0))
    {
        hits[0] = tMin;
        hits[1] = tMax;
        return {hits, 2};
    }

    if (tMax >= ScalarT(0))
    {
        hits[0] = tMax;
        return {hits, 1};
    }

    return {};
}

// ray - cylinder
template <class ScalarT>
[[nodiscard]] constexpr optional<ScalarT> closest_intersection_parameter(ray<3, ScalarT> const& r, cylinder<3, ScalarT> const& c)
{
    const auto caps = caps_of(c);
    const auto t_cap0 = intersection_parameter(r, caps[0]);
    const auto t_cap1 = intersection_parameter(r, caps[1]);
    const auto t_cyl = closest_intersection_parameter(r, boundary_no_caps_of(c));

    optional<ScalarT> t;

    if (t_cyl.has_value())
        t = t_cyl.value();

    if (t_cap0.has_value() && (!t.has_value() || t.value() > t_cap0.value()))
        t = t_cap0;

    if (t_cap1.has_value() && (!t.has_value() || t.value() > t_cap1.value()))
        t = t_cap1;

    TG_INTERNAL_ASSERT(!t.has_value() || t.value() >= 0);

    return t;
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

// ray - triangle
template <class ScalarT>
[[nodiscard]] constexpr optional<ScalarT> intersection_parameter(ray<3, ScalarT> const& r,
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
    return (lambda > ScalarT(0)) ? lambda : tg::optional<ScalarT>();
}

// ray - box
template <class ScalarT>
[[nodiscard]] constexpr ray_hits<2, ScalarT> intersection_parameter(ray<3, ScalarT> const& r, box<3, ScalarT> const& b)
{
    // see https://github.com/gszauer/GamePhysicsCookbook/blob/master/Code/Geometry3D.cpp

    auto const p = b.center - r.origin;

    // TODO: without normalize
    auto const X = normalize(b.half_extents[0]);
    auto const Y = normalize(b.half_extents[1]);
    auto const Z = normalize(b.half_extents[2]);

    auto f = tg::vec3(dot(X, r.dir), //
                      dot(Y, r.dir), //
                      dot(Z, r.dir));

    auto const e = tg::vec3(dot(X, p), //
                            dot(Y, p), //
                            dot(Z, p));

    auto const size = tg::vec3(length(b.half_extents[0]), //
                               length(b.half_extents[1]), //
                               length(b.half_extents[2]));

    float t[6] = {0, 0, 0, 0, 0, 0};
    for (int i = 0; i < 3; ++i)
    {
        if (abs(f[i]) < tg::epsilon<ScalarT> / 1000)
        {
            if (-e[i] - size[i] > 0 || -e[i] + size[i] < 0)
                return {};

            f[i] = tg::epsilon<ScalarT>; // Avoid div by 0!
        }

        t[i * 2 + 0] = (e[i] + size[i]) / f[i]; // tmin[x, y, z]
        t[i * 2 + 1] = (e[i] - size[i]) / f[i]; // tmax[x, y, z]
    }

    auto const tmin = tg::max(tg::max(tg::min(t[0], t[1]), tg::min(t[2], t[3])), tg::min(t[4], t[5]));
    auto const tmax = tg::min(tg::min(tg::max(t[0], t[1]), tg::max(t[2], t[3])), tg::max(t[4], t[5]));

    // if tmax < 0, ray is intersecting AABB
    // but entire AABB is behing it's origin
    if (tmax < 0)
        return {};

    // if tmin > tmax, ray doesn't intersect AABB
    if (tmin > tmax)
        return {};

    ScalarT hits[2];

    if (tmin >= 0) // two valid intersections
    {
        hits[0] = tmin;
        hits[1] = tmax;
        return {hits, 2};
    }
    else // one valid intersection
    {
        hits[0] = tmax;
        return {hits, 1};
    }
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
template <int D, class ScalarT>
[[nodiscard]] constexpr bool intersects(aabb<D, ScalarT> const& a, sphere<D, ScalarT> const& b)
{
    return intersects(b, a);
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
template <class ScalarT>
[[nodiscard]] constexpr bool intersects(aabb<2, ScalarT> const& a, triangle<2, ScalarT> const& b)
{
    return intersects(b, a);
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
template <class ScalarT>
[[nodiscard]] constexpr bool intersects(aabb<3, ScalarT> const& a, triangle<3, ScalarT> const& b)
{
    return intersects(b, a);
}

} // namespace tg
