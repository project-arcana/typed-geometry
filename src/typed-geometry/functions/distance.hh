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
// Base case for distance2 of point/point
template <int D, class ScalarA, class ScalarB>
TG_NODISCARD constexpr auto distance2(pos<D, ScalarA> const& a, pos<D, ScalarB> const& b) -> decltype(length2(a - b))
{
    return length2(a - b);
}

// Default implementation of distance as sqrt(distance2)
template <class A, class B>
TG_NODISCARD constexpr auto distance(A const& a, B const& b) -> decltype(sqrt(distance2(a, b)))
{
    return sqrt(distance2(a, b));
}

// Default implementation of distance2 as distance2(ca, cb) for closest points ca and cb
template <class A, class B>
TG_NODISCARD constexpr auto distance2(A const& a, B const& b) -> decltype(length2(closest_points(a, b).first - closest_points(a, b).second))
{
    auto cp = closest_points(a, b);
    return length2(cp.first - cp.second);
}

// Convience for distance to (0,0,0)
template <class Obj>
TG_NODISCARD constexpr auto distance_to_origin(Obj const& o) -> decltype(distance(o, pos_type_for<Obj>::zero))
{
    return distance(o, pos_type_for<Obj>::zero);
}
template <class Obj>
TG_NODISCARD constexpr auto distance2_to_origin(Obj const& o) -> decltype(distance(o, pos_type_for<Obj>::zero))
{
    return distance2(o, pos_type_for<Obj>::zero);
}


// =========== Object Implementations ===========

// signed distance is positive if p lies above pl, 0 if it lies on the plane and negative if below pl
template <class ScalarT>
TG_NODISCARD constexpr fractional_result<ScalarT> signed_distance(pos<3, ScalarT> const& p, plane const& pl)
{
    return dot(p - zero<pos<3, ScalarT>>(), pl.normal) - pl.dis;
}

template <class ScalarT>
TG_NODISCARD constexpr fractional_result<ScalarT> distance(pos<3, ScalarT> const& p, plane const& pl)
{
    return abs(signed_distance(p, pl));
}


// =========== Other Implementations ===========

template <class ScalarT>
TG_NODISCARD constexpr ScalarT distance2(pos<2, ScalarT> const& p, quadric<2, ScalarT> const& q)
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
TG_NODISCARD constexpr ScalarT distance2(pos<3, ScalarT> const& p, quadric<3, ScalarT> const& q)
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
TG_NODISCARD constexpr ScalarT distance2(quadric<D, ScalarT> const& q, pos<D, ScalarT> const& p)
{
    return distance(p, q);
}

} // namespace tg
