#pragma once

#include <typed-geometry/common/constants.hh>
#include <typed-geometry/common/scalar_math.hh>
#include <typed-geometry/detail/operators/ops_vec.hh>
#include <typed-geometry/detail/scalar_traits.hh>
#include <typed-geometry/types/objects/aabb.hh>
#include <typed-geometry/types/objects/ball.hh>
#include <typed-geometry/types/objects/box.hh>
#include <typed-geometry/types/objects/cone.hh>
#include <typed-geometry/types/objects/cylinder.hh>
#include <typed-geometry/types/objects/pyramid.hh>
#include <typed-geometry/types/size.hh>

#include "length.hh"

namespace tg
{
template <class ScalarT>
TG_NODISCARD constexpr ScalarT volume(size<3, ScalarT> const& s)
{
    return s.width * s.height * s.depth;
}

template <class ScalarT>
TG_NODISCARD constexpr ScalarT volume(aabb<3, ScalarT> const& b)
{
    return volume(size<3, ScalarT>(b.max - b.min));
}

template <class ScalarT>
TG_NODISCARD constexpr ScalarT volume(box<3, ScalarT> const& b)
{
    return 8 * sqrt(length_sqr(b.half_extents[0]) * length_sqr(b.half_extents[1]) * length_sqr(b.half_extents[2]));
}

template <class ScalarT>
TG_NODISCARD constexpr ScalarT volume(ball<3, ScalarT> const& b)
{
    return (tg::pi_scalar<ScalarT> * ScalarT(4) / ScalarT(3)) * tg::pow3(b.radius);
}

template <class ScalarT>
TG_NODISCARD constexpr ScalarT volume(cylinder<3, ScalarT> const& b)
{
    return (tg::pi_scalar<ScalarT> * pow2(b.radius)) * length(b.axis);
}

template <class ScalarT>
TG_NODISCARD constexpr ScalarT volume(pyramid<3, ScalarT> const& b)
{
    return area(b.base) * b.height * ScalarT(1) / ScalarT(3);
}

template <class ScalarT>
TG_NODISCARD constexpr ScalarT volume(cone<3, ScalarT> const& b)
{
    return area(b.base) * b.height * ScalarT(1) / ScalarT(3);
}

} // namespace tg
