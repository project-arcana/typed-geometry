#pragma once

#include <cmath>
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
template <int D, class ScalarT>
constexpr bool contains(pos<D, ScalarT> const& b, pos<D, ScalarT> const& o, ScalarT eps = ScalarT(0))
{
    if (eps > 0)
        return distance2(b, o) < eps * eps;
    return b == o;
}

template <class ScalarT>
constexpr bool contains(box<1, ScalarT> const& b, pos<1, ScalarT> const& o, ScalarT eps = ScalarT(0))
{
    return b.min.x - eps <= o.x && o.x <= b.max.x + eps;
}
template <class ScalarT>
constexpr bool contains(box<2, ScalarT> const& b, pos<2, ScalarT> const& o, ScalarT eps = ScalarT(0))
{
    return b.min.x - eps <= o.x && o.x <= b.max.x + eps && //
           b.min.y - eps <= o.y && o.y <= b.max.y + eps;
}
template <class ScalarT>
constexpr bool contains(box<3, ScalarT> const& b, pos<3, ScalarT> const& o, ScalarT eps = ScalarT(0))
{
    return b.min.x - eps <= o.x && o.x <= b.max.x + eps && //
           b.min.y - eps <= o.y && o.y <= b.max.y + eps && //
           b.min.z - eps <= o.z && o.z <= b.max.z + eps;
}
template <class ScalarT>
constexpr bool contains(box<4, ScalarT> const& b, pos<4, ScalarT> const& o, ScalarT eps = ScalarT(0))
{
    return b.min.x - eps <= o.x && o.x <= b.max.x + eps && //
           b.min.y - eps <= o.y && o.y <= b.max.y + eps && //
           b.min.z - eps <= o.z && o.z <= b.max.z + eps && //
           b.min.w - eps <= o.w && o.w <= b.max.w + eps;
}

template <int D, class ScalarT>
constexpr bool contains(box<D, ScalarT> const& b, box<D, ScalarT> const& o, ScalarT eps = ScalarT(0))
{
    return contains(b, o.min, eps) && contains(b, o.max, eps);
}

// Note that eps is used to compare 2D areas, not 1D lengths
template <class ScalarT>
constexpr bool contains(triangle<2, ScalarT> const& t, pos<2, ScalarT> const& p, ScalarT eps = ScalarT(0))
{
    auto pv0 = t.v0 - p;
    auto pv1 = t.v1 - p;
    auto pv2 = t.v2 - p;
    auto A0 = cross(pv1, pv2);
    auto A1 = cross(pv2, pv0);
    auto A2 = cross(pv0, pv1);

    if (eps > 0)
        return (A0 >= -std::copysign(eps, A0) == (A1 >= -std::copysign(eps, A0))) && //
               (A1 >= -std::copysign(eps, A0) == (A2 >= -std::copysign(eps, A0)));

    return ((A0 >= -eps) == (A1 >= -eps)) && ((A1 >= -eps) == (A2 >= -eps));
}

} // namespace tg
