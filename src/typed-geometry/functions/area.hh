#pragma once

#include <typed-geometry/types/objects/ball.hh>
#include <typed-geometry/types/objects/box.hh>
#include <typed-geometry/types/objects/sphere.hh>
#include <typed-geometry/types/objects/triangle.hh>
#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/size.hh>
#include <typed-geometry/types/vec.hh>

#include <typed-geometry/detail/operators/ops_vec.hh>
#include <typed-geometry/detail/scalar_traits.hh>
#include <typed-geometry/detail/scalars/scalar_math.hh>

namespace tg
{
template <class ScalarT>
TG_NODISCARD constexpr squared_result<ScalarT> area(size<2, ScalarT> const& s)
{
    return s.width * s.height;
}

template <class ScalarT>
TG_NODISCARD constexpr squared_result<ScalarT> area(box<2, ScalarT> const& b)
{
    return area(size<2, ScalarT>(b.max - b.min));
}

template <class ScalarT>
TG_NODISCARD constexpr fractional_result<ScalarT> signed_area(triangle<2, ScalarT> const& b)
{
    return cross(b.v1 - b.v0, b.v2 - b.v0) * fractional_result<ScalarT>(0.5);
}

template <class ScalarT>
TG_NODISCARD constexpr fractional_result<ScalarT> area(triangle<2, ScalarT> const& b)
{
    return abs(signed_area(b));
}

template <class ScalarT>
TG_NODISCARD constexpr fractional_result<ScalarT> area(triangle<3, ScalarT> const& b)
{
    return length(cross(b.v1 - b.v0, b.v2 - b.v0)) * fractional_result<ScalarT>(0.5);
}

template <class ScalarT>
TG_NODISCARD constexpr squared_result<ScalarT> area(ball<2, ScalarT> const& b)
{
    return tg::pi<ScalarT>.radians() * tg::pow2(b.radius);
}

template <class ScalarT>
TG_NODISCARD constexpr squared_result<ScalarT> area(ball<3, ScalarT> const& b)
{
    return (tg::pi<ScalarT>.radians() * ScalarT(4)) * tg::pow2(b.radius);
}

template <class ScalarT>
TG_NODISCARD constexpr squared_result<ScalarT> area(sphere<3, ScalarT> const& b)
{
    return (tg::pi<ScalarT>.radians() * ScalarT(4)) * tg::pow2(b.radius);
}
} // namespace tg
