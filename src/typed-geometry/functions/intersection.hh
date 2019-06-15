#pragma once

#include <typed-geometry/common/assert.hh>
#include <typed-geometry/detail/optional.hh>

#include <typed-geometry/types/objects/circle.hh>
#include <typed-geometry/types/objects/hyperplane.hh>
#include <typed-geometry/types/objects/ray.hh>
#include <typed-geometry/types/objects/segment.hh>
#include <typed-geometry/types/objects/sphere.hh>
#include <typed-geometry/types/objects/triangle.hh>

#include "contains.hh"
#include "cross.hh"
#include "dot.hh"
#include "normal.hh"

// family of intersection functions:

// intersects(a, b) -> bool    iff any point lies in a and in b
// intersection(a, b) -> ???   returns an object describing the intersection

// intersection_coordinate(a, b) -> coords?   such that a[coords] == intersection(a, b)
// intersection_coordinates(a, b) -> pair<coords, coords>?   such that a[coords.first] == intersection(a, b) == b[coords.second]

// related function:
//   closest_hit(ray, b) -> optional<pos>

namespace tg
{
template <class A, class B>
struct intersection_result;

template <int D, class ScalarT>
struct intersection_result<ray<3, ScalarT>, hyperplane<D, ScalarT>>
{
    bool empty;
    tg::pos<D, ScalarT> pos;
};

template <class ScalarT>
struct intersection_result<ray<3, ScalarT>, triangle<3, ScalarT>>
{
    bool empty;
    tg::pos<3, ScalarT> pos;
};

template <class ScalarT>
struct intersection_result<ray<3, ScalarT>, sphere<3, ScalarT>>
{
    // empty: 0 intersections
    // !empty && emptyB: 1 intersection (posA)
    // !empty && !emptyB: 2 intersections (posA, posB)
    bool empty;
    bool emptyB;
    tg::pos<3, ScalarT> posA;
    tg::pos<3, ScalarT> posB;
};

template <class ScalarT>
struct intersection_result<sphere<3, ScalarT>, sphere<3, ScalarT>>
{
    bool empty;
    tg::circle<3, ScalarT> circle;
};

template <class ScalarT>
struct intersection_result<circle<2, ScalarT>, circle<2, ScalarT>>
{
    // in general there are 0 (where empty == true) or 2 intersections
    // in the rare case of the circles touching at one position, both posA and posB are identical
    bool empty;
    tg::pos<2, ScalarT> posA;
    tg::pos<2, ScalarT> posB;
};

// returns whether two objects intersect
template <class A, class B>
TG_NODISCARD constexpr auto intersects(A const& a, B const& b) -> decltype(!intersection(a, b).empty)
{
    return !intersection(a, b).empty;
}

// returns intersection point of ray and hyperplane
template <int D, class ScalarT>
TG_NODISCARD constexpr auto intersection(ray<D, ScalarT> const& r, hyperplane<D, ScalarT> const& p)
    -> intersection_result<ray<D, ScalarT>, hyperplane<D, ScalarT>>
{
    // if plane normal and raydirection are parallel there is no intersection
    auto dotND = dot(p.normal, r.dir);
    if (dotND == 0)
        return {true, {}};

    auto t = -(dot(p.normal, vec<D, ScalarT>(r.origin)) + p.dis) / dotND;

    // check whether plane lies behind ray
    if (t < 0)
        return {true, {}};

    auto result = r.origin + r.dir * t;

    // non-empty intersection
    return {false, result};
}

template <class ScalarT>
TG_NODISCARD constexpr optional<ScalarT> intersection_coordinate(ray<3, ScalarT> const& r, triangle<3, ScalarT> const& t);

// returns intersection point of ray and triangle
template <class ScalarT>
TG_NODISCARD constexpr auto intersection(ray<3, ScalarT> const& r, triangle<3, ScalarT> const& t)
    -> intersection_result<tg::ray<3, ScalarT>, tg::triangle<3, ScalarT>>
{
    auto lambda = intersection_coordinate(r, t);
    if (lambda.has_value())
    {
        return {true, r.origin + r.dir * lambda.value()};
    }
    else
        return {false, {}};
}

// returns intersection point(s) of ray and sphere
template <class ScalarT>
TG_NODISCARD constexpr auto intersection(ray<3, ScalarT> const& r, sphere<3, ScalarT> const& s)
    -> intersection_result<tg::ray<3, ScalarT>, tg::sphere<3, ScalarT>>
{
    auto tA = ScalarT(0);
    auto tB = ScalarT(0);
    auto empty = true;
    auto emptyB = true;
    // analytic solution
    auto l = r.origin - s.center;

    auto a = dot(r.dir, r.dir); // as dir is always normalized
    auto b = ScalarT(2) * dot(r.dir, l);
    auto c = dot(l, l) - s.radius * s.radius;

    auto discr = b * b - ScalarT(4) * a * c;
    if (discr < 0) // no intersection
        return {true, true, {}, {}};
    else if (discr == 0)
    { // one intersection
        tA = tB = ScalarT(-0.5) * b / a;
        empty = false;
    }
    else
    { // two intersections
        auto q = (b > 0) ? ScalarT(-0.5) * (b + sqrt(discr)) : ScalarT(-0.5) * (b - sqrt(discr));
        tA = q / a;
        tB = c / q;
        empty = emptyB = false;
    }

    // tA is closer
    if (tA > tB)
    {
        auto t = tA;
        tA = tB;
        tB = t;
    }
    // tA must not be negative
    if (tA < 0)
    {
        // try other t
        tA = tB;
        // also negative
        if (tA < 0)
            return {true, true, {}, {}};
        // clear
        tB = 0;
        emptyB = true;
    }

    return {empty, emptyB, r.origin + r.dir * tA, r.origin + r.dir * tB};
}


// returns intersection circle of sphere and sphere (normal points from a to b)
// for now does not work if spheres are identical (result would be a sphere3 again)
template <class ScalarT>
TG_NODISCARD constexpr auto intersection(sphere<3, ScalarT> const& a, sphere<3, ScalarT> const& b)
    -> intersection_result<sphere<3, ScalarT>, sphere<3, ScalarT>>
{
    auto d2 = distance2(a.center, b.center);

    auto const empty_circle = circle<3, ScalarT>(tg::pos<3, ScalarT>::zero, ScalarT(0), tg::dir<3, ScalarT>::pos_x);

    // TODO: intersection sphere
    if (a.center == b.center && a.radius == b.radius)
        return {true, empty_circle};

    auto d = tg::sqrt(d2);

    // no intersection
    if (d > a.radius + b.radius)
        return {true, empty_circle};


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
        return {true, empty_circle};
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
    return {false, {ipos, irad, dir<3, ScalarT>((b.center - a.center) / d)}};
}

// returns intersection points of two circles in 2D
// for now does not work if circles are identical (result would be a circle2 again)
template <class ScalarT>
TG_NODISCARD constexpr auto intersection(circle<2, ScalarT> const& a, circle<2, ScalarT> const& b)
    -> intersection_result<circle<2, ScalarT>, circle<2, ScalarT>>
{
    // TODO: intersection circle
    if (a.center == b.center && a.radius == b.radius)
        return {true, {}, {}};

    auto d2 = distance2(a.center, b.center);
    auto d = tg::sqrt(d2);
    auto ar = a.radius;
    auto br = b.radius;
    if (ar + br < d) // no intersection
        return {true, {}, {}};

    if (d < tg::abs(ar - br)) // no intersection (one inside the other)
        return {true, {}, {}};

    TG_INTERNAL_ASSERT(d > ScalarT(0));

    auto t = (ar * ar - br * br + d2) / (2 * d);
    auto h2 = ar * ar - t * t;
    TG_INTERNAL_ASSERT(h2 >= ScalarT(0));

    auto h = tg::sqrt(h2);
    auto h_by_d = h / d;

    auto p_between = a.center + t / d * (b.center - a.center);

    auto a_to_b = b.center - a.center;
    auto a_to_b_swap = tg::vec2(a_to_b.y, a_to_b.x);

    // imagining cirlce a on the left side of circle b...
    auto p_above = p_between + h_by_d * a_to_b_swap;
    auto p_below = p_between - h_by_d * a_to_b_swap;

    return {false, p_above, p_below};
}

template <int D, class ScalarT>
TG_NODISCARD constexpr optional<ScalarT> intersection_coordinate(segment<D, ScalarT> const& a, hyperplane<D, ScalarT> const& p)
{
    auto denom = dot(p.normal, a.pos1 - a.pos0);
    if (denom == 0)
        return {};

    auto t = (p.dis - dot(p.normal, a.pos0 - tg::pos<D, ScalarT>::zero)) / denom;
    if (t < 0 || t > 1)
        return {};
    return t;
}

template <int D, class ScalarT>
TG_NODISCARD constexpr optional<ScalarT> intersection_coordinate(ray<D, ScalarT> const& r, hyperplane<D, ScalarT> const& p)
{
    // if plane normal and ray direction are parallel there is no intersection
    auto dotND = dot(p.normal, r.dir);
    if (dotND == 0)
        return {};

	// plane: <x, p.normal> = p.dis
	// ray: x = r.origin + t * r.dir
	// =>
	// <r.origin + t * r.dir, p.normal> = p.dis
	// t = (p.dis - <r.origin, p.normal>) / <r.dir, p.normal>

    auto t = (p.dis - dot(p.normal, vec<D, ScalarT>(r.origin))) / dotND;

    // check whether plane lies behind ray
    if (t < 0)
        return {};

    return t;
}

template <class ScalarT>
TG_NODISCARD constexpr optional<ScalarT> intersection_coordinate(ray<3, ScalarT> const& r, triangle<3, ScalarT> const& t)
{
    auto constexpr eps = 0.000001f;

    auto e1 = t.pos1 - t.pos0;
    auto e2 = t.pos2 - t.pos0;

    auto pvec = tg::cross(tg::vec3(r.dir), e2);
    auto det = dot(pvec, e1);

    if (det < eps)
        return {};

    auto tvec = r.origin - t.pos0;
    auto u = dot(tvec, pvec);
    if (u < 0.f || u > det)
        return {};

    auto qvec = cross(tvec, e1);
    auto v = dot(r.dir, qvec);
    if (v < 0.f || v + u > det)
        return {};

    auto lambda = (1.f / det) * dot(e2, qvec);
    return (lambda > 0) ? lambda : tg::optional<float>();
}

template <int D, class ScalarT>
TG_NODISCARD constexpr optional<ScalarT> intersection_coordinate(ray<D, ScalarT> const& r, sphere<D, ScalarT> const& s)
{
    auto const t = dot(s.center - r.origin, r.dir);
    auto const p = r[t];
    auto const dt = tg::sqrt(s.radius * s.radius - distance2(p, s.center));

    if (t - dt >= 0)
        return t - dt;
    if (t + dt >= 0)
        return t + dt;
    return {};
}

template <class ScalarT>
TG_NODISCARD constexpr optional<ScalarT> intersection_coordinate(ray<3, ScalarT> const& r, box<3, ScalarT> const& b)
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
        if (tg::abs(f[i]) < tg::epsilon<ScalarT> / 1000)
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

