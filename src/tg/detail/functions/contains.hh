#pragma once

#include "../../types/box.hh"
#include "../../types/pos.hh"
#include "../../types/triangle.hh"

// For a given primitive and a position, return whether the first contains the latter

// Contained functions:
// - contains
//      - bbox
//      - triangle

namespace tg
{
template <class ScalarT>
constexpr bool contains(box<1, ScalarT> const& b, pos<1, ScalarT> const& o)
{
    return b.min.x <= o.x && o.x <= b.max.x;
}
template <class ScalarT>
constexpr bool contains(box<2, ScalarT> const& b, pos<2, ScalarT> const& o)
{
    return b.min.x <= o.x && o.x <= b.max.x && //
           b.min.y <= o.y && o.y <= b.max.y;
}
template <class ScalarT>
constexpr bool contains(box<3, ScalarT> const& b, pos<3, ScalarT> const& o)
{
    return b.min.x <= o.x && o.x <= b.max.x && //
           b.min.y <= o.y && o.y <= b.max.y && //
           b.min.z <= o.z && o.z <= b.max.z;
}
template <class ScalarT>
constexpr bool contains(box<4, ScalarT> const& b, pos<4, ScalarT> const& o)
{
    return b.min.x <= o.x && o.x <= b.max.x && //
           b.min.y <= o.y && o.y <= b.max.y && //
           b.min.z <= o.z && o.z <= b.max.z && //
           b.min.w <= o.w && o.w <= b.max.w;
}

template <int D, class ScalarT>
constexpr bool contains(box<D, ScalarT> const& b, box<D, ScalarT> const& o)
{
    return contains(b, o.min) && contains(b, o.max);
}

template <class ScalarT>
constexpr bool contains(triangle<2, ScalarT> const& t, pos<2, ScalarT> const& p)
{
    auto pv0 = t.v0 - p;
    auto pv1 = t.v1 - p;
    auto pv2 = t.v2 - p;
    auto A0 = cross(pv1, pv2);
    auto A1 = cross(pv2, pv0);
    auto A2 = cross(pv0, pv1);

    return ((A0 >= 0) == (A1 >= 0)) && ((A1 >= 0) == (A2 >= 0));
}

} // namespace tg
