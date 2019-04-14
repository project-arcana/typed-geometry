#pragma once

#include <cmath>

#include "../../types/objects/box.hh"
#include "../../types/objects/triangle.hh"
#include "../../types/pos.hh"

// For a given primitive and a position, return whether the first contains the latter
// contains(a, b) is true iff a contains b

// NOTE:
// contains takes an epsilon parameter that roughly means:
//   contains(a, b) if distance(a, p) <= eps

// Contained functions:
// - contains
//      - bbox
//      - triangle

namespace tg
{
// default implementation if distance(a, pos) is available
template <class A, int D, class ScalarT>
TG_NODISCARD constexpr auto contains(A const& a, pos<D, ScalarT> const& p, ScalarT eps = ScalarT(0)) -> decltype(distance(a, p), false)
{
    return distance(a, p) <= eps;
}

template <int D, class ScalarT>
TG_NODISCARD constexpr bool contains(pos<D, ScalarT> const& b, pos<D, ScalarT> const& o, ScalarT eps = ScalarT(0))
{
    if (eps > 0)
        return distance2(b, o) < eps * eps;
    return b == o;
}

template <class ScalarT>
TG_NODISCARD constexpr bool contains(box<1, ScalarT> const& b, ScalarT const& o, ScalarT eps = ScalarT(0))
{
    return b.min.x - eps <= o && o <= b.max.x + eps;
}
template <class ScalarT>
TG_NODISCARD constexpr bool contains(box<1, ScalarT> const& b, pos<1, ScalarT> const& o, ScalarT eps = ScalarT(0))
{
    return b.min.x - eps <= o.x && o.x <= b.max.x + eps;
}
template <class ScalarT>
TG_NODISCARD constexpr bool contains(box<2, ScalarT> const& b, pos<2, ScalarT> const& o, ScalarT eps = ScalarT(0))
{
    return b.min.x - eps <= o.x && o.x <= b.max.x + eps && //
           b.min.y - eps <= o.y && o.y <= b.max.y + eps;
}
template <class ScalarT>
TG_NODISCARD constexpr bool contains(box<3, ScalarT> const& b, pos<3, ScalarT> const& o, ScalarT eps = ScalarT(0))
{
    return b.min.x - eps <= o.x && o.x <= b.max.x + eps && //
           b.min.y - eps <= o.y && o.y <= b.max.y + eps && //
           b.min.z - eps <= o.z && o.z <= b.max.z + eps;
}
template <class ScalarT>
TG_NODISCARD constexpr bool contains(box<4, ScalarT> const& b, pos<4, ScalarT> const& o, ScalarT eps = ScalarT(0))
{
    return b.min.x - eps <= o.x && o.x <= b.max.x + eps && //
           b.min.y - eps <= o.y && o.y <= b.max.y + eps && //
           b.min.z - eps <= o.z && o.z <= b.max.z + eps && //
           b.min.w - eps <= o.w && o.w <= b.max.w + eps;
}

template <int D, class ScalarT>
TG_NODISCARD constexpr bool contains(box<D, ScalarT> const& b, box<D, ScalarT> const& o, ScalarT eps = ScalarT(0))
{
    return contains(b, o.min, eps) && contains(b, o.max, eps);
}

// Note that eps is used to compare 2D areas, not 1D lengths
template <class ScalarT>
TG_NODISCARD constexpr bool contains(triangle<2, ScalarT> const& t, pos<2, ScalarT> const& p, ScalarT eps = ScalarT(0))
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

template <class ScalarT>
TG_NODISCARD constexpr bool contains(triangle<3, ScalarT> const& t, pos<3, ScalarT> const& p)
{
    // TODO
    // use eps?
    // does this also work for triangles where vertices are not ordered cc? should it?

    auto n = normal(t);

    // checking whether point lies on right side of any edge
    auto e = t.v1 - t.v0;
    auto C = cross(e, p - t.v0);
    if (dot(n, C) < 0)
        return false;

    e = t.v2 - t.v1;
    C = cross(e, p - t.v1);
    if (dot(n, C) < 0)
        return false;

    e = t.v0 - t.v2;
    C = cross(e, p - t.v2);
    if (dot(n, C) < 0)
        return false;

    // point always on left side
    return true;
}

} // namespace tg
