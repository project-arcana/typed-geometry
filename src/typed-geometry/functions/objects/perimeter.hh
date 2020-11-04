#pragma once

#include <typed-geometry/types/objects/aabb.hh>
#include <typed-geometry/types/objects/box.hh>
#include <typed-geometry/types/objects/ellipse.hh>
#include <typed-geometry/types/objects/hemisphere.hh>
#include <typed-geometry/types/objects/quad.hh>
#include <typed-geometry/types/objects/sphere.hh>
#include <typed-geometry/types/objects/triangle.hh>

#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/vec.hh>

#include <typed-geometry/detail/operators/ops_vec.hh>
#include <typed-geometry/detail/scalar_traits.hh>
#include <typed-geometry/functions/basic/scalar_math.hh>
#include <typed-geometry/functions/vector/distance.hh>
#include <typed-geometry/functions/vector/length.hh>

namespace tg
{
template <class ScalarT, int D, class TraitsT>
[[nodiscard]] constexpr ScalarT perimeter_of(sphere<2, ScalarT, D, TraitsT> const& s)
{
    return ScalarT(2) * tg::pi_scalar<ScalarT> * s.radius;
}

template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr ScalarT perimeter_of(hemisphere<2, ScalarT, TraitsT> const& s)
{
    if constexpr (std::is_same_v<TraitsT, boundary_no_caps_tag>)
        return tg::pi_scalar<ScalarT> * s.radius;
    else
        return tg::pi_scalar<ScalarT> * s.radius + ScalarT(2) * s.radius;
}

// Only an approximation, but a good one
template <class ScalarT, int D, class TraitsT>
[[nodiscard]] constexpr ScalarT perimeter_of(ellipse<2, ScalarT, D, TraitsT> const& e)
{
    // see Approximation 3 of https://www.mathsisfun.com/geometry/ellipse-perimeter.html
    const auto a = length(e.semi_axes[0]);
    const auto b = length(e.semi_axes[1]);
    const auto h = ScalarT(3) * pow2(a - b) / pow2(a + b);
    return tg::pi_scalar<ScalarT> * (a + b) * (ScalarT(1) + h / (ScalarT(10 + sqrt(ScalarT(4) - h))));
}

template <int D, class ScalarT>
[[nodiscard]] constexpr ScalarT perimeter_of(triangle<D, ScalarT> const& t)
{
    return distance(t.pos0, t.pos1) + distance(t.pos1, t.pos2) + distance(t.pos2, t.pos0);
}

template <int D, class ScalarT>
[[nodiscard]] constexpr ScalarT perimeter_of(quad<D, ScalarT> const& q)
{
    return distance(q.pos00, q.pos10) + distance(q.pos10, q.pos11) + distance(q.pos11, q.pos01) + distance(q.pos01, q.pos00);
}

template <class ScalarT, int D, class TraitsT>
[[nodiscard]] constexpr ScalarT perimeter_of(box<2, ScalarT, D, TraitsT> const& b)
{
    return (length(b.half_extents[0]) + length(b.half_extents[1])) * ScalarT(4);
}

template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr ScalarT perimeter_of(aabb<2, ScalarT, TraitsT> const& b)
{
    const auto s = size<2, ScalarT>(b.max - b.min);
    return ScalarT(2) * (s.width + s.height);
}


template <class ObjectT>
[[deprecated("use perimeter_of")]] [[nodiscard]] constexpr auto perimeter(ObjectT const& o) -> decltype(perimeter_of(o))
{
    return perimeter_of(o);
}
} // namespace tg
