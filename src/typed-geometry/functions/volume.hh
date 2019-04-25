#pragma once

#include <typed-geometry/common/constants.hh>
#include <typed-geometry/common/scalar_math.hh>
#include <typed-geometry/detail/operators/ops_vec.hh>
#include <typed-geometry/detail/scalar_traits.hh>
#include <typed-geometry/types/objects/aabb.hh>
#include <typed-geometry/types/objects/ball.hh>
#include <typed-geometry/types/objects/box.hh>
#include <typed-geometry/types/size.hh>

#include "length.hh"

namespace tg
{
template <class ScalarT>
TG_NODISCARD constexpr squared_result<ScalarT> volume(size<3, ScalarT> const& s)
{
    return s.width * s.height * s.depth;
}

template <class ScalarT>
TG_NODISCARD constexpr squared_result<ScalarT> volume(aabb<3, ScalarT> const& b)
{
    return volume(size<3, ScalarT>(b.max - b.min));
}

template <class ScalarT>
TG_NODISCARD constexpr squared_result<ScalarT> volume(box<3, ScalarT> const& b)
{
    return 8 * tg::sqrt(length2(b.half_extents[0]) * length2(b.half_extents[1]) * length2(b.half_extents[2]));
}

template <class ScalarT>
TG_NODISCARD constexpr squared_result<ScalarT> volume(ball<3, ScalarT> const& b)
{
    return (tg::pi<ScalarT>.radians() * ScalarT(4) / ScalarT(3)) * tg::pow3(b.radius);
}
} // namespace tg