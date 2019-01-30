#pragma once

#include "../../types/objects/line.hh"
#include "../../types/objects/plane.hh"
#include "../../types/objects/segment.hh"
#include "../../types/pos.hh"
#include "../../types/vec.hh"
#include "../operators/ops_pos.hh"
#include "../operators/ops_vec.hh"
#include "../scalars/scalar_math.hh"
#include "clamp.hh"
#include "closest_points.hh"
#include "coordinates.hh"

namespace tg
{
// Base case for distance2 of point/point
template <int D, class ScalarT>
constexpr squared_result<ScalarT> distance2(pos<D, ScalarT> const& a, pos<D, ScalarT> const& b)
{
    return length2(a - b);
}

// Default implementation of distance as sqrt(distance2)
template <class A, class B>
constexpr auto distance(A const& a, B const& b) -> decltype(sqrt(distance2(a, b)))
{
    return sqrt(distance2(a, b));
}

// Default implementation of distance2 as distance2(ca, cb) for closest points ca and cb
template <class A, class B>
constexpr auto distance2(A const& a, B const& b) -> decltype(length2(closest_points(a, b).first - closest_points(a, b).second))
{
    auto cp = closest_points(a, b);
    return length2(cp.first - cp.second);
}


// =========== Object Implementations ===========

// signed distance is positive if p lies above pl, 0 if it lies on the plane and negative if below pl
template <class ScalarT>
constexpr fractional_result<ScalarT> signed_distance(pos<3, ScalarT> const& p, plane const& pl)
{
    return dot(p - pos<3, ScalarT>::zero, pl.n) - pl.d;
}

template <class ScalarT>
constexpr fractional_result<ScalarT> distance(pos<3, ScalarT> const& p, plane const& pl)
{
    return abs(signed_distance(p, pl));
}
} // namespace tg
