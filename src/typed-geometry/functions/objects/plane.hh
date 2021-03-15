#pragma once

#include <typed-geometry/functions/objects/normal.hh>
#include <typed-geometry/types/objects/box.hh>
#include <typed-geometry/types/objects/ellipse.hh>
#include <typed-geometry/types/objects/plane.hh>
#include <typed-geometry/types/objects/sphere.hh>
#include <typed-geometry/types/objects/triangle.hh>

// tg::plane_of(obj) returns the supporting plane of an object
// (when unambiguously defined)
// TODO: maybe also for "base planes" as in cone / hemisphere / pyramid?

namespace tg
{
template <int D, class ScalarT>
[[nodiscard]] constexpr plane<D, ScalarT> plane_of(plane<D, ScalarT> const& p)
{
    return p;
}

template <int D, class ScalarT>
[[nodiscard]] constexpr plane<D, ScalarT> plane_of(halfspace<D, ScalarT> const& p)
{
    return plane<3, ScalarT>(p.normal, p.dis);
}

template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr plane<3, ScalarT> plane_of(box<2, ScalarT, 3, TraitsT> const& b)
{
    return plane<3, ScalarT>(normal_of(b), b.center);
}

template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr plane<3, ScalarT> plane_of(ellipse<2, ScalarT, 3, TraitsT> const& e)
{
    return plane<3, ScalarT>(normal_of(e), e.center);
}

template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr plane<3, ScalarT> plane_of(sphere<2, ScalarT, 3, TraitsT> const& s)
{
    return plane<3, ScalarT>(s.normal, s.center);
}

template <class ScalarT>
[[nodiscard]] constexpr plane<3, ScalarT> plane_of(triangle<3, ScalarT> const& t)
{
    return plane<3, ScalarT>(normal_of(t), t.pos0);
}

}
