#pragma once

#include <typed-geometry/common/assert.hh>

#include <typed-geometry/types/objects/circle.hh>
#include <typed-geometry/types/objects/hyperplane.hh>
#include <typed-geometry/types/objects/ray.hh>
#include <typed-geometry/types/objects/sphere.hh>
#include <typed-geometry/types/objects/triangle.hh>

#include "contains.hh"
#include "dot.hh"
#include "normal.hh"

// family of intersection functions:

// intersects(a, b) -> bool    iff any point lies in a and in b
// intersection(a, b) -> ???   returns an object describing the intersection

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
    auto dotND = dot(p.n, r.dir);
    if (dotND == 0)
        return {true, {}};

    auto t = -(dot(p.n, vec<D, ScalarT>(r.pos)) + p.d) / dotND;

    // check whether plane lies behind ray
    if (t < 0)
        return {true, {}};

    auto result = r.pos + r.dir * t;

    // non-empty intersection
    return {false, result};
}

// returns intersection point of ray and triangle
template <class ScalarT>
TG_NODISCARD constexpr auto intersection(ray<3, ScalarT> const& r, triangle<3, ScalarT> const& t)
    -> intersection_result<tg::ray<3, ScalarT>, tg::triangle<3, ScalarT>>
{
    auto p = hyperplane<3, ScalarT>(normal(t), t.v0);

    auto result = intersection(r, p);

    // check whether potential intersection point indeed lies on the triangle
    if (!contains(t, result.pos))
        return {true, {}};

    // non-empty intersection
    return result;
}


// returns intersection circle of sphere and sphere (normal points from a to b)
// for now does not work if spheres are identical (result would be a sphere3 again)
template <class ScalarT>
TG_NODISCARD constexpr auto intersection(sphere<3, ScalarT> const& a, sphere<3, ScalarT> const& b)
    -> intersection_result<sphere<3, ScalarT>, sphere<3, ScalarT>>
{
    auto d2 = distance2(a.center, b.center);

    // TODO: intersection sphere
    if (a.center == b.center && a.radius == b.radius)
        return {true, {}};

    auto d = tg::sqrt(d2);

    // no intersection
    if (d > a.radius + b.radius)
        return {true, {}};


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
        return {true, {}};
    }

    TG_ASSERT(d > ScalarT(0));

    // squared radii of a and b
    auto ar2 = a.radius * a.radius;
    auto br2 = b.radius * b.radius;

    auto t = ScalarT(0.5) + (ar2 - br2) / (ScalarT(2) * d2);

    // position and radius of intersection circle
    auto ipos = a.center + t * (b.center - a.center);
    auto irad = sqrt(ar2 - t * t * d2);

    // non-empty intersection (circle)
    return {false, {ipos, irad, (b.center - a.center) / d}};
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

    TG_ASSERT(d > ScalarT(0));

    auto t = (ar * ar - br * br + d2) / (2 * d);
    auto h2 = ar * ar - t * t;
    TG_ASSERT(h2 >= ScalarT(0));

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

} // namespace tg
