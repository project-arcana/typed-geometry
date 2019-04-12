#pragma once

#include "../../types/objects/ray.hh"
#include "../../types/objects/triangle.hh"
#include "../../types/objects/hyperplane.hh"

#include "contains.hh"
#include "dot.hh"
#include "normal.hh"

namespace tg
{
template <class A, class B>
struct intersection_result;

template <int D, class ScalarT>
struct intersection_result<ray<3, ScalarT>, hyperplane<D, ScalarT>>
{
    bool empty;
    pos<D, ScalarT> pos;
};

template <class ScalarT>
struct intersection_result<ray<3, ScalarT>, triangle<3, ScalarT>>
{
    bool empty;
    pos<3, ScalarT> pos;
};

// returns whether two objects intersect
template <class A, class B>
constexpr auto intersects(A const& a, B const& b) -> decltype(!intersection(a, b).empty)
{
    return !intersection(a, b).empty;
}

// returns intersection point of ray and hyperplane
template <int D, class ScalarT>
constexpr auto intersection(ray<3, ScalarT> const& r, hyperplane<D, ScalarT> const& p)
    -> intersection_result<ray<3, ScalarT>, hyperplane<D, ScalarT>>
{
    // if plane normal and raydirection are parallel there is no intersection
    auto dotND = dot(p.n, r.dir);
    if (dotND == 0)
        return {true, {}};

    auto t = -(dot(p.n, vec3(r.pos)) + p.d) / dotND;

    // check whether plane lies behind ray
    if (t < 0)
        return {true, {}};

    auto result = r.pos + r.dir * t;

    // non-empty intersection
    return {false, result};
}

// returns intersection point of ray and triangle
template <class ScalarT>
constexpr auto intersection(ray<3, ScalarT> const& r, triangle<3, ScalarT> const& t)
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
} // namespace tg
