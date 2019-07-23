#pragma once

#include <typed-geometry/common/scalar_math.hh>
#include <typed-geometry/detail/operators/ops_pos.hh>
#include <typed-geometry/detail/operators/ops_vec.hh>
#include <typed-geometry/detail/special_values.hh>
#include <typed-geometry/detail/tg_traits.hh>
#include <typed-geometry/types/objects/line.hh>
#include <typed-geometry/types/objects/plane.hh>
#include <typed-geometry/types/objects/segment.hh>
#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/quadric.hh>
#include <typed-geometry/types/vec.hh>

#include "closest_points.hh"

namespace tg
{
// Base case for distance_sqr of point/point
template <int D, class ScalarA, class ScalarB>
TG_NODISCARD constexpr auto distance_sqr(pos<D, ScalarA> const& a, pos<D, ScalarB> const& b) -> decltype(length_sqr(a - b))
{
    return length_sqr(a - b);
}
template <int D, class ScalarA, class ScalarB>
[[deprecated("distance between vectors is ill-defined. did you mean distance_sqr(pos, pos)?")]] TG_NODISCARD constexpr auto distance_sqr(
    vec<D, ScalarA> const& a, vec<D, ScalarB> const& b) -> decltype(length_sqr(a - b))
{
    return length_sqr(a - b);
}

// Default implementation of distance as sqrt(distance_sqr)
template <class A, class B>
TG_NODISCARD constexpr auto distance(A const& a, B const& b) -> decltype(sqrt(distance_sqr(a, b)))
{
    return sqrt(distance_sqr(a, b));
}

// Default implementation of distance_sqr as distance_sqr(ca, cb) for closest points ca and cb
template <class A, class B>
TG_NODISCARD constexpr auto distance_sqr(A const& a, B const& b) -> decltype(length_sqr(closest_points(a, b).first - closest_points(a, b).second))
{
    auto cp = closest_points(a, b);
    return length_sqr(cp.first - cp.second);
}

// Convenience for distance to (0,0,0)
template <class Obj>
TG_NODISCARD constexpr auto distance_to_origin(Obj const& o) -> decltype(distance(o, pos_type_for<Obj>::zero))
{
    return distance(o, pos_type_for<Obj>::zero);
}
template <class Obj>
TG_NODISCARD constexpr auto distance_sqr_to_origin(Obj const& o) -> decltype(distance(o, pos_type_for<Obj>::zero))
{
    return distance_sqr(o, pos_type_for<Obj>::zero);
}


// =========== Object Implementations ===========

// signed distance is positive if p lies above pl, 0 if it lies on the plane and negative if below pl
template <int D, class ScalarT>
TG_NODISCARD constexpr fractional_result<ScalarT> signed_distance(pos<3, ScalarT> const& p, hyperplane<D, ScalarT> const& pl)
{
    return dot(p - pos<D, ScalarT>::zero, pl.normal) - pl.dis;
}

template <int D, class ScalarT>
TG_NODISCARD constexpr fractional_result<ScalarT> distance(pos<3, ScalarT> const& p, hyperplane<D, ScalarT> const& pl)
{
    return abs(signed_distance(p, pl));
}


// =========== Other Implementations ===========

template <class ScalarT, class = enable_if<is_scalar<ScalarT>>>
TG_NODISCARD constexpr ScalarT distance_sqr(ScalarT a, ScalarT b)
{
    auto const d = a - b;
    return d * d;
}

template <class ScalarT>
TG_NODISCARD constexpr ScalarT distance_sqr(pos<2, ScalarT> const& p, quadric<2, ScalarT> const& q)
{
    /// Residual L2 error as given by x^T A x - 2 r^T x + c

    vec<2, ScalarT> Ax = {
        q.A00 * p.x + q.A01 * p.y, //
        q.A01 * p.x + q.A11 * p.y, //
    };

    return dot(vec<2, ScalarT>(p), Ax)        // x^T A x
           - 2 * dot(vec<2, ScalarT>(p), q.r) // - 2 r^T x
           + q.d_sqr;                         // + c
}

template <class ScalarT>
TG_NODISCARD constexpr ScalarT distance_sqr(pos<3, ScalarT> const& p, quadric<3, ScalarT> const& q)
{
    /// Residual L2 error as given by x^T A x - 2 r^T x + c

    vec<3, ScalarT> Ax = {
        q.A00 * p.x + q.A01 * p.y + q.A02 * p.z, //
        q.A01 * p.x + q.A11 * p.y + q.A12 * p.z, //
        q.A02 * p.x + q.A12 * p.y + q.A22 * p.z, //
    };

    return dot(vec<3, ScalarT>(p), Ax)        // x^T A x
           - 2 * dot(vec<3, ScalarT>(p), q.r) // - 2 r^T x
           + q.d_sqr;                         // + c
}

template <int D, class ScalarT>
TG_NODISCARD constexpr ScalarT distance_sqr(quadric<D, ScalarT> const& q, pos<D, ScalarT> const& p)
{
    return distance_sqr(p, q);
}

} // namespace tg
