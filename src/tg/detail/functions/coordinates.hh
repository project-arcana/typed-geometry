#pragma once

#include "../../types/pos.hh"
#include "../../types/objects/segment.hh"
#include "../../types/objects/triangle.hh"
#include "../operators/ops_vec.hh"
#include "../utility.hh"
#include "length.hh"

// For a given primitive and a position, return the position's relative coordinates

// Contained functions:
// - coords
//      - triangle (barycoords)
//      - segment (t-parameter)

// Note: Passing a position that does not lie inside the given primitive,
// the behavior is (yet) undefined
namespace tg
{
template <class ScalarT>
constexpr array<ScalarT, 3> coordinates(triangle<2, ScalarT> const& t, pos<2, ScalarT> const& p)
{
    auto pv0 = t.v0 - p;
    auto pv1 = t.v1 - p;
    auto pv2 = t.v2 - p;
    auto A = cross(t.v1 - t.v0, t.v2 - t.v0);
    auto A0 = cross(pv1, pv2);
    auto A1 = cross(pv2, pv0);
    auto A2 = cross(pv0, pv1);

    auto A_inv = ScalarT(1.0) / A;
    return {{A0 * A_inv, A1 * A_inv, A2 * A_inv}};
}

template <int D, class ScalarT>
constexpr ScalarT coordinates(segment<D, ScalarT> const& s, pos<D, ScalarT> const& p)
{
    auto d = s.b - s.a;
    auto t = dot(p - s.a, d) / dot(d, d);
    return t;
}


} // namespace tg
