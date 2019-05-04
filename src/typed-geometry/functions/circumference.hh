#pragma once

#include <typed-geometry/types/objects/ball.hh>
#include <typed-geometry/types/objects/cylinder.hh>
#include <typed-geometry/types/objects/disk.hh>
#include <typed-geometry/types/objects/sphere.hh>

#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/vec.hh>

#include <typed-geometry/common/scalar_math.hh>
#include <typed-geometry/detail/operators/ops_vec.hh>
#include <typed-geometry/detail/scalar_traits.hh>

namespace tg
{
template <class ScalarT>
TG_NODISCARD constexpr squared_result<ScalarT> circumference(ball<2, ScalarT> const& b)
{
    return 2 * tg::pi<ScalarT>.radians() * b.radius;
}

template <class ScalarT>
TG_NODISCARD constexpr squared_result<ScalarT> circumference(ball<3, ScalarT> const& b)
{
    return 2 * tg::pi<ScalarT>.radians() * b.radius;
}

template <class ScalarT>
TG_NODISCARD constexpr squared_result<ScalarT> circumference(sphere<2, ScalarT> const& b)
{
    return 2 * tg::pi<ScalarT>.radians() * b.radius;
}

template <class ScalarT>
TG_NODISCARD constexpr squared_result<ScalarT> circumference(sphere<3, ScalarT> const& b)
{
    return 2 * tg::pi<ScalarT>.radians() * b.radius;
}

template <class ScalarT>
TG_NODISCARD constexpr squared_result<ScalarT> circumference(disk<2, ScalarT> const& b)
{
    return 2 * tg::pi<ScalarT>.radians() * b.radius;
}

template <class ScalarT>
TG_NODISCARD constexpr squared_result<ScalarT> circumference(disk<3, ScalarT> const& b)
{
    return 2 * tg::pi<ScalarT>.radians() * b.radius;
}
} // namespace tg
