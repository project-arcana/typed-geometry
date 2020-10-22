#pragma once

#include <typed-geometry/detail/operators/ops_vec.hh>
#include <typed-geometry/detail/scalar_traits.hh>
#include <typed-geometry/functions/basic/constants.hh>
#include <typed-geometry/functions/basic/scalar_math.hh>
#include <typed-geometry/functions/vector/length.hh>
#include <typed-geometry/types/objects/aabb.hh>
#include <typed-geometry/types/objects/box.hh>
#include <typed-geometry/types/objects/cylinder.hh>
#include <typed-geometry/types/objects/pyramid.hh>
#include <typed-geometry/types/objects/sphere.hh>
#include <typed-geometry/types/size.hh>

namespace tg
{
template <class ScalarT>
[[nodiscard]] constexpr ScalarT volume_of(size<3, ScalarT> const& s)
{
    return s.width * s.height * s.depth;
}

template <class ScalarT>
[[nodiscard]] constexpr ScalarT volume_of(aabb<3, ScalarT> const& b)
{
    return volume_of(size<3, ScalarT>(b.max - b.min));
}

template <class ScalarT>
[[nodiscard]] constexpr ScalarT volume_of(box<3, ScalarT> const& b)
{
    return 8 * sqrt(length_sqr(b.half_extents[0]) * length_sqr(b.half_extents[1]) * length_sqr(b.half_extents[2]));
}

template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr ScalarT volume_of(sphere<3, ScalarT, 3, TraitsT> const& b)
{
    return (tg::pi_scalar<ScalarT> * ScalarT(4) / ScalarT(3)) * tg::pow3(b.radius);
}

template <class ScalarT>
[[nodiscard]] constexpr ScalarT volume_of(cylinder<3, ScalarT> const& b)
{
    return (tg::pi_scalar<ScalarT> * pow2(b.radius)) * length(b.axis);
}

template <class BaseT>
[[nodiscard]] constexpr typename BaseT::scalar_t volume_of(pyramid<BaseT> const& b)
{
    using T = typename BaseT::scalar_t;
    return area_of(b.base) * b.height * T(1) / T(3);
}


template <class ObjectT>
[[deprecated("use volume_of")]] [[nodiscard]] constexpr auto volume(ObjectT const& o) -> decltype(volume_of(o))
{
    return volume_of(o);
}
} // namespace tg
