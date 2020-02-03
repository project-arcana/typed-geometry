#pragma once

#include <typed-geometry/types/objects/box.hh>
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
template <class ScalarT>
[[nodiscard]] constexpr ScalarT perimeter(sphere<2, ScalarT> const& v)
{
    return ScalarT(2) * tg::pi<ScalarT>.radians() * v.radius;
}

template <class ScalarT>
[[nodiscard]] constexpr ScalarT perimeter(sphere<2, ScalarT, 3> const& v)
{
    return ScalarT(2) * tg::pi<ScalarT>.radians() * v.radius;
}

template <int D, class ScalarT>
[[nodiscard]] constexpr ScalarT perimeter(triangle<D, ScalarT> const& v)
{
    return distance(v.pos0, v.pos1) + distance(v.pos1, v.pos2) + distance(v.pos2, v.pos0);
}

template <int D, class ScalarT>
[[nodiscard]] constexpr ScalarT perimeter(quad<D, ScalarT> const& v)
{
    return distance(v.pos00, v.pos10) + distance(v.pos10, v.pos11) + distance(v.pos11, v.pos01) + distance(v.pos01, v.pos00);
}

template <class ScalarT>
[[nodiscard]] constexpr ScalarT perimeter(box<2, ScalarT> const& v)
{
    return (length(v.half_extents[0]) + length(v.half_extents[1])) * ScalarT(4);
}

template <class ScalarT>
[[nodiscard]] constexpr ScalarT perimeter(box<2, ScalarT, 3> const& v)
{
    return (length(v.half_extents[0]) + length(v.half_extents[1])) * ScalarT(4);
}

} // namespace tg
