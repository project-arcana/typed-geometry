#pragma once

#include <typed-geometry/types/objects/ball.hh>
#include <typed-geometry/types/objects/circle.hh>
#include <typed-geometry/types/objects/cylinder.hh>
#include <typed-geometry/types/objects/disk.hh>
#include <typed-geometry/types/objects/sphere.hh>
#include <typed-geometry/types/objects/rect.hh>

#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/vec.hh>

#include <typed-geometry/common/scalar_math.hh>
#include <typed-geometry/detail/operators/ops_vec.hh>
#include <typed-geometry/detail/scalar_traits.hh>

namespace tg
{
template <class ScalarT>
TG_NODISCARD constexpr ScalarT perimeter(ball<2, ScalarT> const& b)
{
    return 2 * tg::pi<ScalarT>.radians() * b.radius;
}

template <class ScalarT>
TG_NODISCARD constexpr ScalarT perimeter(sphere<2, ScalarT> const& b)
{
    return 2 * tg::pi<ScalarT>.radians() * b.radius;
}

template <class ScalarT>
TG_NODISCARD constexpr ScalarT perimeter(disk<2, ScalarT> const& b)
{
    return 2 * tg::pi<ScalarT>.radians() * b.radius;
}

template <class ScalarT>
TG_NODISCARD constexpr ScalarT perimeter(disk<3, ScalarT> const& b)
{
    return 2 * tg::pi<ScalarT>.radians() * b.radius;
}

template <class ScalarT>
TG_NODISCARD constexpr ScalarT perimeter(circle<2, ScalarT> const& b)
{
    return 2 * tg::pi<ScalarT>.radians() * b.radius;
}

template <class ScalarT>
TG_NODISCARD constexpr ScalarT perimeter(circle<3, ScalarT> const& b)
{
    return 2 * tg::pi<ScalarT>.radians() * b.radius;
}

template <class ScalarT>
TG_NODISCARD constexpr ScalarT perimeter(rect<2, ScalarT> const& b)
{
    return 4 * b.length;
}

template <class ScalarT>
TG_NODISCARD constexpr ScalarT perimeter(rect<3, ScalarT> const& b)
{
    return 4 * b.length;
}


} // namespace tg
