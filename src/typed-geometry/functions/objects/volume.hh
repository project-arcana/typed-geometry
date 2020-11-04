#pragma once

#include <typed-geometry/detail/operators/ops_vec.hh>
#include <typed-geometry/detail/scalar_traits.hh>
#include <typed-geometry/functions/basic/constants.hh>
#include <typed-geometry/functions/basic/scalar_math.hh>
#include <typed-geometry/functions/vector/length.hh>
#include <typed-geometry/types/objects/aabb.hh>
#include <typed-geometry/types/objects/box.hh>
#include <typed-geometry/types/objects/capsule.hh>
#include <typed-geometry/types/objects/cylinder.hh>
#include <typed-geometry/types/objects/ellipse.hh>
#include <typed-geometry/types/objects/hemisphere.hh>
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
    return ScalarT(8) * sqrt(length_sqr(b.half_extents[0]) * length_sqr(b.half_extents[1]) * length_sqr(b.half_extents[2]));
}

template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr ScalarT volume_of(sphere<3, ScalarT, 3, TraitsT> const& s)
{
    return ScalarT(4) / ScalarT(3) * tg::pi_scalar<ScalarT> * pow3(s.radius);
}

template <class ScalarT>
[[nodiscard]] constexpr ScalarT volume_of(hemisphere<3, ScalarT> const& h)
{
    return ScalarT(2) / ScalarT(3) * tg::pi_scalar<ScalarT> * pow3(h.radius);
}

template <class ScalarT>
[[nodiscard]] constexpr ScalarT volume_of(ellipse<3, ScalarT> const& e)
{
    return ScalarT(4) / ScalarT(3) * tg::pi_scalar<ScalarT> * length(e.semi_axes[0]) * length(e.semi_axes[1]) * length(e.semi_axes[2]);
}

template <class ScalarT>
[[nodiscard]] constexpr ScalarT volume_of(cylinder<3, ScalarT> const& c)
{
    return tg::pi_scalar<ScalarT> * pow2(c.radius) * length(c.axis);
}

template <class ScalarT>
[[nodiscard]] constexpr ScalarT volume_of(capsule<3, ScalarT> const& c)
{
    return tg::pi_scalar<ScalarT> * pow2(c.radius) * (ScalarT(4) / ScalarT(3) * c.radius + length(c.axis));
}

template <class BaseT>
[[nodiscard]] constexpr typename BaseT::scalar_t volume_of(pyramid<BaseT> const& p)
{
    using T = typename BaseT::scalar_t;
    return area_of(p.base) * p.height * T(1) / T(3);
}


template <class ObjectT>
[[deprecated("use volume_of")]] [[nodiscard]] constexpr auto volume(ObjectT const& o) -> decltype(volume_of(o))
{
    return volume_of(o);
}
} // namespace tg
