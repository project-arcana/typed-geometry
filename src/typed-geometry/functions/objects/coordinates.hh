#pragma once

#include <typed-geometry/detail/operators/ops_vec.hh>
#include <typed-geometry/types/array.hh>
#include <typed-geometry/types/objects/aabb.hh>
#include <typed-geometry/types/objects/box.hh>
#include <typed-geometry/types/objects/line.hh>
#include <typed-geometry/types/objects/plane.hh>
#include <typed-geometry/types/objects/ray.hh>
#include <typed-geometry/types/objects/segment.hh>
#include <typed-geometry/types/objects/triangle.hh>
#include <typed-geometry/types/pos.hh>

#include <typed-geometry/functions/vector/length.hh>

// For a given primitive and a position, return the position's relative coordinates

// Contained functions:
// - coords
//      - triangle (barycoords)
//      - segment (t-parameter)

// The following identity is useful:
//   p == obj[coordinates(obj, p)]   (for all p in obj)

// Note: Passing a position that does not lie inside the given primitive,
// the behavior is (yet) undefined
namespace tg
{
template <class ScalarT>
[[nodiscard]] constexpr comp<3, ScalarT> coordinates(triangle<2, ScalarT> const& t, pos<2, ScalarT> const& p)
{
    auto pv0 = t.pos0 - p;
    auto pv1 = t.pos1 - p;
    auto pv2 = t.pos2 - p;
    auto A = cross(t.pos1 - t.pos0, t.pos2 - t.pos0);
    auto A0 = cross(pv1, pv2);
    auto A1 = cross(pv2, pv0);
    auto A2 = cross(pv0, pv1);

    auto A_inv = ScalarT(1) / A;
    return {A0 * A_inv, A1 * A_inv, A2 * A_inv};
}

template <class ScalarT>
[[nodiscard]] constexpr comp<3, ScalarT> coordinates(triangle<3, ScalarT> const& t, pos<3, ScalarT> const& p)
{
    auto pv0 = t.pos0 - p;
    auto pv1 = t.pos1 - p;
    auto pv2 = t.pos2 - p;
    auto n = cross(t.pos1 - t.pos0, t.pos2 - t.pos0);
    auto A = length(n);
    n /= A;
    auto A0 = dot(cross(pv1, pv2), n);
    auto A1 = dot(cross(pv2, pv0), n);
    auto A2 = dot(cross(pv0, pv1), n);

    auto A_inv = ScalarT(1) / A;
    return {A0 * A_inv, A1 * A_inv, A2 * A_inv};
}

template <int D, class ScalarT>
[[nodiscard]] constexpr ScalarT coordinates(segment<D, ScalarT> const& s, pos<D, ScalarT> const& p)
{
    auto d = s.pos1 - s.pos0;
    auto t = dot(p - s.pos0, d) / dot(d, d);
    return t;
}

template <int D, class ScalarT>
[[nodiscard]] constexpr ScalarT coordinates(ray<D, ScalarT> const& r, pos<D, ScalarT> const& p)
{
    return dot(p - r.origin, r.dir);
}

template <int D, class ScalarT>
[[nodiscard]] constexpr ScalarT coordinates(line<D, ScalarT> const& l, pos<D, ScalarT> const& p)
{
    return dot(p - l.pos, l.dir);
}

template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr comp<D, ScalarT> coordinates(aabb<D, ScalarT, TraitsT> const& s, pos<D, ScalarT> const& p)
{
    return tg::comp<D, ScalarT>(p - s.min) / tg::comp<D, ScalarT>(s.max - s.min);
}

template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr comp<D, ScalarT> coordinates(box<D, ScalarT, D, TraitsT> const& b, pos<D, ScalarT> const& p)
{
    return comp(inverse(b.half_extents) * (p - b.center));
}
template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr comp<2, ScalarT> coordinates(box<2, ScalarT, 3, TraitsT> const& b, pos<3, ScalarT> const& p)
{
    auto pPlane = project(p, plane<3, ScalarT>(normal_of(b), b.center));
    auto r = pPlane - b.center;

    comp<2, ScalarT> pLocal;
    auto len0 = length(b.half_extents[0]);
    auto len1 = length(b.half_extents[1]);
    pLocal.comp0 = len0 == ScalarT(0) ? ScalarT(0) : dot(b.half_extents[0] / len0, r) / len0;
    pLocal.comp1 = len1 == ScalarT(0) ? ScalarT(0) : dot(b.half_extents[1] / len1, r) / len1;
    return pLocal;
}

} // namespace tg