    // If tmin is < 0, tmax is closer
    return tmin < 0 ? tmax : tmin;
}

template <class ScalarT>
TG_NODISCARD constexpr optional<ScalarT> intersection_coordinate(ray<3, ScalarT> const& r, disk<3, ScalarT> const& c)
{
    auto const t = intersection_coordinate(r, tg::plane(c.normal, c.center));
    if (!t.has_value())
        return t;

    auto const p = r[t.value()];
    if (distance2(p, c.center) > c.radius * c.radius)
        return {};

    return t;
}

template <class ScalarT>
TG_NODISCARD constexpr optional<ScalarT> intersection_coordinate(ray<3, ScalarT> const& r, cylinder<3, ScalarT> const& c)
{
    optional<ScalarT> t;

    // see
    // https://www.gamedev.net/forums/topic/467789-raycylinder-intersection/

    auto A = c.axis.pos0;
    auto B = c.axis.pos1;
    auto O = r.origin;
    auto V = tg::vec3(r.dir);

    auto AB = (B - A);
    auto AO = (O - A);
    auto AOxAB = cross(AO, AB);     // cross product
    auto VxAB = cross(V, AB);       // cross product
    auto ab2 = dot(AB, AB);         // dot product
    auto pa = dot(VxAB, VxAB);      // dot product
    auto pb = 2 * dot(VxAB, AOxAB); // dot product
    auto pc = dot(AOxAB, AOxAB) - (c.radius * c.radius * ab2);

    // solve second order equation : a*t^2 + b*t + c = 0
    auto p = pb / pa;
    auto q = pc / pa;
    auto sq2 = p * p / 4 - q;

    if (sq2 >= 0)
    {
        auto sq = sqrt(sq2);
        auto ct = -p / 2 - sq >= 0 ? -p / 2 - sq : -p / 2 + sq;

        auto rp = r[ct];
        auto projp = project(rp, line3::from_points(c.axis.pos0, c.axis.pos1));
        auto coord = coordinates(c.axis, projp);
        if (0 <= coord && coord <= 1 && false)
            t = coord;
    }

    auto d = normalize(c.axis.pos1 - c.axis.pos0);

    if (auto ct = intersection_coordinate(r, disk3(c.axis.pos0, c.radius, d)); ct.has_value())
        if (!t.has_value() || ct.value() < t.value())
            t = ct;

    if (auto ct = intersection_coordinate(r, disk3(c.axis.pos1, c.radius, d)); ct.has_value())
        if (!t.has_value() || ct.value() < t.value())
            t = ct;

    return t;
}

} // namespace tg
