#pragma once
#include "../../types/line.hh"
#include "../../types/plane.hh"
#include "../../types/pos.hh"
#include "../../types/segment.hh"
#include "../../types/vec.hh"
#include "../operators/ops_pos.hh"
#include "../operators/ops_vec.hh"
#include "../scalars/scalar_math.hh"
#include "clamp.hh"
#include "closest_points.hh"
#include "coordinates.hh"

namespace tg
{
template <int D, class ScalarT>
constexpr squared_result<ScalarT> distance2(pos<D, ScalarT> const& a, pos<D, ScalarT> const& b)
{
    return length2(a - b);
}

template <int D, class ScalarT>
constexpr squared_result<ScalarT> distance2(pos<D, ScalarT> const& p, segment<D, ScalarT> const& s)
{
    auto closestPointOnSegment = closest_points(s, p).first;
    return distance2(p, closestPointOnSegment);
}

template <int D, class ScalarT>
constexpr fractional_result<ScalarT> distance(pos<D, ScalarT> const& p, segment<D, ScalarT> const& s)
{
    return sqrt(distance2(p, s));
}

template <int D, class ScalarT>
constexpr fractional_result<ScalarT> distance(pos<D, ScalarT> const& a, pos<D, ScalarT> const& b)
{
    return sqrt(distance2(a, b));
}

// signed distance is positive if p lies above pl, 0 if it lies on the plane and negative if below pl
template <class ScalarT>
constexpr fractional_result<ScalarT> signed_distance(pos<3, ScalarT> const& p, plane<ScalarT> const& pl)
{
    return dot(p, pl.n) - pl.d;
}

template <class ScalarT>
constexpr fractional_result<ScalarT> distance(pos<3, ScalarT> const& p, plane<ScalarT> const& pl)
{
    return abs(signed_distance(p, pl));
}
} // namespace tg
