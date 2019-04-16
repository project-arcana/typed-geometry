#pragma once

#include "../../types/objects/circle.hh"
#include "../../types/objects/hyperplane.hh"
#include "../../types/objects/ray.hh"
#include "../../types/objects/sphere.hh"
#include "../../types/objects/triangle.hh"

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
template <class ScalarT>
TG_NODISCARD constexpr auto intersection(sphere<3, ScalarT> const& a, sphere<3, ScalarT> const& b)
    -> intersection_result<sphere<3, ScalarT>, sphere<3, ScalarT>>
{
    auto d2 = distance2(a.center, b.center);

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

} // namespace tg
