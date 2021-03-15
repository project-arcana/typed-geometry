#pragma once

#include <typed-geometry/detail/operators/ops_pos.hh>
#include <typed-geometry/detail/operators/ops_vec.hh>
#include <typed-geometry/detail/special_values.hh>
#include <typed-geometry/detail/tg_traits.hh>
#include <typed-geometry/functions/basic/scalar_math.hh>
#include <typed-geometry/functions/objects/edges.hh>
#include <typed-geometry/functions/objects/vertices.hh>
#include <typed-geometry/functions/vector/distance.hh>
#include <typed-geometry/types/objects/halfspace.hh>
#include <typed-geometry/types/objects/line.hh>
#include <typed-geometry/types/objects/plane.hh>
#include <typed-geometry/types/objects/segment.hh>
#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/quadric.hh>
#include <typed-geometry/types/vec.hh>

#include "closest_points.hh"
#include "intersection.hh"

namespace tg
{
// Default implementation of distance as abs(signed_distance)
// TODO: FIXME via priority_tag<>
// template <class A, class B>
// [[nodiscard]] constexpr auto distance(A const& a, B const& b) -> decltype(abs(signed_distance(a, b)))
// {
//     return abs(signed_distance(a, b));
// }

// Default implementation of distance_sqr as distance_sqr(ca, cb) for closest points ca and cb
template <class A, class B>
[[nodiscard]] constexpr auto distance_sqr(A const& a, B const& b) -> decltype(length_sqr(closest_points(a, b).first - closest_points(a, b).second))
{
    auto cp = closest_points(a, b);
    return length_sqr(cp.first - cp.second);
}

// Convenience for distance to (0,0,0)
template <class Obj>
[[nodiscard]] constexpr auto distance_to_origin(Obj const& o) -> decltype(distance(o, pos_type_for<Obj>::zero))
{
    return distance(o, pos_type_for<Obj>::zero);
}
template <class Obj>
[[nodiscard]] constexpr auto distance_sqr_to_origin(Obj const& o) -> decltype(distance(o, pos_type_for<Obj>::zero))
{
    return distance_sqr(o, pos_type_for<Obj>::zero);
}


// =========== Object Implementations ===========

// signed distance is positive if p lies above pl, 0 if it lies on the plane and negative if below pl
template <int D, class ScalarT>
[[nodiscard]] constexpr fractional_result<ScalarT> signed_distance(pos<D, ScalarT> const& p, plane<D, ScalarT> const& pl)
{
    return dot(p, pl.normal) - pl.dis;
}

template <int D, class ScalarT>
[[nodiscard]] constexpr fractional_result<ScalarT> distance(pos<D, ScalarT> const& p, plane<D, ScalarT> const& pl)
{
    return abs(signed_distance(p, pl));
}

template <int D, class ScalarT>
[[nodiscard]] constexpr fractional_result<ScalarT> signed_distance(pos<D, ScalarT> const& p, halfspace<D, ScalarT> const& h)
{
    return dot(p, h.normal) - h.dis;
}

template <int D, class ScalarT>
[[nodiscard]] constexpr fractional_result<ScalarT> distance(pos<D, ScalarT> const& p, halfspace<D, ScalarT> const& h)
{
    return max(ScalarT(0), signed_distance(p, h));
}

template <int D, class ScalarT>
[[nodiscard]] constexpr fractional_result<ScalarT> signed_distance(pos<D, ScalarT> const& p, sphere_boundary<D, ScalarT> const& s)
{
    return distance(p, s.center) - s.radius;
}

template <class ScalarT>
[[nodiscard]] constexpr fractional_result<ScalarT> distance(line<3, ScalarT> const& l0, line<3, ScalarT> const& l1)
{
    auto n = cross(l0.dir, l1.dir);
    return abs(dot(l0.pos - l1.pos, n) / length(n));
}

template <class ScalarT>
[[nodiscard]] constexpr fractional_result<ScalarT> distance_sqr(segment<2, ScalarT> const& s0, segment<2, ScalarT> const& s1)
{
    auto l0 = inf_of(s0);
    auto l1 = inf_of(s1);
    auto len0 = length(s0);
    auto len1 = length(s1);

    auto [t0, t1] = intersection_parameters(l0, l1);

    if (ScalarT(0) <= t0 && t0 <= len0 && //
        ScalarT(0) <= t1 && t1 <= len1)
        return ScalarT(0); // intersects

    auto p0 = t0 * ScalarT(2) < len0 ? s0.pos0 : s0.pos1;
    auto p1 = t1 * ScalarT(2) < len1 ? s1.pos0 : s1.pos1;

    return min(distance_sqr(p0, s1), distance_sqr(p1, s0));
}

template <class ScalarT>
[[nodiscard]] constexpr fractional_result<ScalarT> distance_sqr(segment<3, ScalarT> const& s0, segment<3, ScalarT> const& s1)
{
    auto l0 = inf_of(s0);
    auto l1 = inf_of(s1);
    auto len0 = length(s0);
    auto len1 = length(s1);

    auto [t0, t1] = closest_points_parameters(l0, l1);

    if (ScalarT(0) <= t0 && t0 <= len0 && //
        ScalarT(0) <= t1 && t1 <= len1)
        return distance_sqr(l0[t0], l1[t1]); // closest points is inside segments

    auto p0 = t0 * ScalarT(2) < len0 ? s0.pos0 : s0.pos1;
    auto p1 = t1 * ScalarT(2) < len1 ? s1.pos0 : s1.pos1;

    return min(distance_sqr(p0, s1), distance_sqr(p1, s0));
}

template <class ScalarT>
[[nodiscard]] constexpr fractional_result<ScalarT> distance_sqr(segment<2, ScalarT> const& s, line<2, ScalarT> const& l)
{
    auto ls = inf_of(s);
    auto len = length(s);

    auto [ts, tl] = intersection_parameters(ls, l);
    if (ScalarT(0) <= ts && ts <= len)
        return ScalarT(0); // intersects

    auto p = ts * ScalarT(2) < len ? s.pos0 : s.pos1;
    return distance_sqr(p, l);
}
template <class ScalarT>
[[nodiscard]] constexpr fractional_result<ScalarT> distance_sqr(segment<3, ScalarT> const& s, line<3, ScalarT> const& l)
{
    auto ls = inf_of(s);
    auto len = length(s);

    auto [ts, tl] = closest_points_parameters(ls, l);
    auto tClamped = clamp(ts, ScalarT(0), len);
    return distance_sqr(ls[tClamped], l);
}
template <int D, class ScalarT>
[[nodiscard]] constexpr fractional_result<ScalarT> distance_sqr(line<D, ScalarT> const& l, segment<D, ScalarT> const& s)
{
    return distance_sqr(s, l);
}

// TODO: use GJK or something?
template <class ScalarT>
[[nodiscard]] constexpr fractional_result<ScalarT> distance(aabb<3, ScalarT> const& bb, triangle<3, ScalarT> const& t)
{
    if (intersects(bb, t))
        return fractional_result<ScalarT>(0);

    auto d = tg::max<ScalarT>();

    // tri vertices to bb
    for (auto p : vertices_of(t))
        d = min(d, distance(bb, p));

    // bb vertices to tri
    for (auto p : vertices_of(bb))
        d = min(d, distance(t, p));

    // edges to edges
    for (auto e0 : edges_of(t))
        for (auto e1 : edges_of(bb))
            d = min(d, distance(e0, e1));

    return d;
}
template <class ScalarT>
[[nodiscard]] constexpr fractional_result<ScalarT> distance(triangle<3, ScalarT> const& t, aabb<3, ScalarT> const& bb)
{
    return distance(bb, t);
}


// =========== Other Implementations ===========

template <class ScalarT, class = enable_if<is_scalar<ScalarT>>>
[[nodiscard]] constexpr ScalarT distance_sqr(ScalarT a, ScalarT b)
{
    auto const d = a - b;
    return d * d;
}

template <class ScalarT>
[[nodiscard]] constexpr ScalarT distance_sqr(pos<2, ScalarT> const& p, quadric<2, ScalarT> const& q)
{
    return q(p);
}

template <class ScalarT>
[[nodiscard]] constexpr ScalarT distance_sqr(pos<3, ScalarT> const& p, quadric<3, ScalarT> const& q)
{
    return q(p);
}

template <int D, class ScalarT>
[[nodiscard]] constexpr ScalarT distance_sqr(quadric<D, ScalarT> const& q, pos<D, ScalarT> const& p)
{
    return distance_sqr(p, q);
}

} // namespace tg
